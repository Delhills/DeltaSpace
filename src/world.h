
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
	bool done;
	bool alive;
	float ground_timer;
	float timer;
	int currentLaps;
	int totalLaps;
	EntityMesh* goal;
	std::vector<EntityMesh*> map;
	std::vector<Obstacle*> obstacles;
	EntityMesh* sky;
	EntityMesh* puntito;
	Camera* camera; //our global camera
	

	World(const char* filename,Camera* camera);
	void renderMap();
	void renderGUI(float x, float y, float w, float h, bool flip, Texture* texture, Vector4 range);
	void render();
	void update(double seconds_elapsed);
	void addObstacleMouse(eObstacleType type);
	void renderObstacles();
	bool checkCol(EntityMesh* obstacle, Vector3 playerPos);
	bool onGround();
	void RenderMinimap();
	void SendFlying();
	void Restart();
	void onObstacle(eObstacleType type);
	bool loadMap(const char* filename);
	void addMap(Vector4 type, int pos);
	void addPart(Mesh* mesh, int pos);
	void addObstacle(std::string type,Vector3 pos,Vector3 normal);

};

#endif