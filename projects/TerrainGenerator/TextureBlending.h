#pragma once

#include "SimpleImage.h"
#include <vector>
#include "2DAControl.h"
#include <string>


class TextureBlending
{
public:

	TextureBlending();
	~TextureBlending();

	// calculates the alpha values based on height and slope
	inline void static calcAlphas(float height, float slope);

	// get the color if the tiled image on the x and y given
	inline void static getColorTiled(GEDUtils::SimpleImage &image, int x, int y, float& r, float& g, float& b);

	// calculates the blending of each layer
	inline void static blend(int x, int y, unsigned index, float& r, float& g, float& b);

	// creates the color image
	void static createImage(std::vector<float>& heightmap, std::vector<bestGroup::Vec3f>& normalsOut, int resolution, const char* filename);

	//HARDCODING IN HERE
	void static init();

private:
	//static std::vector<float> alphas;
	static std::vector<bestGroup::Tex> textures;
	//grayscaling a single pixel
	inline float static grayscale_pixel( float r, float b, float g);
	void static grayscale(unsigned resolution);
};
