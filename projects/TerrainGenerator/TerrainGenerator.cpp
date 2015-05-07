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

// TestMoreShit
// Access a 2D array of width w at position x / y 
#define IDX(xpos, ypos, width) ((xpos) + (ypos) * (width))

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

void smoothArray2D(std::vector<float> &array2D_, int width_, int height_){
	//create new, temporal array
	std::vector<float> array2D = std::vector<float>(width_ * height_);

	//get the 4 corners right
#pragma region corners
	//top left
	array2D[0] = (
		array2D_[0] + //corner piece
		array2D_[1] + //right of the corner
		array2D_[width_] + //below the corner
		array2D_[width_ + 1] //bottom right
		) / 4.0f;

	//top right
	array2D[width_ - 1] = (
		array2D_[width_ - 1] + //corner piece
		array2D_[width_ - 2] + //left of the corner
		array2D_[width_ + width_ - 1] + //below the corner
		array2D_[width_ + width_ - 2] //bottom left
		) / 4.0f;

	//bottom left
	array2D[IDX(0, height_ - 1, width_)] = (
		array2D_[IDX(0, height_ - 1, width_)] + //corner piece
		array2D_[IDX(0, height_ - 2, width_)] + //top of the corner
		array2D_[IDX(1, height_ - 1, width_)] + //right of the corner
		array2D_[IDX(1, height_ - 2, width_)] //top right
		) / 4.0f;

	//bottom right
	array2D[IDX(width_ - 1, height_ - 1, width_)] = (
		array2D_[IDX(width_ - 1, height_ - 1, width_)] + //corner piece
		array2D_[IDX(width_ - 1, height_ - 2, width_)] + //top of the corner
		array2D_[IDX(width_ - 2, height_ - 1, width_)] + //left of the corner
		array2D_[IDX(width_ - 2, height_ - 2, width_)] //top left
		) / 4.0f;

	//get the edges right
#pragma endregion
#pragma region edges
	int i;
	for (i = 1; i < width_ - 1; i++){
		//top edge
		array2D[i] = (
			array2D_[i] + //piece
			array2D_[i - 1] + //left of piece
			array2D_[i + 1] + //right of piece
			array2D_[i + width_] + //below piece
			array2D_[i + width_ - 1] + //bottom left
			array2D_[i + width_ + 1] //bottom right
			) / 6.0f;
		//bottom edge
		array2D[IDX(i, height_ - 1, width_)] = (
			array2D_[IDX(i, height_ - 1, width_)] + //piece
			array2D_[IDX(i - 1, height_ - 1, width_)] + //left of piece
			array2D_[IDX(i + 1, height_ - 1, width_)] + //right of piece
			array2D_[IDX(i, height_ - 2, width_)] + //top of piece
			array2D_[IDX(i - 1, height_ - 2, width_)] + //top left
			array2D_[IDX(i + 1, height_ - 2, width_)] //top right
			) / 6.0f;
		//left edge
		array2D[IDX(0, i, width_)] = (
			array2D_[IDX(0, i, width_)] + //piece
			array2D_[IDX(0, i - 1, width_)] + //top of piece
			array2D_[IDX(0, i + 1, width_)] + //below piece
			array2D_[IDX(1, i, width_)] + //right of piece
			array2D_[IDX(1, i - 1, width_)] + //Top right
			array2D_[IDX(1, i + 1, width_)] //Bottom right
			) / 6.0f;
		//right edge
		array2D[IDX(width_ - 1, i, width_)] = (
			array2D_[IDX(width_ - 1, i, width_)] + //piece
			array2D_[IDX(width_ - 1, i - 1, width_)] + //top of piece
			array2D_[IDX(width_ - 1, i + 1, width_)] + //below of piece
			array2D_[IDX(width_ - 2, i, width_)] + //left of piece
			array2D_[IDX(width_ - 2, i - 1, width_)] + //Top left
			array2D_[IDX(width_ - 2, i + 1, width_)] //Bottom left
			) / 6.0f;
	}
#pragma endregion

	//get the rectangle in the middle
	int xpos, ypos;
	for (ypos = 1; ypos < height_ - 1; ypos++){ //"lines"
		int startOfLinePos = IDX(0, ypos, width_); //position of the first  piece of the "line"
		for (xpos = 1; xpos < width_ - 1; xpos++){ //"columns"
			array2D[startOfLinePos + xpos] = (
				array2D_[startOfLinePos + xpos] + //piece
				array2D_[startOfLinePos + xpos - width_] + //top of piece
				array2D_[startOfLinePos + xpos + width_] + //below piece
				array2D_[startOfLinePos + xpos - 1] + //left of piece
				array2D_[startOfLinePos + xpos + 1] + //right of piece
				array2D_[startOfLinePos + xpos - width_ - 1] + //top right 
				array2D_[startOfLinePos + xpos + width_ - 1] + //bottom right
				array2D_[startOfLinePos + xpos - width_ + 1] + //top left
				array2D_[startOfLinePos + xpos + width_ + 1]  //bottom left
				) / 9.0f;
		}
	}

	//copy array2D to array2D_
	for (ypos = 0; ypos < height_; ypos++){
		for (xpos = 0; xpos < width_; xpos++){
			array2D_[IDX(xpos, ypos, width_)] =
				array2D[IDX(xpos, ypos, width_)];
		}
	}
	
}

void smoothArray2D_nTimes(std::vector<float> &array2D_, int width_, int height_, int n){
	using namespace std;
	int g;
	for (g = 0; g < n; g++){
		cout << endl;
		cout << "Smoothing " << g << "/" << n;
		smoothArray2D(array2D_, width_, height_);
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
			unsigned int seed = time(NULL);
	cout << "seed: " << seed << endl << endl;
	
	//seed the rng
	DiamondSquare::rng.seed(seed);
#pragma endregion


#pragma region Array Creation and Smoothing
			//Defining variable for the Array
			static int width = param2;
			static int height = param2;

			/*
			//creating new array
			float* array2D = new float[width * height]();

			//initialising random number generator
			srand(time(NULL)); //seed the rng

			//filling the array with random floats between 0.0f and 1.0f
			int ypos;
			int xpos;
			for (ypos = 0; ypos < height; ypos++){
				for (xpos = 0; xpos < width; xpos++){
					array2D[IDX(xpos, ypos, width)] =
						(rand() % 100 + 1) / 100.0f;
				}
			}
			
			//print array to console
			printArray2D(array2D, width, height);
			//smoothing array
			smoothArray2D_nTimes(array2D, width, height, 10);
			//printing (again)
			std::cout << std::endl << std::endl;
			printArray2D(array2D, width, height);

			system("pause");
			*/
			
			//creating new array
			vector<float> *vec = new vector<float>((width + 1) * (width + 1));
			
			cout << endl << "Jetzt der DiamondSquare" << endl;
			DiamondSquare::diamondSquareAlgorithm(*vec, width + 1);

			cout << "Smoothing..." << endl;

			smoothArray2D_nTimes(*vec, width + 1, height + 1, 400);

			//cutting the boundrys
			vec = DiamondSquare::CutBoundarys(*vec);
		
			smoothArray2D_nTimes(*vec, width , height , 1);
			


			//Saving array to heightfield
			GEDUtils::SimpleImage image(width, height);
			
			for (float ypos = 0; ypos < height; ypos++){
				for (float xpos = 0; xpos < width; xpos++){
					image.setPixel(xpos, ypos, (const float) (*vec)[IDX(xpos, ypos, width)]);
				}
			}

			image.save(param4_s.c_str());
			/*
			wstring path = wstring("");

			
			GEDUtils::TextureGenerator texGen();
			texGen::generateAndStoreImages(&vec, width, param5_s, param7_s);
			*/

#pragma endregion

			//free memory
			delete vec;
			
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