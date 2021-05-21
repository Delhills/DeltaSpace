#include "player.h"

Player::Player()
{

	Texture* texture = Texture::Get("data/player.png");
	Mesh* mesh = Mesh::Get("data/player.obj");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	Vector4 color = Vector4(1, 1, 1, 1);

	entity = new EntityMesh(mesh, texture, shader, color);
	pos = entity->model.getTranslation();
	this->speed = Vector3(0, 0, 0);
	this->targetPos = Vector3();
}


float max_speed = 1;
float min_speed = 0.2;
float turn_speed = 0.5;

void Player::accelerate(float delta)
{

	float finalspeed = lerp(this->speed.z, this->speed.z + delta, 0.08);
	this->speed.z = clamp(finalspeed, min_speed, max_speed);
}
void Player::turn(float delta)
{	
	float finalspeed = lerp(this->speed.x, this->speed.x + delta, 0.5);
	this->speed.x = clamp(finalspeed, -this->speed.z* turn_speed, this->speed.z* turn_speed);
	this->pos.x = clamp(this->pos.x, -9.2, 9.85);
}