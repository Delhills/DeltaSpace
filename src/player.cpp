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



void Player::accelerate(float delta)
{
	this->speed.z = clamp(this->speed.z + delta, 0.2, 1);
}
void Player::turn(float delta)
{
	this->speed.x = clamp(this->speed.x + delta, -this->speed.z*0.5, this->speed.z*0.5);
}