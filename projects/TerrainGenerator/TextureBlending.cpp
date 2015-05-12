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

//returns true, if the slope is steep at this x,y coordinate
bool TextureBlending::isPointSteep(std::vector<bestGroup::Vec3f>& normalsOut, int resolution, int x, int y){
	return (normalsOut[IDX(x, y, resolution)].z < 0.8f);
}


//returns true, if the point at x,y in the highmap is high 
bool TextureBlending::isPointHight(std::vector<float>& highmap, int resolution, int x, int y){
	return (highmap[IDX(x, y, resolution)] < 0.5f);
}


// given in slide03
void TextureBlending::calcAlphas(float height, float slope, float& alpha1, float& alpha2, float& alpha3){
	alpha1 = (1 - height) * slope;
	alpha2 = height;
	alpha3 = height * slope;
}


float TextureBlending::getAlpha(int x, int y, int index){
	float alpha1;
	float alpha2;
	float alpha3;
	
	if (index == 1)
		return 
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

void TextureBlending::blend(std::vector<GEDUtils::SimpleImage> arr, int x, int y, int index, float& r, float& g, float& b){
	if (index >= arr.size){
		return;
	}

	if (index == 0){
		TextureBlending::getColorTiled(arr[0], x, y, r, g, b);
	}
	else if (index > 0){
		float alpha = 
	}

}



// load images
void TextureBlending::createImage(std::vector<float>& heightmap, std::vector<bestGroup::Vec3f>& normalsOut, int resolution){

	// load the textures into the ram
	GEDUtils::SimpleImage lowSteep("..\..\..\..\external\textures\ground02.jpg");
	GEDUtils::SimpleImage highSteep("..\..\..\..\external\textures\rock4.jpg");
	GEDUtils::SimpleImage highFlat("..\..\..\..\external\textures\Snow.jpg");
	GEDUtils::SimpleImage lowFlat("..\..\..\..\external\textures\gras15.jpg");

	// Array for the images
	TextureBlending::textures = { lowFlat, highFlat, highSteep, lowSteep };


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
			calcAlphas(height, slope, alpha1, alpha2, alpha3);

			// call getColorTiled 4-times per pixel to get the rgb's from the textures
			TextureBlending::getColorTiled


			// write rgb in the file
			image.setPixel(x, y, r, g, b);

		}
	}




	lowSteep.getPixel()


	// fuer jedes bild die farbe an den punkt x,y holen

	// blenden
		//

	// in neues bild speichern als color map commandline tool


}
