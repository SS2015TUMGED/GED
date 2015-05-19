#include "FillVertex.h"
#include "SimpleImage.h"
#include "..//..//TerrainGenerator//2DAControl.h"
#include "..//..//TerrainGenerator//MyTextureGenerator.h"
#include <math.h>

FillVertex::FillVertex()
{
}


FillVertex::~FillVertex()
{
}

 int FillVertex::returnResolution(std::string path){
	
	GEDUtils::SimpleImage heightfield(path.c_str());
	return heightfield.getHeight;
}



static void FillVertexinsertHeightfield(std::string path, std::vector<CustomData::SimpleVertex> &vertex){

	// load heightmapimage with simpleimage
	GEDUtils::SimpleImage heightfield(path.c_str());

	int height = heightfield.getHeight;
	int width = heightfield.getWidth;

	CustomData::SimpleVertex tmp_vertex;

	// iterate through the image
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			// save the coordinates to the tmp_vertex
			tmp_vertex.Pos.x = (float) x;
			tmp_vertex.Pos.y = (float) y;
			tmp_vertex.Pos.z = (float) heightfield.getPixel(x, y);
			tmp_vertex.Pos.w = 1.0f;

			// put the tmp_vertex at position x,y in vertex
			vertex[IDX(x, y, width)];

		}
	}



}


static void insertNormalmap(std::string path, std::vector<CustomData::SimpleVertex> &vertex){
	
	// tmp vector to use the MyTextureGenerator::generateNormals method
	std::vector<float> tmp_height (vertex.size);

	// copy the floats from the vertex
	for (unsigned i = 0; i < vertex.size; i++)
	{
		tmp_height[i] = vertex[i].Pos.z;
	}

	// tmp normalsmap 
	std::vector<bestGroup::Vec3f> tmp_normals(vertex.size);

	// use  MyTextureGenerator::generateNormals to generate normals
	int resolution = (int) sqrtf(vertex.size);
	if ((resolution * resolution) != (int)vertex.size)
		throw "Deine Mutter";

	// generate the normals and write them into the tmp_normals
	MyTextureGenerator::generateNormals(tmp_height, resolution, tmp_normals);


	// copy the normals into the vertex
	for (int y = 0; y < resolution; y++)
	{
		for (int x = 0; x < resolution; x++)
		{
			vertex[IDX(x, y, resolution)].Normal.x = tmp_normals[IDX(x, y, resolution)].x;
			vertex[IDX(x, y, resolution)].Normal.y = tmp_normals[IDX(x, y, resolution)].y;
			vertex[IDX(x, y, resolution)].Normal.z = tmp_normals[IDX(x, y, resolution)].z;
			vertex[IDX(x, y, resolution)].Normal.w = 0.0f;
		}
	}
}




