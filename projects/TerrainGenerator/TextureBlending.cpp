#include "TextureBlending.h"
#include <vector>
#include "MyTextureGenerator.h"
#include "SimpleImage.h"
#include "2DAControl.h"
#include <iostream>
#include <string>

// load the textures into the ram
GEDUtils::SimpleImage lowSteep("..\\..\\..\\..\\external\\textures\\ground02.jpg");
GEDUtils::SimpleImage highSteep("..\\..\\..\\..\\external\\textures\\rock4.jpg");
GEDUtils::SimpleImage highFlat("..\\..\\..\\..\\external\\textures\\rock3.jpg");
GEDUtils::SimpleImage lowFlat("..\\..\\..\\..\\external\\textures\\gras15.jpg");
GEDUtils::SimpleImage snow("..\\TerrainGenerator\\Textures\\Snow.h");

// Array for the images 
//std::vector<float> TextureBlending::alphas = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
//std::vector<GEDUtils::SimpleImage> TextureBlending::textures = { lowSteep, lowFlat, highFlat, highSteep, snow };
std::vector<bestGroup::Tex> TextureBlending::textures;

void TextureBlending::init(){
	
	bestGroup::Tex tex = bestGroup::Tex(lowFlat, "lowFlat", 0.0f, 0.15f, 0.2f);
	textures.push_back(tex);

	tex = bestGroup::Tex(highFlat, "highFlat", 0.0f, 0.6f, 0.2f);
	textures.push_back(tex);

	tex = bestGroup::Tex(lowSteep, "lowSteep", 0.0f, 0.15f, 0.6f);
	textures.push_back(tex);

	tex = bestGroup::Tex(highSteep, "highSteep", 0.0f, 0.6f, 0.6f);
	textures.push_back(tex);

	tex = bestGroup::Tex(snow, "snow", 0.0f, 0.85f, 0.55f);
	textures.push_back(tex);

}

inline void TextureBlending::calcAlphas(float height, float slope){
	textures.shrink_to_fit();
	slope *= 1/slope;
	for (auto i = 0; i< textures.size(); i++){
		textures[i].alpha = (height > textures[i].height) ? (textures[i].height / height) : (height / textures[i].height);
		textures[i].alpha *= (slope > textures[i].slope) ? (textures[i].slope / slope) : (slope / textures[i].slope);
	}
}
/*
inline void TextureBlending::calcAlphas(float height, float slope){
	slope *= slope;
	alphas[0] = 1.0f;
	alphas[1] = (1 - height) * slope + 0.1f;
	if (alphas[1] > 1.0f){ alphas[1] = 1.0f; }
	//alphas[4] = 0.0f;
	if (height > 0.5f){
		alphas[1] = 0.8f - height*slope;
		alphas[2] = height;
		alphas[3] = height* slope;
	}
	else{
		alphas[2] = height*0.99f;
		alphas[3] = height*0.99f*slope;
	}

	alphas[4] = height*slope*height*1.8f;
	if (alphas[4] > 1.0f){ alphas[4] = 1.0f; }
}
*/
inline void TextureBlending::getColorTiled(GEDUtils::SimpleImage &image, int x, int y, float& r, float& g, float& b)
{
	int imageWidth = image.getWidth();
	int imageHeight = image.getHeight();

	x = x % imageWidth;
	y = y % imageHeight;

	// return the rgb colors
	image.getPixel(x, y, r, g, b);
}

inline void TextureBlending::blend(int x, int y, unsigned index, float& r, float& g, float& b){

	// end recursion
	if (index >= textures.size()){
		return;
	}
	//std::string filename = "gray";
	//filename += ".tiff";
	//grayscale(textures[index], filename);
	// starting texture
	if (index == 0){
		getColorTiled(textures[0].image, x, y, r, g, b);
	}

	// recursion, add alphas to the pixel
	else if (index > 0){
		float alpha = textures[index].alpha;

		float tmp_r, tmp_g, tmp_b;
		getColorTiled(textures[index].image, x, y, tmp_r, tmp_g, tmp_b);

		// formula from the slides
		r = alpha * tmp_r + (1 - alpha) * r;
		g = alpha * tmp_g + (1 - alpha) * g;
		b = alpha * tmp_b + (1 - alpha) * b;

	}

	// blend the next layer
	index++;
	blend(x, y, index, r, g, b);
}

inline float TextureBlending::grayscale_pixel(float r, float b, float g){
	return (r + b + g) / 3.0f;
}

void TextureBlending::createImage(std::vector<float>& heightmap, std::vector<bestGroup::Vec3f>& normalsOut, int resolution, const char* filename){
	//initialize
	init();

	//grayscale images
	//grayscale((unsigned)resolution);

	// height and slope for calcAlphas
	float height;
	float slope;

	// r, g and b for blend
	float r, g, b = 0.0f;

	// create the new picture
	GEDUtils::SimpleImage image(resolution, resolution);

	// iterate through complete image
	for (int y = 0; y < resolution; y++)
	{
		for (int x = 0; x < resolution; x++)
		{
			// get height and slope from given vectors
			height = heightmap[IDX(x, y, resolution)];
			slope = normalsOut[IDX(x, y, resolution)].z;

			calcAlphas(height, slope);

			// call getColorTiled 4-times per pixel to get the rgb's from the textures
			blend(x, y, 0, r, g, b);

			// write rgb in the file
			image.setPixel(x, y, r, g, b);

		}
	}
	image.save(filename);
	
}

void TextureBlending::grayscale(unsigned resolution){
	float r = 0.0f, b = 0.0f, g = 0.0f;
	for (auto tex : textures){
		GEDUtils::SimpleImage gray(resolution, resolution);
		for (int y = 0; y < resolution; y++){
			for (int x = 0; x < resolution; x++){
				tex.image.getPixel(x, y, r, b, g);
				gray.setPixel(x, y, grayscale_pixel(r, b, g));
			}
		}
		//gray.save(tex.name + "_gray.tiff");
	}
}


