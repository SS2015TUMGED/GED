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
	 * const unsigned int res := resolution of the vector
	 */
	static void diamondSquareAlgorithm(std::vector<float> &vec, const unsigned int res);



	/* Returns normally distributed random values between min and max.
	 * @param:
	 * float min	:= the mininmal number (included)
	 * float max	:= the maximal number (included)
	 */
	static float normalDisRandom(float min, float max);

	//Normally Distributed Random Number Generator
	static std::default_random_engine rng; //create new rng

	static unsigned int resolution;

	//cutting the vector in the right size
	static std::vector<float>* CutBoundarys(std::vector<float> &vec);

private: 

	//SquareStep of the algorithm 
	static void squareStep(std::vector<float> &vec);

	//DiamondStep of the algorithm
	static void diamondStep(std::vector<float>& v);

	//helper function
	static void diamondStepSingle(std::vector<float>& v, int startx, int starty, unsigned int iteration);

	//Calculates a random procentual roughness
	static float roughness();

	//Calculates a random procentual roughness
	static float roughness2();
};
