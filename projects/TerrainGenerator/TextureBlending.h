#pragma once

#include "SimpleImage.h"
#include <vector>
#include "2DAControl.h"


class TextureBlending
{
public:

	TextureBlending();
	~TextureBlending();

	// calculates the alpha values based on height and slope
	void static calcAlphas(float height, float slope);

	// get the color if the tiled image on the x and y given
	void static getColorTiled(GEDUtils::SimpleImage image, int x, int y, float& r, float& g, float& b);

	// calculates the blending of each layer
	void static blend(int x, int y, unsigned index, float& r, float& g, float& b);

	// creates the color image
	void static createImage(std::vector<float>& heightmap, std::vector<bestGroup::Vec3f>& normalsOut, int resolution, const char* filename);

private:
	 static std::vector<float> alphas;
	 static std::vector<GEDUtils::SimpleImage> textures;
};
