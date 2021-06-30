
#ifndef WORLD_H
#define WORLD_H
#include "includes.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
#include "entities.h"
#include "player.h"
#include "GUI.h"

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
	bool pause;
	bool nextLevel;
	bool goToMenu;
	float ground_timer;
	float timer;
	int currentLaps;
	float countDown;
	int totalLaps;
	Goal* goal;
	std::vector<EntityMesh*> map;
	std::vector<Obstacle*> obstacles;
	EntityMesh* sky;
	EntityMesh* puntito;
	Camera* camera; //our global camera
	GUI* gui;
	eButton buttonPressed;
	const char* textureFile;
	

	World(const char* filename,Camera* camera, GUI* gui, const char* textureFile);
	void renderMap();
	void render();
	void update(double seconds_elapsed);
	void addObstacleMouse(eObstacleType type);
	void renderObstacles();
	bool checkCol(EntityMesh* obstacle, Vector3 playerPos);
	bool onGround();
	void RenderMinimap();
	void SendFlying();
	bool hasWon();
	void Restart();
	void onObstacle(eObstacleType type);
	bool loadMap(const char* filename);
	void addMap(Vector4 type, int pos);
	void addPart(Mesh* mesh, int pos);
	void addObstacle(std::string type,Vector3 pos,Vector3 normal);

};

#endif