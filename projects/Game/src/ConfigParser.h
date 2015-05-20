#pragma once
#include <iostream>
#include <set>

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


	
	// getters
	float getSpinning();
	float getSpinSpeed();
	float getTerrainWidth();
	float getTerrainDepth();
	float getTerrainHeight();
	Color getBackgroundColor();

	void load(std::string str);

	ConfigParser();
	~ConfigParser();


private:

	void getPathes(std::string path);

	float spinning, spinSpeed,
			terrainWidth, terrainDepth, terrainHeight;

	Color backgroundColor;

	

};

