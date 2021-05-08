#ifndef WORLD_H
#define WORLD_H
#include "includes.h"
#include "game.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"


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
	//virtual void update(float elapsed_time);

	//Vector3 getPosition();
};


class EntityMesh : public Entity {
public:
	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	Vector4 color;

	void render();
	//void update(float dt);
};

class World
{
public: 

	static World* instance;

	World();
	std::vector<Entity*> entities;

	//void clear();
	//void addEntity(Entity* entity);

	//bool load(const char* filename);
	//Entity* createEntity(std::string type);

};

#endif