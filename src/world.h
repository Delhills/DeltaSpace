
#ifndef WORLD_H
#define WORLD_H
#include "includes.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "entities.h"
#include "player.h"

extern float mouse_speed;

class World
{
public:

	static World* instance;

	EntityMesh root;

	Player player;
	bool freecam;
	EntityMesh* map[mapSize];
	EntityMesh* players[mapSize];
	Camera* camera; //our global camera
	World();
	void renderMap();
	void render();
	void update(double seconds_elapsed);
	//void clear();
	//void addEntity(Entity* entity);

	//bool load(const char* filename);
	//Entity* createEntity(std::string type);

};

#endif