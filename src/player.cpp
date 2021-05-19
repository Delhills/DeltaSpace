#include "player.h"

Player::Player()
{

	Texture* texture = Texture::Get("data/player.png");
	Mesh* mesh = Mesh::Get("data/player.obj");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	Vector4 color = Vector4(1, 1, 1, 1);

	entity = new EntityMesh(mesh, texture, shader, color);
	pos = entity->model.getTranslation();

}


void Player::move(Vector3 delta)
{
	this->pos = this->pos - delta;
	this->entity->model.translate(-delta.x, -delta.y, -delta.z);
}
