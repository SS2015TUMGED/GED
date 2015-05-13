#include "TextureBlending.h"
#include <vector>
#include "MyTextureGenerator.h"
#include "SimpleImage.h"


// Access a 2D array of width w at position x / y 
#define IDX(xpos, ypos, width) ((xpos) + (ypos) * (width))

TextureBlending::TextureBlending()
{
}

TextureBlending::~TextureBlending()
{
}

// load the textures into the ram
GEDUtils::SimpleImage lowFlat("..\\..\\..\\..\\external\\textures\\gras15.jpg");
GEDUtils::SimpleImage lowSteep("..\\..\\..\\..\\external\\textures\\ground02.jpg");
GEDUtils::SimpleImage highFlat("..\\..\\..\\..\\external\\textures\\pebble01.jpg");
GEDUtils::SimpleImage highSteep("..\\..\\..\\..\\external\\textures\\rock4.jpg");
GEDUtils::SimpleImage snowLand("..\\..\\..\\..\\external\\textures\\Snow.jpg");

// Array for the images
std::vector<float> TextureBlending::alphas = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
std::vector<GEDUtils::SimpleImage> TextureBlending::textures = { lowFlat, lowSteep, highFlat, highSteep, snowLand };

// given in slide03
void TextureBlending::calcAlphas(float height, float slope){
	slope *= slope;
	TextureBlending::alphas[0] = 1.0f;
	TextureBlending::alphas[1] = (1 - height) * slope;
	//alphas[4] = 0.0f;
	if (height > 0.5f){
		alphas[1] = 0.8f - height*slope;
		TextureBlending::alphas[2] = height;
		TextureBlending::alphas[3] = height* slope;
	}
	else{
		alphas[2] = height*0.99f;
		alphas[3] = height*0.99f*slope;
	}

	alphas[4] = height*slope*height*2.0f;
	if (height > 0.2f && height < 0.3){
		alphas[1] =1 -  height * 10 / 3;

	}


	if (height < 0.2f){
		for (int i = 1; i < 5; i++)
		{
			alphas[i] = 0.0f;
		}
	}

}

void TextureBlending::getColorTiled(GEDUtils::SimpleImage image, int x, int y, float& r, float& g, float& b)
{
	int imageWidth = image.getWidth();
	int imageHeight = image.getHeight();

	x = x % imageWidth;
	y = y % imageHeight;

	// return the rgb colors
	image.getPixel(x, y, r, g, b);

}

void TextureBlending::blend(int x, int y, int index, float& r, float& g, float& b){
	
	// end recursion
	if (index >= TextureBlending::textures.size()){
		return;
	}

	// starting texture
	if (index == 0){
		TextureBlending::getColorTiled(TextureBlending::textures[0], x, y, r, g, b);
	}

	// recursion, add alphas to the pixel
	else if (index > 0){
		float alpha = TextureBlending::alphas[index];

		float tmp_r, tmp_g, tmp_b;
		TextureBlending::getColorTiled(TextureBlending::textures[index], x, y, tmp_r, tmp_g, tmp_b);

		// formula from the slides
		r = alpha * tmp_r + (1 - alpha) * r;
		g = alpha * tmp_g + (1 - alpha) * g;
		b = alpha * tmp_b + (1 - alpha) * b;
		
	}

	index++;
	TextureBlending::blend(x, y, index, r, g, b);

}



// load images
void TextureBlending::createImage(std::vector<float>& heightmap, std::vector<bestGroup::Vec3f>& normalsOut, int resolution, const char* filename){




	// necessary alpha variables
	float alpha0 = 1.0f;
	float height;
	float slope;

	float r, g, b = 0.0f;

	// create the new picture
	GEDUtils::SimpleImage image(resolution, resolution);

	// iterate through complete image
	for (int y = 0; y < resolution; y++)
	{
		for (int x = 0; x < resolution; x++)
		{
			height	= heightmap[IDX(x, y, resolution)];
			slope = normalsOut[IDX(x, y, resolution)].z;
			
			
			TextureBlending::calcAlphas(height, slope);

			// call getColorTiled 4-times per pixel to get the rgb's from the textures
			TextureBlending::blend(x, y, 0, r, g, b);


			// write rgb in the file
			image.setPixel(x, y, r, g, b);

		}
	}
	image.save(filename);
}


