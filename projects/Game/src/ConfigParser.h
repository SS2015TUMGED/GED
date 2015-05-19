#pragma once
#include <iostream>
#include <set>

class ConfigParser
{
public:
	struct Color {
		float r,g,b;
	};
	
	// getters
	float getSpinning();
	float getSpinSpeed();
	float getTerrainWidth();
	float getTerrainDepth();
	float getTerrainHeight();
	Color getBackgroundColor();
	std::string getTerrainPath();

	void load(std::string str);

	ConfigParser();
	~ConfigParser();


private:
	float spinning, spinSpeed,
			terrainWidth, terrainDepth, terrainHeight;

	Color backgroundColor;

	std::string terrainPath;

};

