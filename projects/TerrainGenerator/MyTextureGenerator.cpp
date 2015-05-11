#include "MyTextureGenerator.h"
#include <math.h> 




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



void MyTextureGenerator::generateNormals(const std::vector<float>& heightfield, int resolution,
	std::vector<float>& normalsOut){

	// documentation 
	// http://acko.net/blog/making-worlds-3-thats-no-moon/


	//  iterate through the rows
	for (size_t y = 0; y < resolution; y++)
	{
		//iterate through the cols
		for (size_t x = 0; x < resolution; x++)
		{
			//normal mapping

			//necessary variables
			float x1 = 0.0f;	// normal x1 direction
			float x2 = 0.0f;	// normal x2 direction
			float x3 = 1.0f;	// normal x3 direction
			float length = 0.0f;// normal length before normalizing
			// height of the position in the map
			float height = heightfield[IDX(x, y, resolution)];


			// check if col exists and handle the borders/corners
				if ( x > 0 && x < resolution - 1)
					 x1 = (heightfield[IDX(x + 1, y, resolution)] - heightfield[IDX(x - 1, y, resolution)]) / 2;
				else if (x == 0)
					 x1 = (heightfield[IDX(x + 2, y, resolution)] - heightfield[IDX(x + 1, y, resolution)]) / 2;
				else if (x == resolution - 1)
					 x1 = (heightfield[IDX(x - 2, y, resolution)] - heightfield[IDX(x - 1, y, resolution)]) / 2;
			
				// check if rows exists & handle the corners/borders
				if (y > 0 && y < resolution - 1)
					 x2 = (heightfield[IDX(x, y + 1, resolution)] - heightfield[IDX(x, y - 1, resolution)]) / 2;
				else if (y == 0)
					 x2 = (heightfield[IDX(x, y + 2, resolution)] - heightfield[IDX(x, y + 1, resolution)]) / 2;
				else if (y == resolution -1) 
					 x2 = (heightfield[IDX(x, y - 2, resolution)] - heightfield[IDX(x, y - 1, resolution)]) / 2;

				// get the length of the flatvector just created
				length = caluclateVectorLength(x1, x2);

				// normalize vector
				x1 = x1 / length;
				x2 = x2 / length;
				x3 = 1 / length;

				// multiply with the height
				x1 *= height;
				x2 *= height;
				x3 *= height;



		}
	}


}



float caluclateVectorLength(float x1, float x2){
	return sqrtf(x1*x1 + x2*x2 + 1);
}


