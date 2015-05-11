#pragma once

#include <vector>


namespace bestGroup
{
	/// Utility struct: A 3-dimensional float vector
	struct Vec3f
	{
		float x;
		float y;
		float z;

		Vec3f(float x, float y, float z) : x(x), y(y), z(z) {}
		Vec3f() : x(0), y(0), z(0) {}
	};
}


class MyTextureGenerator
{
public:
	MyTextureGenerator();
	~MyTextureGenerator();

	static void generateNormals(const std::vector<float>& heightfield, int resolution,
		std::vector<bestGroup::Vec3f> &normalsOut);

	static void saveNormalsToImage(const std::vector<bestGroup::Vec3f>& normalsOut, int resolution, const char* filename);
};

