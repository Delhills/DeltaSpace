#include "world.h"
#include "game.h"
#include "input.h"
#include "fbo.h"


float mouse_speed = 20.0f;

World* World::instance = NULL;

World::World() {
	instance = this;
	freecam = false;

	Texture* texture = Texture::Get("data/export.png");
	Mesh* mesh = Mesh::Get("data/export.obj");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	Vector4 color = Vector4(1, 1, 1, 1);
	int offset = 40;

	Vector3 padding = mesh->box.halfsize;

	for (int i = 0; i < mapSize; i++)
	{
		map[i] = new EntityMesh(mesh, texture, shader, color);
		map[i]->model.setTranslation(0, 0, i * (-padding.z - offset));
	}

	texture = Texture::Get("data/barra2.png");
	mesh = Mesh::Get("data/barra2.obj");

	for (int i = 0; i < mapSize * 2; i = i + 2)
	{
		bars[i] = new EntityMesh(mesh, texture, shader, color);
		bars[i + 1] = new EntityMesh(mesh, texture, shader, color);

		Vector3 pos = map[i / 2]->model.getTranslation();


		bars[i]->model.setTranslation(pos.x, pos.y, pos.z);
		bars[i]->model.setTranslation(-padding.x / 2 + 10, 0, i * (-padding.z) + 5);
		bars[i + 1]->model.setTranslation(padding.x / 2 - 10, 0, i * (-padding.z) + 5);
		map[i / 2]->addChild(bars[i]);
		map[i / 2]->addChild(bars[i + 1]);
	}

	player = Player();
	Vector3 paddingPlayer = player.entity->mesh->box.halfsize;
	player.entity->model.setTranslation(0, paddingPlayer.y/10, 0);

	texture = Texture::Get("data/SunsetSky.png");
	mesh = Mesh::Get("data/sphere.obj");
	sky = new EntityMesh(mesh, texture, shader, color);


	//create our camera
	this->camera = new Camera();
	camera->lookAt(Vector3(0.f, 100.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, Game::instance->window_width / (float)Game::instance->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
}

void World::render() {
	camera->enable();

	Matrix44 playerModel;
	playerModel.translate(player.pos.x, player.pos.y, player.pos.z);

	
	//islas[0]->render();
	renderMap();
	renderObstacles();
	player.entity->render(playerModel);


	if (!freecam) {

		Vector3 eye = playerModel * Vector3(0.0f, 6.0f, 10.0f);
		Vector3 center = playerModel * Vector3(0.0f, 0.0f, -2.0f);
		Vector3 up = Vector3(0.0f, 1.0f, 0.0f);

		camera->lookAt(eye, center, up);
	}

	Matrix44 skyModel;
	//skyModel.translate(camera->eye.x,camera->eye.y,camera->eye.z);
	sky->render(skyModel);
}

void World::renderMap() {
	//Matrix44 model = islas[0]->model;

	for (size_t i = 0; i < mapSize; i++)
	{
		map[i]->render();
	}

}

void World::renderObstacles() {
	//Matrix44 model = islas[0]->model;

	for (size_t i = 0; i < Obstacles.size(); i++)
	{
		Obstacles.at(i)->render();
	}

}

void World::addObstacle() {

	Vector3 orgin = camera->eye;
	int width = Game::instance->window_width;
	int height = Game::instance->window_height;

	Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, width, height);


	Vector3 up = Vector3(0, 1, 0);
	Vector3 pos = RayPlaneCollision(Vector3(),up,orgin,dir);


	Mesh* mesh = Mesh::Get("data/obstacle.obj");

	Texture* texture = Texture::Get("data/obstacle.png");
	Shader* shader=Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	Vector4 color = Vector4(1, 1, 1, 1);

	
	
	//for (size_t i = 0; i < Obstacles.size(); i++)
	//{
	//	if (Obstacles.at(i)->model.ra(orgin.v, dir.v, true, 0, 100) == true) {
	//		
	//	}
	//
	//}
	
	EntityMesh* obstacle = new EntityMesh(mesh, texture, shader, color);
	obstacle->model.setTranslation(pos.x, pos.y, pos.z);
	Obstacles.push_back(obstacle);

	
}

void World::update(double seconds_elapsed) {

	bool mouse_locked = Game::instance->mouse_locked;
	float speed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant
	

		//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
	}

	//async input to move the camera around
	if (Input::wasKeyPressed(SDL_SCANCODE_1)) freecam = !freecam; //move faster with left shift


	if (freecam)
	{
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
		if (Input::wasKeyPressed(SDL_SCANCODE_C)) addObstacle();
	}
	else
	{
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) player.accelerate(speed);
		else if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) player.accelerate(-speed);

		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) player.turn(speed);
		else if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) player.turn(-speed);
		else player.turn(-player.speed.x);

		player.pos = player.pos - player.speed;
	}

	
	

	std::cout << mouse_speed << "\n";

	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();


}