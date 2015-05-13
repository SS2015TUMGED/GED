#include "TextureBlending.h"
#include <vector>
#include "MyTextureGenerator.h"
#include "SimpleImage.h"
#include "2DAControl.h"

// load the textures into the ram
GEDUtils::SimpleImage lowSteep("..\\..\\..\\..\\external\\textures\\ground02.jpg");
GEDUtils::SimpleImage highSteep("..\\..\\..\\..\\external\\textures\\rock4.jpg");
GEDUtils::SimpleImage highFlat("..\\..\\..\\..\\external\\textures\\rock3.jpg");
GEDUtils::SimpleImage lowFlat("..\\..\\..\\..\\external\\textures\\gras15.jpg");
GEDUtils::SimpleImage snow("..\\TerrainGenerator\\Snow.jpg");

// Array for the images
std::vector<float> TextureBlending::alphas = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
std::vector<GEDUtils::SimpleImage> TextureBlending::textures = { lowSteep,lowFlat, highFlat, highSteep, snow };

// given in slide03
void TextureBlending::calcAlphas(float height, float slope){
	slope *= slope;
	TextureBlending::alphas[0] = 1.0f;
	TextureBlending::alphas[1] = (1 - height) * slope + 0.1f;
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
	
		alphas[4] = height*slope*height*1.8f;
	
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

void TextureBlending::blend(int x, int y, unsigned index, float& r, float& g, float& b){
	
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
			height = heightmap[IDX(x, y, resolution)];
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


