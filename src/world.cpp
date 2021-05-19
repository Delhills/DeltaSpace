#include "world.h"
#include "game.h"
#include "input.h"


float mouse_speed = 100.0f;

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

	player = Player();
	Vector3 paddingPlayer = player.entity->mesh->box.halfsize;
	player.entity->model.setTranslation(0, paddingPlayer.y/10, 0);


	//create our camera
	this->camera = new Camera();
	camera->lookAt(Vector3(0.f, 100.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, Game::instance->window_width / (float)Game::instance->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
}

void World::render() {
	camera->enable();

	//islas[0]->render();
	renderMap();
	player.entity->render();

	Vector3 eye = player.entity->model * Vector3(0.0f, 6.0f, 10.0f);
	Vector3 center = player.entity->model * Vector3(0.0f, 0.0f, -2.0f);
	Vector3 up = Vector3(0.0f, 1.0f, 0.0f);

	camera->lookAt(eye, center, up);

	
}

void World::renderMap() {
	//Matrix44 model = islas[0]->model;

	for (size_t i = 0; i < mapSize; i++)
	{
		map[i]->render();
	}

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
	if (Input::isKeyPressed(SDL_SCANCODE_1)) freecam = !freecam; //move faster with left shift


	if (freecam)
	{
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	}
	else
	{
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) player.move(Vector3(0.0f, 0.0f, 1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) player.move(Vector3(0.0f, 0.0f, -1.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) player.move(Vector3(1.0f, 0.0f, 0.0f) * speed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) player.move(Vector3(-1.0f, 0.0f, 0.0f) * speed);
	}

	std::cout << mouse_speed << "\n";

	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();


}