#include "Ememy.h"
#include <DirectXMath.h>

#define _USE_MATH_DEFINES
#include <math.h>


Ememy::Ememy()
{
}


Ememy::~Ememy()
{
}
std::list<Ememy::EnemyInstance> Ememy::g_EnemyInstances;
void Ememy::spawn(ConfigParser::EnemyType enemy, float width)
{
	//initiating instance variables
	Ememy::EnemyInstance instance;
	instance.hitpoints = enemy.Hitpoints;
	instance.name = enemy.Name;
	instance.type = enemy;

	
	//calculating and initiating spawn position
	float random_value = Ememy::normalDisRandom(0.0f, M_PI * 2.0f);
	float random_height = Ememy::normalDisRandom(42.0f, 300.0f);
	DirectX::XMFLOAT3 pos;
	pos.x = width * std::sin(random_value);
	pos.y = random_height;
	pos.z = width * std::cos(random_value);
	//load position to the instance
	instance.pos = DirectX::XMLoadFloat3(&pos); 

	//calculate and initiate direction vector
	float random_value2 = Ememy::normalDisRandom(0.0f, M_PI * 2.0f);
	DirectX::XMFLOAT3 vel;
	vel.x = (width / 16) * std::sin(random_value2) - pos.x;
	vel.y = random_height - pos.y;
	vel.z = (width / 16) * std::cos(random_value) - pos.z;
	//load vector into the instance
	instance.vel = DirectX::XMLoadFloat3(&vel);
	DirectX::XMVector3Normalize(instance.vel);
	//Normalized Vector * Speed
	DirectX::XMVectorSetByIndex(instance.vel, DirectX::XMVectorGetByIndex(instance.vel,0)*enemy.Speed, 0);
	DirectX::XMVectorSetByIndex(instance.vel, DirectX::XMVectorGetByIndex(instance.vel, 1)*enemy.Speed,1);
	DirectX::XMVectorSetByIndex(instance.vel, DirectX::XMVectorGetByIndex(instance.vel, 2)*enemy.Speed, 2);
	g_EnemyInstances.push_back(instance);
}

/* Returns normally distributed random values between min and max.
* @param:
* float min	:= the mininmal number (included)
* float max	:= the maximal number (included)
*/
float Ememy::normalDisRandom(float min, float max){

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
