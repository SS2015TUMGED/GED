#include "FillVertex.h"
#include "SimpleImage.h"
#include "2DAControl.h"
#include "MyTextureGenerator.h"
#include <math.h>
#include "2DAControl.h"
#include "ConfigParser.h"
#include "CustomData.h"

FillVertex::FillVertex()
{
}


FillVertex::~FillVertex()
{
}

 int FillVertex::returnResolution(std::string path){
	
	GEDUtils::SimpleImage heightfield(path.c_str());
	return heightfield.getHeight();
}

 void FillVertex::generateNormals(const std::vector<float> &heightfield, int resolution,
	 std::vector<bestGroup::Vec3f> &normalsOut)
 {

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
			 float x1 = 0.0f;						// normal x1 direction
			 float x2 = 0.0f;						// normal x2 direction
			 float x3 = 1.0f / (resolution / 2.0f);	// normal x3 direction
			 float length = 0.0f;					// normal length before normalizing


			 // check if col exists and handle the borders/corners
			 if (x > 0 && x < resolution - 1)
				 x1 = (heightfield[IDX(x + 1, y, resolution)] - heightfield[IDX(x - 1, y, resolution)]) / 2.0f;
			 else if (x == 0)
				 x1 = (heightfield[IDX(x + 1, y, resolution)] - heightfield[IDX(x, y, resolution)]);
			 else if (x == resolution - 1)
				 x1 = (heightfield[IDX(x - 1, y, resolution)] - heightfield[IDX(x, y, resolution)]);

			 // check if rows exists & handle the corners/borders
			 if (y > 0 && y < resolution - 1)
				 x2 = (heightfield[IDX(x, y + 1, resolution)] - heightfield[IDX(x, y - 1, resolution)]) / 2.0f;
			 else if (y == 0)
				 x2 = (heightfield[IDX(x, y + 1, resolution)] - heightfield[IDX(x, y, resolution)]);
			 else if (y == resolution - 1)
				 x2 = (heightfield[IDX(x, y - 1, resolution)] - heightfield[IDX(x, y, resolution)]);

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
			 normalsOut[IDX(x, y, resolution)].y = x3;
			 normalsOut[IDX(x, y, resolution)].z = x2;
		 }
	 }
 }

 void FillVertex::insertHeightfield(std::string path, std::vector<CustomData::SimpleVertex> &vertex){

	// load heightmapimage with simpleimage
	GEDUtils::SimpleImage heightfield(path.c_str());

	int depth = heightfield.getHeight();
	int width = heightfield.getWidth();

	CustomData::SimpleVertex tmp_vertex;

	// iterate through the image
	for (int y = 0; y < depth; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// save the coordinates to the tmp_vertex
			
			// UV data
			tmp_vertex.UV.x = (float) x / (width - 1);
			tmp_vertex.UV.y = (float) y / (depth - 1);

			// Vertex Position
			// intervall: 
			//Pos.x [-width/2 ; width/s]
			//Pos.y [0 ; height ]
			//Pos.z [-depth / 2 ; depth/ 2 ]
			tmp_vertex.Pos.x = (float) (x - width / 2) / width * parser.getTerrainWidth();
			tmp_vertex.Pos.y = (float) heightfield.getPixel(x, y) * parser.getTerrainHeight();
			tmp_vertex.Pos.z = (float) (y - depth / 2) /depth * parser.getTerrainDepth();
			tmp_vertex.Pos.w = 1.0f;

			// put the tmp_vertex at position x,y in vertex
			vertex[IDX(x, y, width)] = tmp_vertex;

		}
	}



}


void FillVertex::insertNormalmap(std::vector<CustomData::SimpleVertex> &vertex){
	// load the heightmap
	GEDUtils::SimpleImage heightfield(ConfigParser::height.c_str());

	// get the resolution
	int width = heightfield.getWidth();
	int depth = heightfield.getHeight();


	// tmp vector to use the MyTextureGenerator::generateNormals method
	std::vector<float> tmp_height (width * depth);

	// copy the height values from the image into the vector
	for (int y = 0; y < depth; y++)
	{
		for (int x = 0; x < width; x++)
		{
			tmp_height[IDX(x, y, width)] = heightfield.getPixel(x, y);
		}
	}

	// tmp normalsmap 
	std::vector<bestGroup::Vec3f> tmp_normals(width * depth);

	// use  MyTextureGenerator::generateNormals to generate normals
	int resolution = width;
	
	// generate the normals and write them into the tmp_normals
	FillVertex::generateNormals(tmp_height, resolution, tmp_normals);


	// copy the normals into the vertex
	for (int y = 0; y < resolution; y++)
	{
		for (int x = 0; x < resolution; x++)
		{

			// safe the normals into the vertex, and scale them to fit the resolution
			vertex[IDX(x, y, resolution)].Normal.x = (float)tmp_normals[IDX(x, y, resolution)].x;
			vertex[IDX(x, y, resolution)].Normal.y = (float)tmp_normals[IDX(x, y, resolution)].y;
			vertex[IDX(x, y, resolution)].Normal.z = (float)tmp_normals[IDX(x, y, resolution)].z;
			vertex[IDX(x, y, resolution)].Normal.w = 0.0f;

			/*
			vertex[IDX(x, y, resolution)].Normal.x = 0.0f;
			vertex[IDX(x, y, resolution)].Normal.y = 1.0f;
			vertex[IDX(x, y, resolution)].Normal.z = 0.0f;
			vertex[IDX(x, y, resolution)].Normal.w = 0.0f;
			*/

		}
	}
}




