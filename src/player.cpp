#include "player.h"

Player::Player()
{
	Texture* texture = Texture::Get("data/textures/player2.png");
	Mesh* mesh = Mesh::Get("data/meshes/player.obj");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	Vector4 color = Vector4(1, 1, 1, 1);

	entity = new EntityMesh(mesh, texture, shader, color);
	pos = Vector3(0, 0, 0);
	
	this->speed = Vector3(0, 0, 0);
	this->turn_speed_coef = -50;
	this->speed_coef = 10;
	this->max_speed = 100;
	this->min_speed = 20;
	this->turbo_coef = 200;

}

void Player::Render() {
	this->entity->render(this->entity->model);
}

void Player::accelerate(float dir)
{
	float vel = dir * speed_coef;
	float finalspeed = lerp(this->speed.z, this->speed.z + vel, 0.08);
	this->speed.z = clamp(finalspeed, min_speed, max_speed);
}

void Player::turn(float seconds_elapsed)
{
	float vel = seconds_elapsed * turn_speed_coef;
	entity->model.rotateGlobal(vel * DEG2RAD, Vector3(0, 0, -1));
}
