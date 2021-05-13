
#ifndef WORLD_H
#define WORLD_H
#include "includes.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"
extern float mouse_speed;

enum eEntityType {
	NONE = 0,
	ENTITY_MESH = 1,
};

class Entity {
public:
	Entity();
	//virtual ~Entity(); //destructor

	eEntityType entity_type;
	Matrix44 model;
	std::string name;

	virtual void render() {};
	virtual void update(float elapsed_time) {};

	Entity* parent;

	//pointers to my children
	std::vector<Entity*> children;

	//methods
	void addChild(Entity* ent);
	void removeChild(Entity* ent);

	virtual void configure() {}


	//Vector3 getPosition();
};


class EntityMesh : public Entity {
public:
	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	Vector4 color;

	void render();
	void update(float dt);
};

class World
{
public:

	static World* instance;

	World();
	EntityMesh* islas[2];;
	Camera* camera; //our global camera
	void render();
	void renderIslands();
	void update(double seconds_elapsed);
	//void clear();
	//void addEntity(Entity* entity);

	//bool load(const char* filename);
	//Entity* createEntity(std::string type);

};

#endif