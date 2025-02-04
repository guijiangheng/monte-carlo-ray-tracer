#include "linear-octree.hpp"

#include <glm/gtx/norm.hpp>

#include "../common/constexpr-math.hpp"
#include "../common/util.hpp"

template <class Data>
LinearOctree<Data>::LinearOctree(Octree<Data> &octree_root)
{
    size_t octree_size = 0, data_size = 0;
    octreeSize(octree_root, octree_size, data_size);

    if (octree_size == 0 || data_size == 0) return;

    uint32_t df_idx = root_idx;
    uint64_t data_idx = 0;
    compact(&octree_root, df_idx, data_idx, true);
}

template <class Data>
std::vector<SearchResult<Data>> LinearOctree<Data>::knnSearch(const glm::dvec3& p, size_t k, double radius_est) const
{
    std::vector<SearchResult<Data>> result;

    if (linear_tree.empty()) return result;
    
    if (k > ordered_data.size()) k = ordered_data.size();

    result.reserve(k);

    double min_distance2 = -1.0;
    double max_distance2 = pow2(radius_est);
    double distance2 = linear_tree[root_idx].BB.distance2(p);

    auto to_visit = reservedPriorityQueue<KNNode>(64);

    KNNode current(root_idx, distance2);

    while (true)
    {
        if (current.octant != null_idx)
        {
            if (linear_tree[current.octant].leaf)
            {
                uint64_t end_idx = linear_tree[current.octant].start_data + linear_tree[current.octant].num_data;
                for (uint64_t i = linear_tree[current.octant].start_data; i < end_idx; i++)
                {
                    double distance2 = glm::distance2(ordered_data[i].pos(), p);
                    if (distance2 <= max_distance2 && distance2 > min_distance2)
                    {
                        to_visit.emplace(distance2, i);
                    }
                }
            }
            else
            {
                uint32_t child_octant = current.octant + 1;
                while (child_octant != null_idx)
                {
                    const auto& node = linear_tree[child_octant];

                    distance2 = node.BB.distance2(p);
                    if (distance2 <= max_distance2 && node.BB.max_distance2(p) > min_distance2)
                    {
                        to_visit.emplace(child_octant, distance2);
                    }
                    child_octant = node.next_sibling;
                }
            }
        }
        else
        {
            result.emplace_back(ordered_data[current.data], current.distance2);
            if (result.size() == k) return result;
        }

        if (to_visit.empty())
        {
            // Maximum search sphere doesn't contain k points. Increase radius and
            // traverse octree again, ignoring the already found closest points.
            max_distance2 *= 2.0;
            if (!result.empty()) min_distance2 = result.back().distance2;
            current = KNNode(root_idx, linear_tree[root_idx].BB.distance2(p));
        }
        else
        {
            current = to_visit.top();
            to_visit.pop();
        }
    }
}

template <class Data>
std::vector<SearchResult<Data>> LinearOctree<Data>::radiusSearch(const glm::dvec3& p, double radius) const
{
    std::vector<SearchResult<Data>> result;
    if (linear_tree.empty()) return result;
    recursiveRadiusSearch(root_idx, p, pow2(radius), result);
    return result;
}

template <class Data>
void LinearOctree<Data>::recursiveRadiusSearch(const uint32_t current, const glm::dvec3& p, double radius2, std::vector<SearchResult<Data>>& result) const
{
    if (linear_tree[current].leaf)
    {
        uint64_t end_idx = linear_tree[current].start_data + linear_tree[current].num_data;
        for (uint64_t i = linear_tree[current].start_data; i < end_idx; i++)
        {
            double distance2 = glm::distance2(ordered_data[i].pos(), p);
            if (distance2 <= radius2)
            {
                result.emplace_back(ordered_data[i], distance2);
            }
        }
    }
    else
    {
        uint32_t child_octant = current + 1;
        while (child_octant != null_idx)
        {
            if (linear_tree[child_octant].BB.distance2(p) <= radius2)
            {
                recursiveRadiusSearch(child_octant, p, radius2, result);
            }
            child_octant = linear_tree[child_octant].next_sibling;
        }
    }
}

template <class Data>
void LinearOctree<Data>::octreeSize(const Octree<Data> &octree_root, size_t &size, size_t &data_size) const
{
    if (octree_root.leaf() && octree_root.data_vec.empty()) return;

    size++;
    data_size += octree_root.data_vec.size();

    if (!octree_root.leaf())
    {
        for (const auto &octant : octree_root.octants)
        {
            octreeSize(*octant, size, data_size);
        }
    }
}

template <class Data>
BoundingBox LinearOctree<Data>::compact(Octree<Data> *node, uint32_t &df_idx, uint64_t &data_idx, bool last)
{
    uint32_t idx = df_idx++;

    linear_tree.emplace_back();
    linear_tree[idx].leaf = (uint8_t)node->leaf();
    linear_tree[idx].start_data = data_idx;
    linear_tree[idx].num_data = (uint16_t)node->data_vec.size();

    BoundingBox BB;
    for (auto&& data : node->data_vec) BB.merge(data.pos());
    ordered_data.insert(ordered_data.end(), node->data_vec.begin(), node->data_vec.end());
    data_idx += node->data_vec.size();
    node->data_vec.clear();
    node->data_vec.shrink_to_fit();

    if (!node->leaf())
    {
        std::vector<size_t> use;
        for (size_t i = 0; i < node->octants.size(); i++)
        {
            if (!(node->octants[i]->leaf() && node->octants[i]->data_vec.empty()))
            {
                use.push_back(i);
            }
        }
        for (const auto &i : use)
        {
            BB.merge(compact(node->octants[i].get(), df_idx, data_idx, i == use.back()));
        }
    }
    node->octants.clear();
    node->octants.shrink_to_fit();
    linear_tree[idx].next_sibling = last ? null_idx : df_idx;
    linear_tree[idx].BB = BB;
    return BB;
}
