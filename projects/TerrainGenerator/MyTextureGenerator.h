#pragma once
#include <vector>
#include "2DAControl.h"

class MyTextureGenerator
{
public:
	MyTextureGenerator();
	~MyTextureGenerator();

	static void generateNormals(const std::vector<float>& heightfield, int resolution,
		std::vector<bestGroup::Vec3f> &normalsOut);

	static void saveNormalsToImage(const std::vector<bestGroup::Vec3f>& normalsOut, int resolution, const char* filename);
};

