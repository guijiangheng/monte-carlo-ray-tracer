#pragma once

#include <vector>
#include <cstdint>
#include <fstream>

//Hard coded (except for dimensions) uncompressed 24bpp true-color TGA header.
//After writing this to file, the RGB bytes can be dumped in sequence (left to right, top to bottom) to create a TGA image.
struct HeaderTGA
{
	HeaderTGA(int width, int height)
		: width(width), height(height) {}

private:
	uint8_t begin[12] = { 0, 0, 2 };
	uint16_t width;
	uint16_t height;
	uint8_t end[2] = { 24, 32 };
};

static glm::dvec3 Uncharted2Tonemap(const glm::dvec3& x)
{
	double A = 0.15, B = 0.50, C = 0.10, D = 0.20, E = 0.02, F = 0.30;

	return ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;
}

static glm::dvec3 filmic(glm::dvec3 in)
{
	//double W = 11.2;
	double W = 11.2;

	in *= 16.0;
	in *= .25; // exposure bias
	in = Uncharted2Tonemap(in);
	in /= Uncharted2Tonemap(glm::dvec3(W));

	return in;
}

static glm::dvec3 reinhard(glm::dvec3 in)
{
	in *= 16.0;
	return in / (1.0 + in);
}

struct Image
{
	Image(int width, int height)
		: blob(std::vector<glm::dvec3>((uint64_t)width* height, glm::dvec3())), width(width), height(height) { }

	void save(const std::string& filename) const
	{
		HeaderTGA header(width, height);
		std::ofstream out(filename, std::ios::binary);
		out.write(reinterpret_cast<char*>(&header), sizeof(header));
		for (const auto& p : blob)
		{
			glm::dvec3 fp = filmic(p);
			for (int c = 2; c >= 0; c--)
			{
				double v = pow(fp[c], 1.0 / 2.2);
				v = v > 1.0 ? 1.0 : v < 0.0 ? 0.0 : v;
				uint8_t pv = (uint8_t)(v * 255.0);
				out.write(reinterpret_cast<char*>(&pv), sizeof(pv));
			}
		}
		out.close();
	}

	glm::dvec3& operator()(int col, int row)
	{
		return blob[(uint64_t)row * width + col];
	}

	int width, height;

private:
	std::vector<glm::dvec3> blob;
};
