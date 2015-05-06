#include "DiamondSquare.h"
#include <iostream>
#include <chrono>
#include <math.h>

// Access a 2D array of width w at position x / y 
#define IDX(xpos, ypos, width) ((xpos) + (ypos) * (width))

unsigned int DiamondSquare::resolution;

/* Uses the Diamond Square Algorithm on the given vector<float>.
* @param:
* vector<float> vec	:= the vector on which the algorithm is executed
*/
void DiamondSquare::diamondSquareAlgorithm(std::vector<float> &vec, const unsigned int res)
{
	resolution = res;

	//assign random values to the 4 corners
	vec[0] = normalDisRandom(0.0f, 1.0f);
	vec[(resolution - 1) * (resolution - 1)] = normalDisRandom(0.0f, 1.0f);
	vec[resolution - 1] = normalDisRandom(0.0f, 1.0f);
	vec[IDX(resolution - 1, 0, resolution)] = normalDisRandom(0.0f, 1.0f);

	for (int i = 0; i < log2(resolution - 1); i++)
	{
		std::cout << std::endl << "Jetzt Diamond" << std::endl;
		system("pause");
		diamondStep(vec);
		std::cout << std::endl << "Jetzt Square" << std::endl;
		system("pause");
		squareStep(vec);
	}
};

void DiamondSquare::diamondStep(std::vector<float>& v)
{
	//counting the iteration steps
	static unsigned int numberOfIterations = 1;
	//How many SingleSteps need to be done by line
	int SingleStepCount = pow(2, numberOfIterations - 1);

	for (int i = 0; i < SingleStepCount; i++) //column
	{
		for (int k = 0; k < SingleStepCount; k++) //line
		{
			diamondStepSingle(
				v,
				k*((resolution - 1) / SingleStepCount),
				i*((resolution - 1) / SingleStepCount),
				numberOfIterations);
		}
	}
	numberOfIterations++;
};

void DiamondSquare::diamondStepSingle(std::vector<float>& v, int startx, int starty, unsigned int iteration)
{
	//Accessing the boarders and writing the average value in the middle
	v[IDX(startx + (resolution - 1) / iteration / 2,
			starty + (resolution - 1) / iteration / 2,
			resolution)] =
		(v[IDX(startx, starty, resolution)] + 
		v[IDX(startx + (resolution - 1) / iteration, starty, resolution)] +
		v[IDX(startx, starty + (resolution - 1) / iteration, resolution)] + 
		v[IDX(startx + (resolution - 1) / iteration,
				starty + (resolution - 1) / iteration,
				resolution)]) / 4.0f;
};

/* Returns normally distributed random values between min and max.
* @param:
* float min	:= the mininmal number (included)
* float max	:= the maximal number (included)
*/
float DiamondSquare::normalDisRandom(float min, float max){

	float currentValue; //the currently generated Value
	float midpoint;
	float offset;

	//offset between midpoint and max (or min)
	offset = (max - min) / 2.0f;

	//middle between min and max
	midpoint = min + offset; 

	//distribute around midpoint midpoint with offset offset
	// -> generates (mostly) numbers between min and max
	std::normal_distribution<float> rng_dis(midpoint, offset);
	do{
		currentValue = rng_dis(rng);
	} while (currentValue <= min || currentValue >= max);

	return currentValue;
};

//SquareStep of the algorithm through the whole array
void DiamondSquare::squareStep(std::vector<float> &vec){
	
	//counting the iteration steps
	static unsigned int numberOfIterations = 0;
	numberOfIterations ++;
	
	unsigned int midpointOffset = ((resolution - 1)/ numberOfIterations) /2;

	//iterating through the array

	//iterate through 'Lines of Squares' eg 1 in the first iteration
	//two in the second iteration etc.
	for (unsigned currentLine = 1; 
		currentLine <= (pow(2,numberOfIterations - 1)); 
		currentLine++)
	{
		//iterate through the 'Columns of Squares' eg 1 in the first iteration etc
		//but always according to the midpoint
		for (unsigned currentColumn = 1; 
			currentColumn <= (pow(2, numberOfIterations - 1));
			currentColumn++)
		{	
			//setting the midpoint of the Square to the correct position
			unsigned currentMidpointPosition =
				IDX(midpointOffset * currentColumn, midpointOffset * currentLine, resolution);

			if (numberOfIterations == 1){

				//left point
				vec[currentMidpointPosition - midpointOffset] =
					(vec[currentMidpointPosition] + //midpoint 
					vec[currentMidpointPosition - midpointOffset - resolution] + //top
					vec[currentMidpointPosition - midpointOffset + resolution] //bottom
					) / 3.0f;

				//right point
				vec[currentMidpointPosition + midpointOffset] =
					(vec[currentMidpointPosition] + //midpoint
					vec[currentMidpointPosition + midpointOffset - resolution] + //top
					vec[currentMidpointPosition + midpointOffset + resolution] //bottom
					) / 3.0f;

				//top point
				vec[currentMidpointPosition - resolution] =
					(vec[currentMidpointPosition] + //midpoint
					vec[currentMidpointPosition - resolution - midpointOffset] + //left
					vec[currentMidpointPosition - resolution + midpointOffset] //right
					) / 3.0f;

				//bottom point
				vec[currentMidpointPosition + resolution] =
					(vec[currentMidpointPosition] + //midpoint
					vec[currentMidpointPosition + resolution - midpointOffset] + //left
					vec[currentMidpointPosition + resolution + midpointOffset] //right
					) / 3.0f;

			}
			else {
				//checking for special cases (borders)
				if (currentColumn == 1){
#pragma region First Column
					if (currentLine == 1){
#pragma region Top Left Corner

						//left point
						vec[currentMidpointPosition - midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - midpointOffset - resolution] + //top
							vec[currentMidpointPosition - midpointOffset + resolution] //bottom
							) / 3.0f;

						//right point
						vec[currentMidpointPosition + midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + midpointOffset + midpointOffset] + //right
							vec[currentMidpointPosition + midpointOffset - resolution] + //top
							vec[currentMidpointPosition + midpointOffset + resolution] //bottom
							) / 4.0f;

						//top point
						vec[currentMidpointPosition - resolution] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - resolution - midpointOffset] + //left
							vec[currentMidpointPosition - resolution + midpointOffset] //right
							) / 3.0f;

						//bottom point
						vec[currentMidpointPosition + resolution] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + resolution - midpointOffset] + //left
							vec[currentMidpointPosition + resolution + midpointOffset] + //right
							vec[currentMidpointPosition + resolution + resolution] //bottom
							) / 4.0f;
#pragma endregion
					}
					else if (currentLine = 2 ^ (numberOfIterations - 1)){ //bottom left corner
#pragma region Bottom Left Corner	
						//left point
						vec[currentMidpointPosition - midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - midpointOffset - resolution] + //top
							vec[currentMidpointPosition - midpointOffset + resolution] //bottom
							) / 3.0f;

						//right point
						vec[currentMidpointPosition + midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + midpointOffset + midpointOffset] + //right
							vec[currentMidpointPosition + midpointOffset - resolution] + //top
							vec[currentMidpointPosition + midpointOffset + resolution] //bottom
							) / 4.0f;

						//top point
						vec[currentMidpointPosition - resolution] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - resolution - midpointOffset] + //left
							vec[currentMidpointPosition - resolution + midpointOffset] + //right
							vec[currentMidpointPosition - resolution - resolution]  //top
							) / 4.0f;

						//bottom point
						vec[currentMidpointPosition + resolution] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + resolution - midpointOffset] + //left
							vec[currentMidpointPosition + resolution + midpointOffset] //right
							) / 3.0f;
#pragma endregion
					}
					else{
#pragma region Left Edge
						//left point
						vec[currentMidpointPosition - midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - midpointOffset - resolution] + //top
							vec[currentMidpointPosition - midpointOffset + resolution] //bottom
							) / 3.0f;

						//right point
						vec[currentMidpointPosition + midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + midpointOffset + midpointOffset] + //right
							vec[currentMidpointPosition + midpointOffset - resolution] + //top
							vec[currentMidpointPosition + midpointOffset + resolution] //bottom
							) / 4.0f;

						//top point
						vec[currentMidpointPosition - resolution] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - resolution - midpointOffset] + //left
							vec[currentMidpointPosition - resolution + midpointOffset] + //right
							vec[currentMidpointPosition - resolution - resolution]  //top
							) / 4.0f;

						//bottom point
						vec[currentMidpointPosition + resolution] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + resolution - midpointOffset] + //left
							vec[currentMidpointPosition + resolution + midpointOffset] + //right
							vec[currentMidpointPosition + resolution + resolution] //bottom
							) / 4.0f;
#pragma endregion
					}
#pragma endregion
				}
				else if (currentColumn = 2 ^ (numberOfIterations - 1)){
#pragma region Last Column
					if (currentLine = 1){
#pragma region Top Right Corner

						//left point
						vec[currentMidpointPosition - midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint 
							vec[currentMidpointPosition - midpointOffset - midpointOffset] + //left
							vec[currentMidpointPosition - midpointOffset - resolution] + //top
							vec[currentMidpointPosition - midpointOffset + resolution] //bottom
							) / 4.0f;

						//right point
						vec[currentMidpointPosition + midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + midpointOffset - resolution] + //top
							vec[currentMidpointPosition + midpointOffset + resolution] //bottom
							) / 3.0f;

						//top point
						vec[currentMidpointPosition - resolution] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - resolution - midpointOffset] + //left
							vec[currentMidpointPosition - resolution + midpointOffset] //right
							) / 3.0f;

						//bottom point
						vec[currentMidpointPosition + resolution] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + resolution - midpointOffset] + //left
							vec[currentMidpointPosition + resolution + midpointOffset] + //right
							vec[currentMidpointPosition + resolution + resolution] //bottom
							) / 4.0f;

#pragma endregion
					}
					else if (currentLine = 2 ^ (numberOfIterations - 1)){
#pragma region Bottom Right Corner

						//left point
						vec[currentMidpointPosition - midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint 
							vec[currentMidpointPosition - midpointOffset - midpointOffset] + //left
							vec[currentMidpointPosition - midpointOffset - resolution] + //top
							vec[currentMidpointPosition - midpointOffset + resolution] //bottom
							) / 4.0f;

						//right point
						vec[currentMidpointPosition + midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + midpointOffset - resolution] + //top
							vec[currentMidpointPosition + midpointOffset + resolution] //bottom
							) / 3.0f;

						//top point
						vec[currentMidpointPosition - resolution] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - resolution - midpointOffset] + //left
							vec[currentMidpointPosition - resolution + midpointOffset] + //right
							vec[currentMidpointPosition - resolution - resolution]  //top
							) / 4.0f;

						//bottom point
						vec[currentMidpointPosition + resolution] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + resolution - midpointOffset] + //left
							vec[currentMidpointPosition + resolution + midpointOffset] //right
							) / 3.0f;

#pragma endregion
					}
					else{
#pragma region Right Edge

						//left point
						vec[currentMidpointPosition - midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint 
							vec[currentMidpointPosition - midpointOffset - midpointOffset] + //left
							vec[currentMidpointPosition - midpointOffset - resolution] + //top
							vec[currentMidpointPosition - midpointOffset + resolution] //bottom
							) / 4.0f;

						//right point
						vec[currentMidpointPosition + midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + midpointOffset - resolution] + //top
							vec[currentMidpointPosition + midpointOffset + resolution] //bottom
							) / 3.0f;

						//top point
						vec[currentMidpointPosition - resolution] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - resolution - midpointOffset] + //left
							vec[currentMidpointPosition - resolution + midpointOffset] + //right
							vec[currentMidpointPosition - resolution - resolution]  //top
							) / 4.0f;

						//bottom point
						vec[currentMidpointPosition + resolution] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + resolution - midpointOffset] + //left
							vec[currentMidpointPosition + resolution + midpointOffset] + //right
							vec[currentMidpointPosition + resolution + resolution] //bottom
							) / 4.0f;

#pragma endregion
					}
#pragma endregion
				}
				else{
#pragma region Middle Columns

					if (currentLine == 1){
#pragma region Top Edge
						//left point
						vec[currentMidpointPosition - midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint 
							vec[currentMidpointPosition - midpointOffset - midpointOffset] + //left
							vec[currentMidpointPosition - midpointOffset - resolution] + //top
							vec[currentMidpointPosition - midpointOffset + resolution] //bottom
							) / 4.0f;

						//right point
						vec[currentMidpointPosition + midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + midpointOffset + midpointOffset] + //right
							vec[currentMidpointPosition + midpointOffset - resolution] + //top
							vec[currentMidpointPosition + midpointOffset + resolution] //bottom
							) / 4.0f;

						//top point
						vec[currentMidpointPosition - resolution] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - resolution - midpointOffset] + //left
							vec[currentMidpointPosition - resolution + midpointOffset] //right
							) / 3.0f;

						//bottom point
						vec[currentMidpointPosition + resolution] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + resolution - midpointOffset] + //left
							vec[currentMidpointPosition + resolution + midpointOffset] + //right
							vec[currentMidpointPosition + resolution + resolution] //bottom
							) / 4.0f;
#pragma endregion
					}
					else if (currentLine = 2 ^ (numberOfIterations - 1)){
#pragma region Bottom Edge

						//left point
						vec[currentMidpointPosition - midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint 
							vec[currentMidpointPosition - midpointOffset - midpointOffset] + //left
							vec[currentMidpointPosition - midpointOffset - resolution] + //top
							vec[currentMidpointPosition - midpointOffset + resolution] //bottom
							) / 4.0f;

						//right point
						vec[currentMidpointPosition + midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + midpointOffset + midpointOffset] + //right
							vec[currentMidpointPosition + midpointOffset - resolution] + //top
							vec[currentMidpointPosition + midpointOffset + resolution] //bottom
							) / 4.0f;

						//top point
						vec[currentMidpointPosition - resolution] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - resolution - midpointOffset] + //left
							vec[currentMidpointPosition - resolution + midpointOffset] + //right
							vec[currentMidpointPosition - resolution - resolution]  //top
							) / 4.0f;

						//bottom point
						vec[currentMidpointPosition + resolution] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + resolution - midpointOffset] + //left
							vec[currentMidpointPosition + resolution + midpointOffset] //right
							) / 3.0f;

#pragma endregion
					}
					else{
#pragma region Rest

						//left point
						vec[currentMidpointPosition - midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint 
							vec[currentMidpointPosition - midpointOffset - midpointOffset] + //left
							vec[currentMidpointPosition - midpointOffset - resolution] + //top
							vec[currentMidpointPosition - midpointOffset + resolution] //bottom
							) / 4.0f;

						//right point
						vec[currentMidpointPosition + midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + midpointOffset + midpointOffset] + //right
							vec[currentMidpointPosition + midpointOffset - resolution] + //top
							vec[currentMidpointPosition + midpointOffset + resolution] //bottom
							) / 4.0f;

						//top point
						vec[currentMidpointPosition - resolution] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - resolution - midpointOffset] + //left
							vec[currentMidpointPosition - resolution + midpointOffset] + //right
							vec[currentMidpointPosition - resolution - resolution]  //top
							) / 4.0f;

						//bottom point
						vec[currentMidpointPosition + resolution] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + resolution - midpointOffset] + //left
							vec[currentMidpointPosition + resolution + midpointOffset] + //right
							vec[currentMidpointPosition + resolution + resolution] //bottom
							) / 4.0f;
#pragma endregion
					}

#pragma endregion
				}
			}
		}
	}
};




