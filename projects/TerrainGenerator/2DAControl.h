#pragma once

#include "SimpleImage.h"

// Access a 2D array of width w at position x / y 
#define IDX(xpos, ypos, width) ((xpos) + (ypos) * (width))

namespace bestGroup{
	/// Utility struct: A 3-dimensional float vector
	struct Vec3f{
		float x;
		float y;
		float z;

		Vec3f(float x, float y, float z) : x(x), y(y), z(z) {}
		Vec3f() : x(0), y(0), z(0) {}
	};

	// Utility struct: A 2-dimensional int vector
	struct Vec2f{
		int x;
		int y;

		Vec2f(int x, int y) : x(x), y(y) {}
		Vec2f() : x(0), y(0) {}
	};

	struct Tex{
		GEDUtils::SimpleImage image;
		float alpha;
		float midpoint;
		float range;
		float slope;

		Tex(GEDUtils::SimpleImage &image,  float alpha, float midpoint, float range, float slope) : 
			image(image), alpha(alpha), midpoint(midpoint), range(range), slope(slope) {}

	};
};

template <class Function>
__int64 time_call(Function&& f)
{
	__int64 begin = GetTickCount();
	f();
	return GetTickCount() - begin;
}
