#pragma once

#include <vector>
#include "MyTextureGenerator.h"
#include <iostream>

class Smoothing
{
public:
	
	static bool isValidCoord(int x, int y, int width, int height);
	static std::vector<bestGroup::Vec2f> getSmoothingCoords(int x0,
		int y0, int range, int width, int height);

	static void squareSmoothing(std::vector<float> &array2D_, int width_, int height_);
	static void circularSmoothing(std::vector<float> &array2D_, int width_, int height_, int  range);
	static void squareSmoothing_nTimes(std::vector<float> &array2D_, int width_, int height_, int n);
	static void circularSmoothing_nTimes(std::vector<float> &array2D_, int width_, int height_, int n, int range);

};

