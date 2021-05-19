#ifndef PLAYER_H
#define PLAYER_H

#include "framework.h"
#include "entities.h"

class Player
{
public:
	Vector3 pos;
	EntityMesh* entity;
	Vector3 playerVel;

	Player();

	void move(Vector3 delta);

};

#endif