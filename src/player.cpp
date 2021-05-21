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
}


float max_speed = 1;
float min_speed = 0.2;
float turn_speed = 0.5;

void Player::accelerate(float delta)
{
	this->speed.z = clamp(this->speed.z + delta, min_speed, max_speed);
}
void Player::turn(float delta)
{
	this->speed.x = clamp(this->speed.x + delta, -this->speed.z* turn_speed, this->speed.z* turn_speed);
}