#pragma once

#include <vector>

class MyTextureGenerator
{
public:
	MyTextureGenerator();
	~MyTextureGenerator();

	void generateNormals(const std::vector<float>& heightfield, int resolution,
		std::vector<float>& normalsOut);
};

