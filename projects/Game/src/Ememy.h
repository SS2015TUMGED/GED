#pragma once
#include <DirectXMath.h>
#include <string>
#include "ConfigParser.h"
#include <list>
#include <random>

class Ememy
{
public:
	Ememy();
	~Ememy();

	struct EnemyInstance{
		DirectX::XMVECTOR pos; //position p in world space
		DirectX::XMVECTOR vel; //velocity v in world space
		int hitpoints;
		std::string name;
		ConfigParser::EnemyType type;

	};

	static std::list<EnemyInstance> g_EnemyInstances;

	static void spawn(ConfigParser::EnemyType, float width);

	/* Returns normally distributed random values between min and max.
	* @param:
	* float min	:= the mininmal number (included)
	* float max	:= the maximal number (included)
	*/
	static float normalDisRandom(float min, float max);

	//Normally Distributed Random Number Generator
	static std::default_random_engine rng; //create new rng

private:
	float posX;
	float posY;
	float posZ;
	float velocityX;
	float velocityY;
	float velocityZ;
	int remainingHitpoints;
	float spawnRate;
};

