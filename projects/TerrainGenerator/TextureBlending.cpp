#include "TextureBlending.h"
#include <vector>
#include "MyTextureGenerator.h"
#include "SimpleImage.h"
#include "2DAControl.h"
#include <iostream>
#include <string>

// load the textures into the ram
//Default Texture
GEDUtils::SimpleImage default("..\\TerrainGenerator\\Textures\\Black.h");

//other Textures
GEDUtils::SimpleImage red("..\\TerrainGenerator\\Textures\\Red.h");
GEDUtils::SimpleImage yellow("..\\TerrainGenerator\\Textures\\Yellow.h");
GEDUtils::SimpleImage dirt1("..\\TerrainGenerator\\Textures\\Tex_Ground_01.h");
GEDUtils::SimpleImage rock2("..\\TerrainGenerator\\Textures\\Tex_Rock_02.h");
GEDUtils::SimpleImage rock1("..\\TerrainGenerator\\Textures\\Tex_Rock_01.h");
GEDUtils::SimpleImage grass("..\\TerrainGenerator\\Textures\\Tex_Grass_01.h");
GEDUtils::SimpleImage snow("..\\TerrainGenerator\\Textures\\Tex_Snow.h");


// Array for the Textures
std::vector<bestGroup::Tex> TextureBlending::textures;

void TextureBlending::init(){
	
	bestGroup::Tex tex = bestGroup::Tex(default,  0.0f, 0.0f, 0.0f, 0.0f);
	textures.push_back(tex);

	tex = bestGroup::Tex(rock1, 0.0f, 0.2f, 0.1, 0.3f);
	textures.push_back(tex);

	tex = bestGroup::Tex(rock2, 0.0f, 0.3f, 0.15f, 0.4f);
	textures.push_back(tex);

	tex = bestGroup::Tex(dirt1, 0.0f, 0.01f, 0.05f, 0.01f);
	textures.push_back(tex);

	tex = bestGroup::Tex(grass, 0.0f, 0.01f, 0.02f, 0.01f);
	textures.push_back(tex);
	
	tex = bestGroup::Tex(snow, 0.0f, 0.5f, 0.07f, 0.2f);
	textures.push_back(tex);
}

inline void TextureBlending::calcAlphas(float height, float slope){
	textures.shrink_to_fit();
	for (auto i = 1; i< textures.size(); i++){
		//____________HEIGHT_______________________________________________
		textures[i].alpha = 
			((-1.0f * textures[i].midpoint) / //(-midpoint / range^2)
			(textures[i].range * textures[i].range)) * 
			((height - textures[i].midpoint) * (height - textures[i].midpoint)) //(x - midpoint) ^2
			+ textures[i].midpoint
			+ 1.0f - textures[i].midpoint; // +m
		textures[i].alpha = (textures[i].alpha <= 0.0f) ? (0.0f) : textures[i].alpha;

		//____________SLOPE________________________________________________
		textures[i].alpha *= (slope + textures[i].slope);
		textures[i].alpha *= (slope + textures[i].slope);
	}
}

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
			r = (r < 0.0f) ? 0.0f : r;
			r = (r > 1.0f) ? 1.0f : r;
			g = (g < 0.0f) ? 0.0f : g;
			g = (g > 1.0f) ? 1.0f : g;
			b = (b < 0.0f) ? 0.0f : b;
			b = (b > 1.0f) ? 1.0f : b;
			image.setPixel(x, y, r, g, b);

		}
	}
	image.save(filename);
	//system("pause");
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


