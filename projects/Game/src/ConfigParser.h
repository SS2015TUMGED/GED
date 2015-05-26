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
	static std::string mesh_indentifier, mesh_diffuse, mesh_specular, mesh_glow, mesh_texture;



	
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

