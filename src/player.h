#ifndef PLAYER_H
#define PLAYER_H

#include "framework.h"
#include "entities.h"

class Player
{
public:
	Vector3 pos;
	EntityMesh* entity;
	Vector3 speed;
	Vector3 targetPos;	

	Player();

	void turn(float delta);
	void accelerate(float delta);
};

#endif