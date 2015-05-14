#pragma once

#include <vector>
#include "2DAControl.h"
#include <iostream>

class Smoothing
{
public:
	
	//helper function to check if a set of coordinates is inside the array with width and height
	static bool isValidCoord(int x, int y, int width, int height);

	//helper function to use a modified bresenham-algorithm to get coordinates of points inside a
	//circular shape around the point with the coordinates x0 and y0.
	//only returns valid coordinates
	static std::vector<bestGroup::Vec2f> getSmoothingCoords(int x0,
		int y0, int range, int width, int height);

	//very fast smoothing algorithm to only smooth in a very small, fixed radius
	static void squareSmoothing(std::vector<float> &array2D_, int width_, int height_);

	//circular (more accurate) smoothing algorithm with variable range but slower
	static void circularSmoothing(std::vector<float> &array2D_, int width_, int height_, int  range);

	//uses squareSmoothing n times in sequence
	static void squareSmoothing_nTimes(std::vector<float> &array2D_, int width_, int height_, int n);

	//uses circular smoothing n times in sequence
	static void circularSmoothing_nTimes(std::vector<float> &array2D_, int width_, int height_, int n, int range);


	static void anotherSimpleSmoothing(std::vector<float> &array2d_, int width, int n, int range);

};

