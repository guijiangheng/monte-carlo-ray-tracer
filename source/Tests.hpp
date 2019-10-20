#pragma once

#include <iostream>
#include <fstream>
#include <string>

#ifdef _WIN32
    #include "windows.h"
    #include "psapi.h"
#endif

#include "Scene.hpp"
#include "PhotonMap.hpp"

//inline void testPhotonMap(
//    Scene& s, 
//    size_t photon_emissions, 
//    size_t num_radius_searches, 
//    uint16_t start_node_points, 
//    uint16_t end_node_points, 
//    uint16_t node_points_step,
//    const std::string& filename)
//{
//#ifdef _WIN32
//    std::ofstream log(filename);
//    std::cout << "max_node_data, mem_usage_MB, build_msec, find_msec, destruct_msec" << std::endl;
//    log << "max_node_data, mem_usage_MB, build_msec, find_msec, destruct_msec" << std::endl;
//
//    auto begin = std::chrono::system_clock::now();
//    auto end = std::chrono::system_clock::now();
//
//    for (uint16_t max_node_data = start_node_points; max_node_data <= end_node_points; max_node_data += node_points_step)
//    {
//        if (max_node_data != start_node_points)
//        {
//            end = std::chrono::system_clock::now();
//            std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000.0 << std::endl;
//            log << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000.0 << std::endl;
//        }
//
//        auto build_before = std::chrono::system_clock::now();
//        PhotonMap test(s, photon_emissions, max_node_data);
//        auto build_now = std::chrono::system_clock::now();
//
//        auto before = std::chrono::system_clock::now();
//        for (size_t i = 0; i < num_radius_searches; i++)
//        {
//            glm::vec3 p(Random::range(4, 6), -5, Random::range(-1, 1));
//            auto r1 = test.direct_map.radiusSearch(p, 0.1f);
//            auto r2 = test.indirect_map.radiusSearch(p, 0.1f);
//            auto r3 = test.caustic_map.radiusSearch(p, 0.1f);
//            auto r4 = test.shadow_map.radiusSearch(p, 0.1f);
//        }
//        auto now = std::chrono::system_clock::now();
//
//        PROCESS_MEMORY_COUNTERS_EX pmc;
//        GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)& pmc, sizeof(pmc));
//        SIZE_T vmem_used = pmc.WorkingSetSize;
//
//        std::cout << max_node_data << ", ";
//        std::cout << vmem_used / 1e6 << ", ";
//        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(build_now - build_before).count() / 1000.0 << ", ";
//        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(now - before).count() / 1000.0 << ", ";
//
//        log << max_node_data << ", ";
//        log << vmem_used / 1e6 << ", ";
//        log << std::chrono::duration_cast<std::chrono::microseconds>(build_now - build_before).count() / 1000.0 << ", ";
//        log << std::chrono::duration_cast<std::chrono::microseconds>(now - before).count() / 1000.0  << ", ";
//
//        begin = std::chrono::system_clock::now();
//    }
//    end = std::chrono::system_clock::now();
//    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000.0 << std::endl;
//    log << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() / 1000.0 << std::endl;
//    log.close();
//
//#else
//    std::cout << "Photon map testing is only supported on windows at the moment." << std::endl;
//#endif
//}
