#pragma once
#include <iostream>
#include <set>
#include <map>
#include "Mesh.h"

class ConfigParser
{
public:
	struct Color {
		float r,g,b;
	};

	/*struct terrainPath {
		static std::string height;
		static std::string color;
		static std::string normal;
	};*/

	// Assignment 07: new structures for Cockpit- and GroundObjects
	struct CockpitObject {
		std::string Name;
		float Scale;
		int RotX;
		int RotY;
		int RotZ;
		float TransX;
		float TransY;
		float TransZ;
	};

	struct GroundObject {
		std::string Name;
		float Scale;
		int RotX;
		int RotY;
		int RotZ;
		float TransX;
		float TransY;
		float TransZ;
	};

	struct EnemyType {
		std::string Name;
		int Hitpoints;
		int Size;
		int Speed;
		std::string Mesh;
		float Scale;
		int RotX;
		int RotY;
		int RotZ;
		float TransX;
		float TransY;
		float TransZ;
		int SpawnRate;
		bool Spawn;
	};

	static std::string height;
	static std::string color;
	static std::string normal;
	static bool terrainSpinning;
	static std::map<std::string, Mesh*> g_Meshes;
	// contains all Cockpit- and GroundObjects
	static std::vector<CockpitObject> cockpitObjects;
	static std::vector<GroundObject> groundObjects;
	//contains all Enemys
	static std::map<std::string, EnemyType> enemys;
	static std::vector<std::wstring> sprites;

	// getters
	float getSpinning();
	float getSpinSpeed();
	float getTerrainWidth();
	float getTerrainDepth();
	float getTerrainHeight();
	Color getBackgroundColor();
	void getPathes(std::string path);
	
	void load(std::string str);

	ConfigParser();
	~ConfigParser();

	struct Gun
	{
		std::string type;
		DirectX::XMFLOAT3 position;     // world-space position (sprite center)
		DirectX::XMFLOAT3 velocity;		//Velocity vector of the Sprite
		float radius;                   // world-space radius (= half side length of the sprite quad)
		float speed;
		float grav;
		float cd;
		float dmg;
		int textureIndex;               // which texture to use (out of SpriteRenderer::m_spriteSRV)
	};

	static Gun Gatling, Plasma;

private:

	

	float spinning, spinSpeed,
			terrainWidth, terrainDepth, terrainHeight;

	Color backgroundColor;

	

};

