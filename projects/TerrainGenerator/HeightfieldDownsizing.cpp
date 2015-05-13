#include "HeightfieldDownsizing.h"
#include <vector>
#include <iostream>
#include "2DAControl.h"

/* getMedian
heightfieldOld has to be an 1D Vector
=> gets the Median value for a scale x scale grid from heightfiledOld 
and returns this float number
start_x and start_y are the x,y coordinates, where the grid starts
e.g.
start_x = 0
start y = 0
scale = 4
median taken	x:0 to x:3 
		and		y:0 to y:4	*/
float HeightfieldDownsizing::getMedian(std::vector<float>& heightfieldOld, int resolution, int start_x, int start_y, int scale){
	
	// var, in which all values of the fields are stored
	float median = 0.0f;
	
	// iterate through the next (int) scale rows 
	for (int y = start_y; y < start_y+scale ; y++)
	{
		//iterate through the cols
		for (int x = start_x; x < start_x+scale; x++)
		{
			// save the value to median
			median += heightfieldOld[IDX(x, y, resolution)];
		}
	}
	//std::cout << median / scale << std::endl;

	// calc and return the median
	return median / (float) (scale * scale);

}

/*
Downsizes heightfieldOld square of scale and saves the median values into newHeightmap
e.g. heightfieldOld is 4096 * 4096
thus newHeightmap will be (4096 / 4) * (4096 / 4) = 1024 * 1024
and will contain the median values of the 4 * 4 grids from heightfieldOld */
void HeightfieldDownsizing::downsize(std::vector<float>& heightfieldOld, int resolutionOld, int scale, std::vector<float>& newHeightmap){
	std::cout << "starting downsizing..." << std::endl;


	// Resolution of newHeightmap
	int newResolution = resolutionOld / scale;
	
	// check if downsizing with the scale factor is possible, if not, throw exception
	if ((resolutionOld % scale) != 0)
		throw "Downsizing not possible, scale and resolution doesn't fit";
	
	// check if heightfield has the size resolutionOld * resolutionOld
	if ((resolutionOld * resolutionOld) != heightfieldOld.size())
		throw "heightfiled and resolution don't fit";

	// check if newHeightmap is newResolution *  newResolution
	if (newResolution * newResolution != newHeightmap.size())
		throw "Heightmap, resolution and scale don't fit";

	// tmp float var to save the median 
	float tmp_median;

	// iterate through every pixel in with the step scale-1
	for (int y = 0; y < resolutionOld; y += scale)
	{
		for (int x = 0; x < resolutionOld; x += scale)
		{
			// get the median of the scale x scale field starting at x,y
			tmp_median = HeightfieldDownsizing::getMedian(heightfieldOld, resolutionOld, x, y, scale);

			// save the median into the newHeightmap at the correct x,y position
			newHeightmap[IDX(x / scale, y / scale, newResolution)] = tmp_median;

		}
	}
	
	std::cout << "downsizing completed..." << std::endl;
}

// func prints the the grid from a specific start_x and start_y and the median from this grid written in newHeightmap
void HeightfieldDownsizing::printGrid(std::vector<float>& heightfieldOld, int resolution, int start_x, int start_y, int scale, std::vector<float>& newHeightmap){
	
	// float to calc the median from heightfieldOld (for comparison)
	float tmp_median = 0.0f;

	// iterate through the heightfieldOld and print to console
	for (int y = start_y; y < start_y + scale; y++)
	{
		for (int x = start_x; x < start_x + scale; x++)
		{

		// print value to console
			std::cout << " " << heightfieldOld[IDX(x, y, resolution)];
			
			tmp_median += heightfieldOld[IDX(x, y, resolution)];
			
		}
		
		// newline
		std::cout << std::endl;
	}
	
	// print the median from this grid
	std::cout << "Median from this grid is: " << tmp_median / (float) (scale * scale) << std::endl;

	// print the median of this grid from newHeightmap
	std::cout << "Median saved in newHeightmap: ";
	std::cout << newHeightmap[IDX(start_x / scale, start_y / scale, resolution / scale)] << std::endl;

}



