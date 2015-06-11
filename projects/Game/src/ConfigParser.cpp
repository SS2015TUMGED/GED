#include "ConfigParser.h"
#include <string>
#include <fstream>
#include <cstdlib>
#include <sstream>


std::string ConfigParser::height;
std::string ConfigParser::color;
std::string ConfigParser::normal;
std::string ConfigParser::mesh_indentifier, ConfigParser::mesh_diffuse, 
ConfigParser::mesh_specular, ConfigParser::mesh_glow, ConfigParser::mesh_texture;
bool ConfigParser::terrainSpinning;


ConfigParser::ConfigParser()
{
}
ConfigParser::~ConfigParser()
{
}

void ConfigParser::load(std::string str){

	using namespace std;
	
	// to read the values  
	int skip = 0;

	// stream to read file
	ifstream ifs(str);


	string tmp_terrainPath;

	// check if file is open
	if (ifs.is_open()){
		cout << "file is open" << endl;
		
		// string for line
		string line;
		
		// string for the first word in the line
		string word;

		string dir;
#ifdef _DEBUG
		// program is in the Debug configuration
		dir = "..\\..\\Debug";
#else
		// program is in the Release configuration
		dir = "..\\..\\Release";
#endif

		int i = 0;

		// loop line by line
		while (getline(ifs, line)){
			//std::cout << line << std::endl;


			//get the components in the line
			istringstream iss(line);
			iss >> word;

			if (word.compare("spinning") == 0){
				iss >> spinning;
				//std::cout << "spinning " << spinning << std::endl;
			}

			else if (word.compare("spinSpeed") == 0){
				iss >> spinSpeed;
				//std::cout << "spinSpeed " << spinSpeed << std::endl;
			}
			else if (word.compare("backgroundColor") == 0){
				iss >> backgroundColor.r >> backgroundColor.g >> backgroundColor.b;
				//std::cout << color.r << " " << color.g << " " << color.b << std::endl;
			}
			else if (word.compare("TerrainPath") == 0){
				iss >> height >> color >> normal;
				height = dir + height;
				color = dir + color;
				normal = dir + normal;
			}
			else if (word.compare("TerrainDepth") == 0){
				iss >> terrainDepth;
			}
			else if (word.compare("TerrainWidth") == 0){
				iss >> terrainWidth;
			}
			else if (word.compare("TerrainHeight") == 0){
				iss >> terrainHeight;
			}
			else if (word.compare("TerrainSpinning") == 0){
				string tmp;
				iss >> tmp;
				terrainSpinning = (tmp.compare("1") == 0);
			}
			else if (word.compare("Mesh") == 0){
				iss >> mesh_indentifier >> mesh_texture >> mesh_diffuse >> mesh_specular >> mesh_glow;
				mesh_indentifier = dir + mesh_indentifier;
				mesh_texture = dir + mesh_texture;
				mesh_diffuse = dir + mesh_diffuse;
				mesh_specular = dir + mesh_specular;
				mesh_glow = dir + mesh_glow;
			}
			else {
				if (!word.empty())
				cout << "Error: unknown Parameter!" << endl;
			}
			//empty word
			word = "";
			//system("pause");
		}
	}
	ifs.close();
	if (!ifs.is_open())
	{
		cout << "file closed...OK" << endl << endl;
	}
	else
		cout << "Error: file not closed!" << endl;
}


float ConfigParser::getSpinning(){
	return spinning;
}
float ConfigParser::getSpinSpeed(){
	return spinSpeed;
}
float ConfigParser::getTerrainWidth(){
	return terrainWidth;
}
float ConfigParser::getTerrainDepth(){
	return terrainDepth;
}
float ConfigParser::getTerrainHeight(){
	return terrainHeight;
}
ConfigParser::Color ConfigParser::getBackgroundColor(){
	return backgroundColor;
}

