#pragma once
class TextureBlending
{
public:



	TextureBlending();
	~TextureBlending();

	void calcAlphas(float height, float slope, float& alpha1, float& alpha2, float& alpha3);

	//returns true, if the slope is steep at this x,y coordinate
	bool isPointSteep(std::vector<bestGroup::Vec3f>& normalsOut, int resolution, int x, int y);

	//returns true, if the point at x,y in the highmap is high 
	bool isPointHight(std::vector<float>& highmap, int resolution, int x, int y);

	void createImage(std::vector<float>& heightmap, std::vector<bestGroup::Vec3f>& normalsOut, int resolution);

	void getColorTiled(GEDUtils::SimpleImage image, int x, int y, float& r, float& g, float& b);

	void blend(std::vector<GEDUtils::SimpleImage> arr, int x, int y, int index, float& r, float& g, float& b);


private:
	std::vector<float> alphas;
	std::vector<GEDUtils::SimpleImage> textures;
};

