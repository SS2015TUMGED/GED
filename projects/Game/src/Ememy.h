#pragma once
#include <DirectXMath.h>
#include <string>
#include "ConfigParser.h"
#include <list>

class Ememy
{
public:
	Ememy();
	~Ememy();

	struct EnemyInstance{
		DirectX::XMVECTOR pos; //position p in world space
		DirectX::XMVECTOR vel; //velocity v in world space
		int hitpoints;
		std::wstring name;
		ConfigParser::EnemyType type;
	};

	std::list<EnemyInstance> g_EnemyInstances;

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

