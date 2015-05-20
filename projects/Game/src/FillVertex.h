#pragma once
#include <iostream>
#include "CustomData.h"
#include "2DAControl.h"


class FillVertex
{
public:
	FillVertex();
	~FillVertex();
	
	//copy paste
	static void generateNormals(const std::vector<float> &heightfield, int resolution, std::vector<bestGroup::Vec3f> &normalsOut);

	// takes the path of the heightmap and returns the height
	static int returnResolution(std::string path);

	// fills the vertex with the data from the heightmap image
	// int scale: every coordinate is multiplied with this int
	static void insertHeightfield(std::string path, std::vector<CustomData::SimpleVertex> &vertex);

	// fills the vertex with the normals
	// normals are calculated
	static void insertNormalmap(std::vector<CustomData::SimpleVertex> &vertex);
};

