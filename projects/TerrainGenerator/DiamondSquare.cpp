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

	float min = 0.3f;
	float max = 0.7f;

	//assign random values to the 4 corners

	//top left
	vec[0] = normalDisRandom(min, max);

	// bottom right
	vec[(resolution * resolution) - 1] = 0.3f; // normalDisRandom(min, max);

	//top right
	vec[resolution - 1] = normalDisRandom(min, max);

	//bottom left
	vec[IDX(0, resolution - 1, resolution)] = 0.4f;//normalDisRandom(min, max);



	for (int i = 0; i < (int) log2(resolution - 1); i++)
	{
		diamondStep(vec);
		squareStep(vec);
	}

};

void DiamondSquare::diamondStep(std::vector<float>& v)
{
	//counting the iteration steps
	static unsigned int numberOfIterations = 1;
	//How many SingleSteps need to be done by line
	int SingleStepCount = (int) pow(2, numberOfIterations - 1);

	for (int i = 0; i < SingleStepCount; i++) //column
	{
		for (int k = 0; k < SingleStepCount; k++) //line
		{
			diamondStepSingle(
				v,
				k*((resolution - 1) / SingleStepCount),
				i*((resolution - 1) / SingleStepCount),
				SingleStepCount);
		}
	}

	numberOfIterations++;
};

void DiamondSquare::diamondStepSingle(std::vector<float>& v, int startx, int starty, unsigned int iteration)
{
	// for the hills
	static unsigned b = 0;

	//Accessing the boarders and writing the average value in the middle
	v[IDX(startx + (resolution - 1) / iteration / 2,
			starty + (resolution - 1) / iteration / 2,
			resolution)] =
		(((v[IDX(startx, starty, resolution)] + 
		v[IDX(startx + (resolution - 1) / iteration, starty, resolution)] +
		v[IDX(startx, starty + (resolution - 1) / iteration, resolution)] + 
		v[IDX(startx + (resolution - 1) / iteration,
				starty + (resolution - 1) / iteration,
				resolution)]) / 4.0f)  * roughness()) + roughness2();
	
	// just for the hills
	if (iteration == 2 && b == 0){
		
		v[IDX(startx + (resolution - 1) / iteration / 2,
			starty + (resolution - 1) / iteration / 2,
			resolution)] = 0.8f;
		b += 1;
	}
	else if (iteration == 2 && b == 1){
		b += 1;

		v[IDX(((startx + (resolution - 1) / iteration / 2)) ,
			starty + (resolution - 1) / iteration / 2,
			resolution)] = 0.6f;
	}
	

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
	offset = (max - min) / 4.0f;

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
	
	unsigned int midpointOffset =(int) ((resolution - 1) / pow(2, numberOfIterations - 1)) / 2;
	std::cout << midpointOffset << std::endl;

	unsigned currentMidpointPosition =
		IDX(midpointOffset, midpointOffset, resolution);

	//iterate through 'Lines of Squares' eg 1 in the first iteration
	//two in the second iteration etc.
	for (unsigned currentLine = 0; 
		currentLine < (pow(2,numberOfIterations - 1)); 
		currentLine ++)
	{

		currentMidpointPosition = (IDX(midpointOffset, 
			(2 * midpointOffset) * currentLine + midpointOffset,  
			resolution) );
	
		//iterate through the 'Columns of Squares' eg 1 in the first iteration etc
		//but always according to the midpoint
		for (unsigned currentColumn = 1; 
			currentColumn <= (pow(2, numberOfIterations - 1) );
			currentColumn++)
		{	
			if (numberOfIterations == 1){

#pragma region first iteration
				//left point
				vec[currentMidpointPosition - midpointOffset] =
					((vec[currentMidpointPosition] + //midpoint 
					vec[currentMidpointPosition - midpointOffset - (resolution * midpointOffset)] + //top
					vec[currentMidpointPosition - midpointOffset + (resolution * midpointOffset)] //bottom
					) / 3.0f) * roughness() + roughness2();

				//right point
				vec[currentMidpointPosition + midpointOffset] =
					(vec[currentMidpointPosition] + //midpoint
					vec[currentMidpointPosition + midpointOffset - (resolution * midpointOffset)] + //top
					vec[currentMidpointPosition + midpointOffset + (resolution * midpointOffset)] //bottom
					) / 3.0f * roughness() + roughness2();

				//top point
				vec[currentMidpointPosition - (resolution * midpointOffset)] =
					(vec[currentMidpointPosition] + //midpoint
					vec[currentMidpointPosition - (resolution * midpointOffset) - midpointOffset] + //left
					vec[currentMidpointPosition - (resolution * midpointOffset) + midpointOffset] //right
					) / 3.0f * roughness() + roughness2();

				//bottom point
				vec[currentMidpointPosition + (resolution * midpointOffset)] =
					(vec[currentMidpointPosition] + //midpoint
					vec[currentMidpointPosition + (resolution * midpointOffset) - midpointOffset] + //left
					vec[currentMidpointPosition + (resolution * midpointOffset) + midpointOffset] //right
					) / 3.0f * roughness() + roughness2();

#pragma endregion
			}
			else {
				//checking for special cases (borders)
				if (currentColumn == 1){
#pragma region First Column
					if (currentLine == 0){
#pragma region Top Left Corner

						//left point
						vec[currentMidpointPosition - midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - midpointOffset - (resolution * midpointOffset)] + //top
							vec[currentMidpointPosition - midpointOffset + (resolution * midpointOffset)] //bottom
							) / 3.0f * roughness() + roughness2();

						//right point
						vec[currentMidpointPosition + midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + midpointOffset + midpointOffset] + //right
							vec[currentMidpointPosition + midpointOffset - (resolution * midpointOffset)] + //top
							vec[currentMidpointPosition + midpointOffset + (resolution * midpointOffset)] //bottom
							) / 4.0f * roughness() + roughness2();

						//top point
						vec[currentMidpointPosition - (resolution * midpointOffset)] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - (resolution * midpointOffset) - midpointOffset] + //left
							vec[currentMidpointPosition - (resolution * midpointOffset) + midpointOffset] //right
							) / 3.0f * roughness() + roughness2();

						//bottom point
						vec[currentMidpointPosition + (resolution * midpointOffset)] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + (resolution * midpointOffset) - midpointOffset] + //left
							vec[currentMidpointPosition + (resolution * midpointOffset) + midpointOffset] + //right
							vec[currentMidpointPosition + (resolution * midpointOffset) + (resolution * midpointOffset)] //bottom
							) / 4.0f * roughness() + roughness2();
#pragma endregion
					}
					else if (currentLine == (pow(2, numberOfIterations - 1))-1){ //bottom left corner
#pragma region Bottom Left Corner	
						//left point
						vec[currentMidpointPosition - midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - midpointOffset - (resolution * midpointOffset)] + //top
							vec[currentMidpointPosition - midpointOffset + (resolution * midpointOffset)] //bottom
							) / 3.0f * roughness() + roughness2();

						//right point
						vec[currentMidpointPosition + midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + midpointOffset + midpointOffset] + //right
							vec[currentMidpointPosition + midpointOffset - (resolution * midpointOffset)] + //top
							vec[currentMidpointPosition + midpointOffset + (resolution * midpointOffset)] //bottom
							) / 4.0f * roughness() + roughness2();

						//top point
						vec[currentMidpointPosition - (resolution * midpointOffset)] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - (resolution * midpointOffset) - midpointOffset] + //left
							vec[currentMidpointPosition - (resolution * midpointOffset) + midpointOffset] + //right
							vec[currentMidpointPosition - (resolution * midpointOffset) - (resolution * midpointOffset)]  //top
							) / 4.0f * roughness() + roughness2();

						//bottom point
						vec[currentMidpointPosition + (resolution * midpointOffset)] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + (resolution * midpointOffset) - midpointOffset] + //left
							vec[currentMidpointPosition + (resolution * midpointOffset) + midpointOffset] //right
							) / 3.0f * roughness() + roughness2();
#pragma endregion
					}
					else{
#pragma region Left Edge
						//left point
						vec[currentMidpointPosition - midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - midpointOffset - (resolution * midpointOffset)] + //top
							vec[currentMidpointPosition - midpointOffset + (resolution * midpointOffset)] //bottom
							) / 3.0f * roughness() + roughness2();

						//right point
						vec[currentMidpointPosition + midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + midpointOffset + midpointOffset] + //right
							vec[currentMidpointPosition + midpointOffset - (resolution * midpointOffset)] + //top
							vec[currentMidpointPosition + midpointOffset + (resolution * midpointOffset)] //bottom
							) / 4.0f * roughness() + roughness2();

						//top point
						vec[currentMidpointPosition - (resolution * midpointOffset)] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - (resolution * midpointOffset) - midpointOffset] + //left
							vec[currentMidpointPosition - (resolution * midpointOffset) + midpointOffset] + //right
							vec[currentMidpointPosition - (resolution * midpointOffset) - (resolution * midpointOffset)]  //top
							) / 4.0f * roughness() + roughness2();

						//bottom point
						vec[currentMidpointPosition + (resolution * midpointOffset)] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + (resolution * midpointOffset) - midpointOffset] + //left
							vec[currentMidpointPosition + (resolution * midpointOffset) + midpointOffset] + //right
							vec[currentMidpointPosition + (resolution * midpointOffset) + (resolution * midpointOffset)] //bottom
							) / 4.0f * roughness() + roughness2();
#pragma endregion
					}
#pragma endregion
				}
				else if (currentColumn == (pow(2, numberOfIterations - 1))){
#pragma region Last Column
					if (currentLine == 0){
#pragma region Top Right Corner

						//left point
						vec[currentMidpointPosition - midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint 
							vec[currentMidpointPosition - midpointOffset - midpointOffset] + //left
							vec[currentMidpointPosition - midpointOffset - (resolution * midpointOffset)] + //top
							vec[currentMidpointPosition - midpointOffset + (resolution * midpointOffset)] //bottom
							) / 4.0f * roughness() + roughness2();

						//right point
						vec[currentMidpointPosition + midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + midpointOffset - (resolution * midpointOffset)] + //top
							vec[currentMidpointPosition + midpointOffset + (resolution * midpointOffset)] //bottom
							) / 3.0f * roughness() + roughness2();

						//top point
						vec[currentMidpointPosition - (resolution * midpointOffset)] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - (resolution * midpointOffset) - midpointOffset] + //left
							vec[currentMidpointPosition - (resolution * midpointOffset) + midpointOffset] //right
							) / 3.0f * roughness() + roughness2();

						//bottom point
						vec[currentMidpointPosition + (resolution * midpointOffset)] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + (resolution * midpointOffset) - midpointOffset] + //left
							vec[currentMidpointPosition + (resolution * midpointOffset) + midpointOffset] + //right
							vec[currentMidpointPosition + (resolution * midpointOffset) + (resolution * midpointOffset)] //bottom
							) / 4.0f * roughness() + roughness2();

#pragma endregion
					}
					else if (currentLine == (pow(2, numberOfIterations - 1))-1){
#pragma region Bottom Right Corner

						//left point
						vec[currentMidpointPosition - midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint 
							vec[currentMidpointPosition - midpointOffset - midpointOffset] + //left
							vec[currentMidpointPosition - midpointOffset - (resolution * midpointOffset)] + //top
							vec[currentMidpointPosition - midpointOffset + (resolution * midpointOffset)] //bottom
							) / 4.0f * roughness() + roughness2();

						//right point
						vec[currentMidpointPosition + midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + midpointOffset - (resolution * midpointOffset)] + //top
							vec[currentMidpointPosition + midpointOffset + (resolution * midpointOffset)] //bottom
							) / 3.0f * roughness() + roughness2();

						//top point
						vec[currentMidpointPosition - (resolution * midpointOffset)] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - (resolution * midpointOffset) - midpointOffset] + //left
							vec[currentMidpointPosition - (resolution * midpointOffset) + midpointOffset] + //right
							vec[currentMidpointPosition - (resolution * midpointOffset) - (resolution * midpointOffset)]  //top
							) / 4.0f * roughness() + roughness2();

						//bottom point
						vec[currentMidpointPosition + (resolution * midpointOffset)] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + (resolution * midpointOffset) - midpointOffset] + //left
							vec[currentMidpointPosition + (resolution * midpointOffset) + midpointOffset] //right
							) / 3.0f * roughness() + roughness2();

#pragma endregion
					}
					else{
#pragma region Right Edge

						//left point
						vec[currentMidpointPosition - midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint 
							vec[currentMidpointPosition - midpointOffset - midpointOffset] + //left
							vec[currentMidpointPosition - midpointOffset - (resolution * midpointOffset)] + //top
							vec[currentMidpointPosition - midpointOffset + (resolution * midpointOffset)] //bottom
							) / 4.0f * roughness() + roughness2();

						//right point
						vec[currentMidpointPosition + midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + midpointOffset - (resolution * midpointOffset)] + //top
							vec[currentMidpointPosition + midpointOffset + (resolution * midpointOffset)] //bottom
							) / 3.0f * roughness() + roughness2();

						//top point
						vec[currentMidpointPosition - (resolution * midpointOffset)] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - (resolution * midpointOffset) - midpointOffset] + //left
							vec[currentMidpointPosition - (resolution * midpointOffset) + midpointOffset] + //right
							vec[currentMidpointPosition - (resolution * midpointOffset) - (resolution * midpointOffset)]  //top
							) / 4.0f * roughness() + roughness2();

						//bottom point
						vec[currentMidpointPosition + (resolution * midpointOffset)] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + (resolution * midpointOffset) - midpointOffset] + //left
							vec[currentMidpointPosition + (resolution * midpointOffset) + midpointOffset] + //right
							vec[currentMidpointPosition + (resolution * midpointOffset) + (resolution * midpointOffset)] //bottom
							) / 4.0f * roughness() + roughness2();

#pragma endregion
					}
#pragma endregion
				}
				else{
#pragma region Middle Columns

					if (currentLine == 0){
#pragma region Top Edge
						//left point
						vec[currentMidpointPosition - midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint 
							vec[currentMidpointPosition - midpointOffset - midpointOffset] + //left
							vec[currentMidpointPosition - midpointOffset - (resolution * midpointOffset)] + //top
							vec[currentMidpointPosition - midpointOffset + (resolution * midpointOffset)] //bottom
							) / 4.0f * roughness() + roughness2();

						//right point
						vec[currentMidpointPosition + midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + midpointOffset + midpointOffset] + //right
							vec[currentMidpointPosition + midpointOffset - (resolution * midpointOffset)] + //top
							vec[currentMidpointPosition + midpointOffset + (resolution * midpointOffset)] //bottom
							) / 4.0f * roughness() + roughness2();

						//top point
						vec[currentMidpointPosition - (resolution * midpointOffset)] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - (resolution * midpointOffset) - midpointOffset] + //left
							vec[currentMidpointPosition - (resolution * midpointOffset) + midpointOffset] //right
							) / 3.0f * roughness() + roughness2();

						//bottom point
						vec[currentMidpointPosition + (resolution * midpointOffset)] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + (resolution * midpointOffset) - midpointOffset] + //left
							vec[currentMidpointPosition + (resolution * midpointOffset) + midpointOffset] + //right
							vec[currentMidpointPosition + (resolution * midpointOffset) + (resolution * midpointOffset)] //bottom
							) / 4.0f * roughness() + roughness2();
#pragma endregion
					}
					else if (currentLine == (pow(2, numberOfIterations - 1))-1){
#pragma region Bottom Edge

						//left point
						vec[currentMidpointPosition - midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint 
							vec[currentMidpointPosition - midpointOffset - midpointOffset] + //left
							vec[currentMidpointPosition - midpointOffset - (resolution * midpointOffset)] + //top
							vec[currentMidpointPosition - midpointOffset + (resolution * midpointOffset)] //bottom
							) / 4.0f * roughness() + roughness2();

						//right point
						vec[currentMidpointPosition + midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + midpointOffset + midpointOffset] + //right
							vec[currentMidpointPosition + midpointOffset - (resolution * midpointOffset)] + //top
							vec[currentMidpointPosition + midpointOffset + (resolution * midpointOffset)] //bottom
							) / 4.0f * roughness() + roughness2();

						//top point
						vec[currentMidpointPosition - (resolution * midpointOffset)] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - (resolution * midpointOffset) - midpointOffset] + //left
							vec[currentMidpointPosition - (resolution * midpointOffset) + midpointOffset] + //right
							vec[currentMidpointPosition - (resolution * midpointOffset) - (resolution * midpointOffset)]  //top
							) / 4.0f * roughness() + roughness2();

						//bottom point
						vec[currentMidpointPosition + (resolution * midpointOffset)] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + (resolution * midpointOffset) - midpointOffset] + //left
							vec[currentMidpointPosition + (resolution * midpointOffset) + midpointOffset] //right
							) / 3.0f * roughness() + roughness2();

#pragma endregion
					}
					else{
#pragma region Rest

						//left point
						vec[currentMidpointPosition - midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint 
							vec[currentMidpointPosition - midpointOffset - midpointOffset] + //left
							vec[currentMidpointPosition - midpointOffset - (resolution * midpointOffset)] + //top
							vec[currentMidpointPosition - midpointOffset + (resolution * midpointOffset)] //bottom
							) / 4.0f * roughness() + roughness2();

						//right point
						vec[currentMidpointPosition + midpointOffset] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + midpointOffset + midpointOffset] + //right
							vec[currentMidpointPosition + midpointOffset - (resolution * midpointOffset)] + //top
							vec[currentMidpointPosition + midpointOffset + (resolution * midpointOffset)] //bottom
							) / 4.0f * roughness() + roughness2();

						//top point
						vec[currentMidpointPosition - (resolution * midpointOffset)] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition - (resolution * midpointOffset) - midpointOffset] + //left
							vec[currentMidpointPosition - (resolution * midpointOffset) + midpointOffset] + //right
							vec[currentMidpointPosition - (resolution * midpointOffset) - (resolution * midpointOffset)]  //top
							) / 4.0f * roughness() + roughness2();

						//bottom point
						vec[currentMidpointPosition + (resolution * midpointOffset)] =
							(vec[currentMidpointPosition] + //midpoint
							vec[currentMidpointPosition + (resolution * midpointOffset) - midpointOffset] + //left
							vec[currentMidpointPosition + (resolution * midpointOffset) + midpointOffset] + //right
							vec[currentMidpointPosition + (resolution * midpointOffset) + (resolution * midpointOffset)] //bottom
							) / 4.0f * roughness() + roughness2() ;
#pragma endregion
					}

#pragma endregion
				}
			}

			//setting the midpoint of the Square to the correct position
			currentMidpointPosition = currentMidpointPosition + midpointOffset + midpointOffset;
			
		}
	}
};

//multiplied roughness factor
float DiamondSquare::roughness(){
	return normalDisRandom(0.99f, 1.01f) ;
};

//added roughness factor
float DiamondSquare::roughness2(){
	return normalDisRandom(-0.05f, 0.05f);
};

//Cutting the vector back to the correct size and
//adding ´some more roughness
std::vector<float>* DiamondSquare::CutBoundarys(std::vector<float> &vec){
	using namespace std;
	//creating second, smaller vector
	vector<float> *newArray = new vector<float>((resolution - 1) * (resolution - 1));

	//iterating through the old vector
	for (unsigned ypos = 0; ypos < resolution - 1; ypos++){

		for (unsigned xpos = 0; xpos < resolution - 1; xpos++){
			//reading value from the old vector
			float currentValue = vec[IDX(xpos, ypos, resolution)];

			//adding some more roughness factors
			currentValue = currentValue * 1.8f;
			currentValue = currentValue * normalDisRandom(0.98f, 1.02f);
			currentValue = currentValue - 0.25f;

			//cutting values that are too big or small
			if (currentValue > 1.0f){ currentValue = 1.0f; }
			else if (currentValue < 0.0f){ currentValue = 0.0f; }

			//writing the value in the new vector
			(* newArray)[IDX(xpos, ypos, resolution - 1)] = currentValue ;
		}
	}
	//delete old vector
	delete &vec;
	return newArray;
}