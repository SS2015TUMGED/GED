#pragma once
#include <vector>


class HeightfieldDownsizing
{
public:
	HeightfieldDownsizing();
	~HeightfieldDownsizing();

	// takes a vector with a heightmap and downscales it by (int) scale and saves it into newHeightmap
	static void downsize(std::vector<float>& heightfieldOld, int resolutionOld, int scale, std::vector<float>& newHeightmap);

	// calcs & returns the median of the scale x scale field starting at start_x, start_y
	static float getMedian(std::vector<float>& heightfieldOld, int resolution, int start_x, int start_y, int scale);

	// func prints the the grid from a specific start_x and start_y and the median from this grid written in newHeightmap (just for testing)
	static void printGrid(std::vector<float>& heightfieldOld, int resolution, int start_x, int start_y, int scale, std::vector<float>& newHeightmap);
};

