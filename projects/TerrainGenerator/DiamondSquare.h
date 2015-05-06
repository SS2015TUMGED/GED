/*	This Class implements the Diamond - Square Algorithm to
*	fill an 1D-normalized 2D-Array with distributed numbers 
*	between 0.0 and 1.0.
*/

#pragma once

#include <vector>
#include <random>

class DiamondSquare{
public:
	DiamondSquare();
	~DiamondSquare();

	/* Uses the Diamond Square Algorithm on the given vector<float>.
	 * @param: 
	 * vector<float> vec	:= the vector on which the algorithm is executed
	 */
	static std::vector<float> diamondSquareAlgorithm(std::vector<float> vec);



	/* Returns normally distributed random values between min and max.
	 * @param:
	 * float min	:= the mininmal number (included)
	 * float max	:= the maximal number (included)
	 */
	static float normalDisRandom(float min, float max);

	//Normally Distributed Random Number Generator
	static std::default_random_engine rng; //create new rng

	static unsigned int resolution;

private: 

	//SquareStep of the algorithm through the whole array
	static void squareStep(std::vector<float> &vec);

};
