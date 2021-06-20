
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
constexpr auto mapSize = 20;
//constexpr auto mapSize = 1;

class World
{
public:

	static World* instance;

	EntityMesh root;

	Player player;
	bool freecam;
	bool done;
	bool alive;
	EntityMesh* goal;
	EntityMesh* map[mapSize];
	std::vector<Obstacle*> obstacles;
	EntityMesh* sky;
	EntityMesh* puntito;
	Camera* camera; //our global camera
	World();
	void renderMap();
	void renderGUI(float x, float y, float w, float h, bool flip, Texture* texture, Vector4 range);
	void render();
	void update(double seconds_elapsed);
	void addObstacle(eObstacleType type);
	void renderObstacles();
	bool checkCol(EntityMesh* obstacle, Vector3 playerPos);
	void ComputePos();
	void RenderMinimap();
	//void clear();
	//void addEntity(Entity* entity);

	//bool load(const char* filename);
	//Entity* createEntity(std::string type);

};

#endif