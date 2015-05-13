#pragma once

#include "SimpleImage.h"
#include <vector>
#include "2DAControl.h"


class TextureBlending
{
public:



	TextureBlending();
	~TextureBlending();

	void static calcAlphas(float height, float slope);

	//returns true, if the slope is steep at this x,y coordinate
	bool static isPointSteep(std::vector<bestGroup::Vec3f>& normalsOut, int resolution, int x, int y);

	//returns true, if the point at x,y in the highmap is high 
	bool static isPointHight(std::vector<float>& highmap, int resolution, int x, int y);

	void static createImage(std::vector<float>& heightmap, std::vector<bestGroup::Vec3f>& normalsOut, int resolution, const char* filename);

	void static getColorTiled(GEDUtils::SimpleImage image, int x, int y, float& r, float& g, float& b);

	void static blend(int x, int y, unsigned index, float& r, float& g, float& b);


private:
	 static std::vector<float> alphas;
	 static std::vector<GEDUtils::SimpleImage> textures;
};

