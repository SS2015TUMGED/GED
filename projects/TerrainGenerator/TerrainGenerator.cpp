// TerrainGenerator.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <string>
#include <sstream>
#include <iostream>
#include "time.h"
#include <cstdlib>
#include <SimpleImage.h>
#include "DiamondSquare.h"
#include <random>
#include <time.h>
#include <TextureGenerator.h>
#include "MyTextureGenerator.h"
#include "TextureBlending.h"
#include "HeightfieldDownsizing.h"
#include "Smoothing.h"
#include "2DAControl.h"

//declare the random number generator
std::default_random_engine DiamondSquare::rng;

void printArray2D(std::vector<float> &array2D_, int width_, int height_){
	int ypos_;
	int xpos_;

	//getting the alignment right
	std::cout.precision(3);
	std::cout << std::fixed;

	for (ypos_ = 0; ypos_ < height_; ypos_++){ //iterating through "lines"
		for (xpos_ = 0; xpos_ < width_; xpos_++){ //iterating through "columns"

			float currentValue = array2D_[IDX(xpos_, ypos_, width_)];

			//writing to console
			std::cout << currentValue << " ";
		}
		std::cout << std::endl; //add linebreak after each "line"
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	using namespace std; 

#pragma region commandLineArguments
	//commandline Argument handling
	try{
		if (argc >= 8){ //check if the number of arguments is correct

			//creating one wstringstream object for every commandline parameter
			wstringstream cmdl_param_stream_1; // -r
			wstringstream cmdl_param_stream_2; // <resolution>
			wstringstream cmdl_param_stream_3; // -o_height
			wstringstream cmdl_param_stream_4; // <output heightfield filename>
			wstringstream cmdl_param_stream_5; // -o_color
			wstringstream cmdl_param_stream_6; // <output color filename>
			wstringstream cmdl_param_stream_7; // -o_normal
			wstringstream cmdl_param_stream_8; // <output normal filename>

			//cast each parameter to the desired form
			wstring param1; // -r
			cmdl_param_stream_1 << argv[1]; //arg to wstringstream
			cmdl_param_stream_1 >> param1; //wstringstream to wstring
			string param1_s(param1.begin(), param1.end()); //wstring to string
			if (param1_s != "-r"){
				throw "ERROR: Illegal Command Line Argument! (Param 1)";
			}


			int param2; // <resolution>
			cmdl_param_stream_2 << argv[2];
			cmdl_param_stream_2 >> param2;
			if (param2 <= 0){
				throw "ERROR: Illegal Command Line Argument! (Param 2)";
			}

			wstring param3; // -o_height
			cmdl_param_stream_3 << argv[3];
			cmdl_param_stream_3 >> param3;
			string param3_s(param3.begin(), param3.end()); //wstring to string
			if (param3_s != "-o_height"){
				throw "ERROR: Illegal Command Line Argument! (Param 3)";
			}

			wstring param4; // <output heightfield filename>
			cmdl_param_stream_4 << argv[4];
			cmdl_param_stream_4 >> param4;
			string param4_s(param4.begin(), param4.end()); //wstring to string

			wstring param5; // -o_color
			cmdl_param_stream_5 << argv[5];
			cmdl_param_stream_5 >> param5;
			string param5_s(param5.begin(), param5.end()); //wstring to string
			if (param5_s != "-o_color"){
				throw "ERROR: Illegal Command Line Argument! (Param 4)";
			}

			wstring param6; // <output color filename>
			cmdl_param_stream_6 << argv[6];
			cmdl_param_stream_6 >> param6;
			string param6_s(param6.begin(), param6.end()); //wstring to string

			wstring param7; // -o_normal
			cmdl_param_stream_7 << argv[7];
			cmdl_param_stream_7 >> param7;
			string param7_s(param7.begin(), param7.end()); //wstring to string
			if (param7_s != "-o_normal"){
				throw "ERROR: Illegal Command Line Argument! (Param 7)";
			}

			wstring param8; // <output normal filename>
			cmdl_param_stream_8 << argv[8];
			cmdl_param_stream_8 >> param8;
			string param8_s(param8.begin(), param8.end()); //wstring to string

			//printing out parameters
			cout << "1: " << param1_s << endl
				<< "2: " << param2 << endl
				<< "3: " << param3_s << endl
				<< "4: " << param4_s << endl
				<< "5: " << param5_s << endl
				<< "6: " << param6_s << endl
				<< "7: " << param7_s << endl
				<< "8: " << param8_s << endl;


#pragma endregion

#pragma region Random Number Generation
			int seed = time(NULL);
			cout << "seed: " << seed << endl << endl;

			//seed the rng
			DiamondSquare::rng.seed(seed);
#pragma endregion


#pragma region Vector creation and DiamondSquare
			//Defining variable for the Array
			static int width = param2;
			static int height = param2;

			//creating new array
			vector<float> *vec = new vector<float>((width + 1) * (height + 1));

			cout << endl << "Jetzt der DiamondSquare" << endl;
			DiamondSquare::diamondSquareAlgorithm(*vec, width + 1);

			//cutting the boundrys
			vec = DiamondSquare::CutBoundarys(*vec);

			//smoothing the vector

			//faster
			Smoothing::anotherSimpleSmoothing(*vec, width, 40, 12);
			Smoothing::squareSmoothing_nTimes(*vec, width, height, 50);

			//Slower, more realistic
			//Smoothing::circularSmoothing_nTimes(*vec, width, height, 5, 1);

			// create the normals vector
			std::vector<bestGroup::Vec3f> *normalsOut = new vector<bestGroup::Vec3f>(width * height);

			//run MyTextureGenerator
			MyTextureGenerator::generateNormals(*vec, width, *normalsOut);

			// create and save the image
			MyTextureGenerator::saveNormalsToImage(*normalsOut, width, param8_s.c_str());
			TextureBlending::createImage(*vec, *normalsOut, width, param6_s.c_str());

#pragma endregion
#pragma region Downsizing the heightmap
			// ********************* Downsize the heightmap ************************

			// new heightmap will be downscaled by scale * scale -> e.g.: 4096 * 4096 -> 1024 * 1024
			int scale = 4;

			int scaleWidth = width / scale;
			// 1. create new heightmap vector
			std::vector<float> *newHeightfield = new vector<float>(scaleWidth * scaleWidth);

			// 2. Downsize
			HeightfieldDownsizing::downsize(*vec, width, scale, *newHeightfield);

			// 3. Print controll grids (for debugging only)
			//HeightfieldDownsizing::printGrid(*vec, width, 8, 8, scale, *newHeightfield);
			//system("pause");

			//Saving downsized heightfield to file
			GEDUtils::SimpleImage image(scaleWidth, scaleWidth);

			for (int ypos = 0; ypos < scaleWidth; ypos++){
				for (int xpos = 0; xpos < scaleWidth; xpos++){
					image.setPixel(xpos, ypos, (const float)(*newHeightfield)[IDX(xpos, ypos, scaleWidth)]);
				}
			}
			image.save(param4_s.c_str());
			std::cout << "downsized image saved..." << std::endl;

#pragma endregion
#pragma endregion

			//free memory
			delete newHeightfield;
			delete vec;
			delete normalsOut;
		}
		else{
			throw "ERROR: Illegal number of Arguments!";
		}
	}
	catch (char* error){
		cout << error << endl;
	}
	return 0;
}