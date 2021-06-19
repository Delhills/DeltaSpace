#ifndef PLAYER_H
#define PLAYER_H

#include "framework.h"
#include "entities.h"

class Player
{
public:
	Vector3 pos;
	float turn_speed_coef;
	float speed_coef;
	float max_speed;
	float min_speed;
	EntityMesh* entity;
	Vector3 speed;
	Vector3 normal;
	Player();

	void turn(float delta);
	void accelerate(float delta);
	void Render();
	
};

#endif