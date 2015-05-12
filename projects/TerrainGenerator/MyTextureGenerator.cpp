#include "MyTextureGenerator.h"
#include <math.h> 
#include <SimpleImage.h>
#include <iostream>


// Access a 2D array of width w at position x / y 
#define IDX(xpos, ypos, width) ((xpos) + (ypos) * (width))

MyTextureGenerator::MyTextureGenerator()
{
}

MyTextureGenerator::~MyTextureGenerator()
{
}

// takes the x1, x2 coordinates of a flatvector and returns the length of the vector
float caluclateVectorLength(float x1, float x2);



void MyTextureGenerator::generateNormals(const std::vector<float> &heightfield, int resolution,
	std::vector<bestGroup::Vec3f> &normalsOut){

	// documentation 
	// http://acko.net/blog/making-worlds-3-thats-no-moon/


	//  iterate through the rows
	for (int y = 0; y < resolution; y++)
	{
		//iterate through the cols
		for (int x = 0; x < resolution; x++)
		{
			//normal mapping

			//necessary variables
			float x1 = 0.0f;	// normal x1 direction
			float x2 = 0.0f;	// normal x2 direction
			float x3 = 1.0f / 512.0f;	// normal x3 direction
			float length = 0.0f;// normal length before normalizing



			// check if col exists and handle the borders/corners
				if ( x > 0 && x < resolution - 1)
					 x1 = (heightfield[IDX(x + 1, y, resolution)] - heightfield[IDX(x - 1, y, resolution)]) / 2.0f;
				else if (x == 0)
					 x1 = (heightfield[IDX(x + 1, y, resolution)] - heightfield[IDX(x  , y, resolution)]) ;
				else if (x == resolution - 1)
					 x1 = (heightfield[IDX(x - 1, y, resolution)] - heightfield[IDX(x , y, resolution)]) ;
			
				// check if rows exists & handle the corners/borders
				if (y > 0 && y < resolution - 1)
					 x2 = (heightfield[IDX(x, y + 1, resolution)] - heightfield[IDX(x, y - 1, resolution)]) / 2.0f;
				else if (y == 0)
					 x2 = (heightfield[IDX(x, y + 1, resolution)] - heightfield[IDX(x, y, resolution)]);
				else if (y == resolution -1) 
					 x2 = (heightfield[IDX(x, y - 1, resolution)] - heightfield[IDX(x, y , resolution)]) ;


				

				

				x1 *= -1;
				x2 *= -1;

				
				// normalize
				length = sqrtf(x1*x1 + x2*x2 + x3*x3);

				// normalize vector
				x1 = x1 / length;
				x2 = x2 / length;
				x3 = x3 / length;

				// Add the normal Vector to normalsOut
				normalsOut[IDX(x, y, resolution)].x = x1;
				normalsOut[IDX(x, y, resolution)].y = x2;
				normalsOut[IDX(x, y, resolution)].z = x3;
		}
	}
}






void MyTextureGenerator::saveNormalsToImage(const std::vector<bestGroup::Vec3f>& normalsOut, int resolution, const char* filename){

	
	
	GEDUtils::SimpleImage image(resolution, resolution);

	for (int y = 0; y < resolution; y++)
	{
		for (int x = 0; x < resolution; x++)
		{
			// get the floats and change the range from [-1;1] to [0;1] 

			float r  = normalsOut[IDX(x, y, resolution)].x;
			r = (r + 1) / 2;

			float g = normalsOut[IDX(x, y, resolution)].y;
			g = (g + 1) / 2;

			float b = normalsOut[IDX(x, y, resolution)].z;
			b = (b + 1) / 2;

			


			// save floats as rgb color to the image
			image.setPixel(x, y, r, g, b);

		}
	}

	// export the image
	image.save(filename);

}



