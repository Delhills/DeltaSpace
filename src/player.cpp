#include "player.h"

Player::Player()
{
	Texture* texture = Texture::Get("data/textures/player2.png");
	Mesh* mesh = Mesh::Get("data/meshes/player.obj");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	Vector4 color = Vector4(1, 1, 1, 1);

	entity = new EntityMesh(mesh, texture, shader, color);
	pos = Vector3(0, 0, 0);
	//rotation = entity->model.getRotationOnly();
	rot = 0.0f;
	this->speed = Vector3(0, 0, 0);
	this->normal = Vector3(0,1,0);
}

void Player::Render() {
	this->entity->render(this->entity->model);
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
	//float finalspeed = lerp(this->speed.x, this->speed.x + delta, 0.5);
	//this->speed.x = clamp(finalspeed, -this->speed.z* turn_speed, this->speed.z* turn_speed);
	
	this->rot += delta;

	if (this->rot > 360) {
		this->rot -= 360;
	}

	if (this->rot < -360) {
		this->rot += 360;
	}
	//Vector3 coll;
	
	//RaySphereCollision(Vector3(0.0, 0.0, this->pos.z), 14, this->pos, (this->entity->model.topVector() * -1), coll);
	//std::cout << coll.x << ", " << coll.y << ", " << coll.z << ", " << "\n";
	//this->pos = coll;
	//this->pos.x = clamp(this->pos.x, -9.2, 9.85);
}