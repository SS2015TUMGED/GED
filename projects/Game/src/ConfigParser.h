#pragma once
#include <iostream>
#include <set>
#include <map>
#include "Mesh.h"

class ConfigParser
{
public:
	struct Color {
		float r,g,b;
	};

	/*struct terrainPath {
		static std::string height;
		static std::string color;
		static std::string normal;
	};*/

	static std::string height;
	static std::string color;
	static std::string normal;
	static bool terrainSpinning;
	static std::map<std::string, Mesh*> g_Meshes;

	
	// getters
	float getSpinning();
	float getSpinSpeed();
	float getTerrainWidth();
	float getTerrainDepth();
	float getTerrainHeight();
	Color getBackgroundColor();
	void getPathes(std::string path);
	
	void load(std::string str);

	ConfigParser();
	~ConfigParser();


private:

	

	float spinning, spinSpeed,
			terrainWidth, terrainDepth, terrainHeight;

	Color backgroundColor;

	

};

