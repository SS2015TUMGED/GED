#pragma once
#include <iostream>
#include "CustomData.h"

class FillVertex
{
public:
	FillVertex();
	~FillVertex();


	// takes the path of the heightmap and returns the height
	static int returnResolution(std::string path);

	// fills the vertex with the data from the heightmap image
	// int scale: every coordinate is multiplied with this int
	static void insertHeightfield(std::string path, std::vector<CustomData::SimpleVertex> &vertex);

	// fills the vertex with the normals
	// at the moment is the path not necessary => normals are calculated
	static void insertNormalmap(std::string path, std::vector<CustomData::SimpleVertex> &vertex);
};

