#include "DiamondSquare.h"
#include <random>
#include <iostream>
#include <chrono>;

// Access a 2D array of width w at position x / y 
#define IDX(xpos, ypos, width) ((xpos) + (ypos) * (width))

/* Uses the Diamond Square Algorithm on the given vector<float>.
* @param:
* vector<float> vec	:= the vector on which the algorithm is executed
*/
std::vector<float> DiamondSquare::diamondSquareAlgorithm(std::vector<float> vec){
	return vec;
};

/* Method to create a seed for an random number generator
* @return: unsigned int  the seed from random value
*/
unsigned int getSeed(){
	//start timer 
	typedef std::chrono::high_resolution_clock myclock;
	myclock::time_point beginning = myclock::now();

	// obtain a seed from the timer
	myclock::duration d = myclock::now() - beginning;
	unsigned int seed = d.count();

	return seed;
}

/* Returns normally distributed random values between min and max.
* @param:
* float min	:= the mininmal number (included)
* float max	:= the maximal number (included)
*/
float DiamondSquare::normalDisRandom(float min, float max, unsigned seed){

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





