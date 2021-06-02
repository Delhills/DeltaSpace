#ifndef PLAYER_H
#define PLAYER_H

#include "framework.h"
#include "entities.h"

class Player
{
public:
	Vector3 pos;
	//Matrix44 rotation;
	float rot;
	EntityMesh* entity;
	Vector3 speed;
	Vector3 normal;
	Player();

	void turn(float delta);
	void accelerate(float delta);
	void Render();
};

#endif