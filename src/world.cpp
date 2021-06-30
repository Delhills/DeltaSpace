#include "world.h"
#include "game.h"
#include "input.h"
#include "fbo.h"
#include <iostream>
#include <fstream>
#include <string>
#include "audio.h"

float mouse_speed = 10.0f;

World* World::instance = NULL;

World::World(const char* filename,Camera* w_camera, GUI* gui, const char* textureFile) 
{
	countDown = 2.99;
	timer = 0;
	instance = this;
	freecam = false;
	done = false;
	alive = true;
	pause = false;
	nextLevel = false;
	goToMenu = false;
	this->textureFile = textureFile;
	ground_timer = 0;
	currentLaps = 1;
	totalLaps = 3;
	this->gui = gui;
	buttonPressed = eButton::NO_BUTTON;
	
	loadMap(filename);

	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/goal.fs");
	Vector4 color = Vector4(1, 1, 1, 1);
	Texture* texture = Texture::getWhiteTexture();
	Mesh* mesh = Mesh::Get("data/meshes/goal.obj");
	Goal* goal = new Goal(mesh, texture, shader, color);

	EntityMesh* lastMap = map[map.size()-1];
	float position = lastMap->model.getTranslation().z;
	float offset = -1*lastMap->mesh->box.halfsize.z;


	goal->model.setTranslation(0, 0, position + offset);
	
	this->goal = goal;


	player = Player();
	Vector3 paddingPlayer = player.entity->mesh->box.halfsize;
	player.entity->model.setTranslation(0, -13, 0);

	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	texture = Texture::Get("data/textures/skybox.PNG");
	mesh = Mesh::Get("data/meshes/esfera.obj");
	sky = new EntityMesh(mesh, texture, shader, color);

	mesh = Mesh::Get("data/meshes/obstacle.obj");
	texture = Texture::Get("data/textures/obstacle.png");

	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/top.fs");

	puntito = new EntityMesh(mesh, texture, shader, color);

	//create our camera
	this->camera = w_camera;
	camera->lookAt(Vector3(0.f, 100.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, Game::instance->window_width / (float)Game::instance->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
	
}

void World::render() 
{
	//set the clear color (the background color)

	int windowHeight = Game::instance->window_height;
	int windowWidth = Game::instance->window_width;

	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//set flags
	glDisable(GL_BLEND);

	glDisable(GL_CULL_FACE);


	camera->enable();

	glDisable(GL_DEPTH_TEST);
	Matrix44 skyModel;
	skyModel.translate(camera->eye.x, camera->eye.y, camera->eye.z);
	sky->render(skyModel);
	glEnable(GL_DEPTH_TEST);



	if (this->pause) {
		drawText((windowWidth / 2) - 120, (windowHeight / 4), "Pausa ", Vector3(1, 1, 0), 10);
		drawText((windowWidth / 2) - 280, (4 * windowHeight / 8) - 20, "PULSA ESC PARA CONTINUAR", Vector3(1, 1, 1), 4);
		gui->RenderGui();
	}
	else {

		renderObstacles();
		goal->render(currentLaps);
		Matrix44 playerModel = player.entity->model;
		if (ground_timer < 1.0) {
			renderMap();
		}
		if (!freecam) {

			Vector3 newEye = playerModel * Vector3(0.0f, 7.0f, 10.0f);
			newEye.x = lerp(camera->eye.x, newEye.x, 2 * Game::instance->elapsed_time);
			newEye.y = lerp(camera->eye.y, newEye.y, 2 * Game::instance->elapsed_time);

			Vector3 center = playerModel * Vector3(0.0f, 0.0f, -5.0f);
			Vector3 up = playerModel.topVector();
			camera->lookAt(newEye, center, up);
		}
		player.entity->model = playerModel;
		player.Render();

		drawText(5, windowHeight - 25, std::to_string((int)floor(player.speed.z * 100)) + " Km/h", Vector3(1, 1, 1), 3);
		drawText(windowWidth - 75, windowHeight - 25, std::to_string(currentLaps) + "/" + std::to_string(totalLaps) , Vector3(1, 1, 1), 3);
		if (done) 
		{ 
			std::string min_srt,sec_str;

			drawText(windowWidth / 2 - 300, windowHeight / 4, "Has ganado", Vector3(1, 1, 0), 10);

			int minutes = (int)(timer / 60);
			float seconds = timer - minutes * 60;
			min_srt = std::to_string(minutes);
			if (seconds < 10) sec_str = "0";
			sec_str += std::to_string(seconds);

			drawText(windowWidth / 2 - 300, windowHeight / 4 + 80 , min_srt + ":" + sec_str, Vector3(1, 1, 1), 0.5*sin(Game::instance->time * PI)+3);
			gui->RenderGui();
		}
		else if (!alive) {
			drawText(windowWidth / 2 - 300, windowHeight / 4, "Has perdido", Vector3(1, 1, 0), 10);
			gui->RenderGui();
		}

	 
		//Draw the floor grid
		//drawGrid();

		RenderMinimap();
	}



	if (countDown > 0.0)
	{
		drawText(windowWidth / 2 - 15, windowHeight / 2 - 15, std::to_string(((int)this->countDown)+1), Vector3(1, 1, 1), 10);
	}

	//renderGUI(100, 100, 100, 100, false, Texture::Get("data/gui/atlasGUI.png"), Vector4(0.003, 0.118, 0.0995, 0.1015));
	
	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(Game::instance->window);
}

void World::renderMap() 
{

	for (size_t i = 0; i < map.size(); i++)
	{
		map[i]->render();
	}

}

void World::renderObstacles() 
{

	for (size_t i = 0; i < obstacles.size(); i++)
	{
		obstacles[i]->render();
	}

}

bool World::checkCol(EntityMesh* obstacle, Vector3 playerPos)
{
	Vector3 characterCenter = playerPos + Vector3(0, 1, 0);

	Vector3 coll;
	Vector3 collnorm;

	return (obstacle->mesh->testSphereCollision(obstacle->model, characterCenter, 1, coll, collnorm));

}

void World::addObstacleMouse(eObstacleType type) 
{

	Vector3 origin = camera->eye;
	int width = Game::instance->window_width;
	int height = Game::instance->window_height;

	Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, width, height);


	Vector3 coll, normal, pos;
	for (size_t i = 0; i < map.size(); i++)
	{
		if (map[i]->mesh->testRayCollision(map[i]->model, origin, dir,
			coll, normal)) {
			
			pos = coll + Vector3(0.7, 0.7, 0.7) * normal;
		}
		
	}

	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	std::string type_str;
	if (type == BAD) {

		mesh = Mesh::Get("data/meshes/obstacle.obj");

		texture = Texture::Get("data/textures/obstacle.png");

		shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

		type_str = "bad";
	}
	else if (type == GOOD) {

		mesh = Mesh::Get("data/meshes/good.obj");

		texture = Texture::getWhiteTexture();

		shader = Shader::Get("data/shaders/basic.vs", "data/shaders/turbo.fs");

		type_str = "good";
	}


	Vector4 color = Vector4(1, 1, 1, 1);

	
	std::cout << "{" << "\n";
	std::cout << " \"type\" " << ":"  <<" \""<< type_str  << "\" " << "," << "\n";
	std::cout << " \"position\" " << ": [" << pos.x << ", " << pos.y << ", " << pos.z <<"] ," << "\n";
	std::cout << " \"orientation\" " << ": [" << normal.x << ", " << normal.y << ", " << normal.z << "] " << "\n";
	std::cout << "}," << "\n";
	Obstacle* obstacle = new Obstacle(mesh, texture, shader, color, type);
	
	obstacle->model.setTranslation(pos.x, pos.y, pos.z);
	obstacle->model.setUpAndOrthonormalize(normal);
	obstacles.push_back(obstacle);

}

void World::update(double seconds_elapsed) 
{

	if (this->countDown >= 0.0)
	{	
		this->countDown = this->countDown - 1 * seconds_elapsed;
		return;
	}

	if (goToMenu)
	{
		return;
	}
	bool mouse_locked = Game::instance->mouse_locked;
	float camSpeed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant
	

	if (Input::wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
		if (this->pause) Audio::UnPause;
		else Audio::Pause;
		this->pause = !this->pause;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_3)) this->done = !this->done;
	


	if (Input::wasMousePressed(1))
	{
		buttonPressed = gui->buttonPressed;
	}
	if (this->pause || !alive || done) {
		switch (buttonPressed)
		{
		case PAUSE_REPLAY:
			Restart();
			break;
		case PAUSE_NEXT:
			this->nextLevel = true;
			break;
		case PAUSE_EXIT:
			//Game::instance->must_exit = true;
			this->goToMenu = true;
			this->goToMenu = true;
			break;
		}
		buttonPressed = NO_BUTTON;
		if (this->pause)
		{
			return;
		}
	}


	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
	}

		

	if (!onGround()) {
		
		ground_timer += seconds_elapsed;

		if (ground_timer > 0.15) {

			this->alive = false;
		}
	}
	else {
		ground_timer = 0;
		timer += seconds_elapsed;
	}

	if (done && ground_timer > 0.15) { 
		SendFlying();
		return;
	}

	if (!alive) {
		SendFlying();
		return;
	}
	
	if (player.max_speed < 100) {
		player.max_speed += seconds_elapsed;
	}
	else {
		player.max_speed += seconds_elapsed*0.3;
	}
	if (freecam)
	{
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) camSpeed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * camSpeed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * camSpeed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * camSpeed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * camSpeed);
		if (Input::wasKeyPressed(SDL_SCANCODE_C)) addObstacleMouse(BAD);
		if (Input::wasKeyPressed(SDL_SCANCODE_V)) addObstacleMouse(GOOD);
	}
	else
	{
		if (player.speed.z > player.max_speed) { 
			//Si tienes turbo no puedes acelerar ni frenar
			player.speed.z -= seconds_elapsed * 100;
		}
		else
		{
			if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) player.accelerate(60*seconds_elapsed);
			else if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) player.accelerate(-45.0 * seconds_elapsed);
			else {
				player.accelerate(-lerp(player.speed.z, 0.0, 0.7)*seconds_elapsed);
			}
		}


		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) player.turn(seconds_elapsed);
		else if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) player.turn(-seconds_elapsed);
		


		player.entity->model.translate(0, 0, -player.speed.z * seconds_elapsed);
		
		player.pos = player.entity->model.getTranslation();

	}

	

	for (size_t i = 0; i < obstacles.size(); i++)
	{
		if (checkCol(obstacles[i], player.pos)) {

			onObstacle(obstacles[i]->type);

		}
	}

	if (hasWon()) {
		std::cout << "ole muy bien!" << "\n";
		if (currentLaps < totalLaps) 
		{
			currentLaps++;
			player.min_speed += 15;
			player.speed.z += 15;
			player.turbo_coef += 15;
			player.entity->model.setTranslation(0, -13, 0);
			done = false;
			alive = true;
		}
		else done = true;
	}


	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();

}

void World::onObstacle(eObstacleType type) 
{

	switch (type)
	{
	case BAD:
		Audio::Play(Songs[COLISION]);
		player.speed.z = -player.speed.z;
		break;

	case GOOD:
		Audio::Play(Songs[BOOST]);
		player.speed.z = player.turbo_coef;
		break;

	default:
		break;
	}

}

void World::SendFlying() 
{

	Vector3 top = -1 * player.entity->model.topVector();
	player.entity->model.translate(top.x, top.y, top.z);
	player.entity->model.rotate(DEG2RAD / 4, Vector3(1, 1, 1));

}

bool World::hasWon()
{
	return alive && player.pos.z < goal->model.getTranslation().z;
}

bool World::onGround() 
{

	Vector3 coll, normal;
	bool onGround = false;
	for (size_t i = 0; i < map.size(); i++)
	{
		
		if (map[i]->mesh->testRayCollision(map[i]->model, Vector3(0.0, 0.0, player.pos.z), -1 * player.entity->model.topVector(),
			coll, normal)) {

			onGround = true;

		}
	}

	return onGround;

}

void World::RenderMinimap() 
{

	glViewport(Game::instance->window_width - 200, Game::instance->window_height - 200, 200, 200);
	Camera mapCam;
	mapCam.setPerspective(60, 1, 0, 1000);


	Vector3 eye = Vector3(player.pos.x, player.pos.y + 100, player.pos.z);
	Vector3 center = player.pos;
	Vector3 up = -1 * player.entity->model.frontVector();

	mapCam.lookAt(eye, center, up);

	mapCam.enable();
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	renderMap();


	Matrix44 puntitoModel;
	puntitoModel.setTranslation(player.pos.x, player.pos.y, player.pos.z);

	puntitoModel.rotate(90 * DEG2RAD, Vector3(1, 0, 0));

	this->puntito->render(puntitoModel);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glViewport(0, 0, Game::instance->window_width, Game::instance->window_height);
}

void World::Restart() 
{
	camera->lookAt(Vector3(0.f, 100.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	player.entity->model.setTranslation(0, -13, 0);
	player.speed = Vector3(0, 0, 0);
	player.max_speed = 70;
	player.min_speed = -70;
	done = false;
	alive = true;
	pause = false;
	goToMenu = false;
	ground_timer = 0.0;
	timer = 0.0;
	currentLaps = 1;
	countDown = 2.99;
}

bool World::loadMap(const char* filename) 
{

	std::string content;

	std::cout << " + Reading level : " << filename << "..." << std::endl;
	if (!readFile(filename, content))
	{
		std::cout << "- ERROR: file not found: " << filename << std::endl;
		return false;
	}
	//parse json string
	cJSON* json = cJSON_Parse(content.c_str());
	if (!json)
	{
		std::cout << "ERROR: JSON has errors: " << filename << std::endl;
		return false;
	}

	//entities
	cJSON* obstacles_json = cJSON_GetObjectItemCaseSensitive(json, "obstacles");
	cJSON* obstacle_json;

	cJSON_ArrayForEach(obstacle_json, obstacles_json)
	{
		std::string type_obs = cJSON_GetObjectItem(obstacle_json, "type")->valuestring;
		Vector3 pos = readJSONVector3(obstacle_json, "position", Vector3());
		Vector3 normal = readJSONVector3(obstacle_json, "orientation", Vector3());
		addObstacle(type_obs, pos, normal);
	}

	cJSON* map_json = cJSON_GetObjectItemCaseSensitive(json, "map");
	cJSON* part_json;

	int map_counter = 0;
	cJSON_ArrayForEach(part_json, map_json)
	{
		Vector4 type = readJSONVector4(part_json, "type");
		addMap(type, map_counter);
		map_counter++;
	}

}

void World::addMap(Vector4 type, int pos)
{
	Mesh* mesh;
	if (type.x == 1) {
		mesh = Mesh::Get("data/meshes/tubo_down.obj");
		addPart(mesh, pos);
	}
	if (type.y == 1) {
		mesh = Mesh::Get("data/meshes/tubo_right.obj");
		addPart(mesh, pos);
	}
	if (type.z == 1) {
		mesh = Mesh::Get("data/meshes/tubo_up.obj");
		addPart(mesh, pos);
	}
	if (type.w == 1) {
		mesh = Mesh::Get("data/meshes/tubo_left.obj");
		addPart(mesh, pos);
	}
}

void World::addPart(Mesh* mesh, int pos) 
{

	Texture* texture = Texture::Get(textureFile);
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	Vector4 color = Vector4(1, 1, 1, 1);
	EntityMesh* map_part = new EntityMesh(mesh, texture, shader, color);

	map.push_back(map_part);

	float offset = -2;
	float transation = offset * map_part->mesh->box.halfsize.z;

	Vector3 position = Vector3(0, 0, transation * pos);
	map[map.size() - 1]->model.setTranslation(position.x, position.y, position.z);

}

void World::addObstacle(std::string type,Vector3 pos,Vector3 normal)
{
	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	eObstacleType cType;
	Vector4 color = Vector4(1, 1, 1, 1);

	if (type == "bad") {

		cType = BAD;

		mesh = Mesh::Get("data/meshes/obstacle.obj");

		texture = Texture::Get("data/textures/obstacle.png");

		shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	}
	else if (type == "good") {

		cType = GOOD;

		mesh = Mesh::Get("data/meshes/good.obj");

		texture = Texture::getWhiteTexture();

		shader = Shader::Get("data/shaders/basic.vs", "data/shaders/turbo.fs");

	}
	
	Obstacle* obstacle = new Obstacle(mesh, texture, shader, color, cType);
	obstacle->model.setTranslation(pos.x, pos.y, pos.z);
	obstacle->model.setUpAndOrthonormalize(normal);
	obstacles.push_back(obstacle);
	
}