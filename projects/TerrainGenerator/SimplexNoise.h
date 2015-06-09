#pragma once

#include <vector>
#include <random>


class SimplexNoise
{
public:
	SimplexNoise();
	~SimplexNoise();


	/* Returns normally distributed random values between min and max.
	* @param:
	* float min	:= the mininmal number (included)
	* float max	:= the maximal number (included)
	*/
	static float normalDisRandom(float min, float max);

	//Normally Distributed Random Number Generator
	static std::default_random_engine rng; //create new rng

private:
	//structs
	struct gradient {
	
		short x;
		short y;
		short z;
		gradient( short x, short y, short z) : x(x), y(y), z(z) {}
		gradient() : x(0), y(0), z(0) {}

	};

	static const std::vector<short> p;
	
	//variables
	static unsigned char SimplexNoise::perm[];
	//static std::vector<short> permMod12;
	
	static float SimplexNoise::simplexPixel(float xin, float yin);
	static float SimplexNoise::grad(int hash, float x, float y);

public:
	
	static void simplex(std::vector<float> &vec, int resolution);

};
