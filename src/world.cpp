#include "world.h"
#include "game.h"
#include "input.h"
#include "fbo.h"

float mouse_speed = 10.0f;

World* World::instance = NULL;

World::World() {

	instance = this;
	freecam = false;
	done = false;
	alive = true;
	ground_timer = 0;
	Texture* texture = Texture::Get("data/textures/export.png");
	Mesh* mesh = Mesh::Get("data/meshes/tubo.obj");

	Mesh* mesh_up = Mesh::Get("data/meshes/tubo_up.obj");
	Mesh* mesh_down = Mesh::Get("data/meshes/tubo_down.obj");
	Mesh* mesh_left = Mesh::Get("data/meshes/tubo_left.obj");
	Mesh* mesh_right = Mesh::Get("data/meshes/tubo_right.obj");

	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	Vector4 color = Vector4(1, 1, 1, 1);
	int offset = 40;

	Vector3 padding = mesh->box.halfsize;

	for (int i = 0; i < mapSize; i++)
	{
		if (i <= 5) {
			map[i] = new EntityMesh(mesh_down, texture, shader, color);
			
		}
		if(i>5 & i<= 10){
			map[i] = new EntityMesh(mesh, texture, shader, color);
		}
		if (i > 10 & i <= 15) {
			map[i] = new EntityMesh(mesh_left, texture, shader, color);
		}
		if (i>15) {
			map[i] = new EntityMesh(mesh_down, texture, shader, color);
		}
		map[i]->model.setTranslation(0, 0, i * (-padding.z - offset) - 10);
	}


	texture = Texture::Get("data/textures/meta.png");
	mesh = Mesh::Get("data/meshes/meta.obj");
	EntityMesh* goal = new EntityMesh(mesh, texture, shader, color);
	goal->model.setTranslation(0, 0, mapSize * (-padding.z - offset) + 30);
	this->goal = goal;


	player = Player();
	Vector3 paddingPlayer = player.entity->mesh->box.halfsize;
	//player.entity->model.setTranslation(0, paddingPlayer.y/10, 0);
	player.entity->model.setTranslation(0, -13, 0);

	texture = Texture::Get("data/textures/SkySkybox.png");
	mesh = Mesh::Get("data/meshes/esfera.obj");
	sky = new EntityMesh(mesh, texture, shader, color);

	mesh = Mesh::Get("data/meshes/obstacle.obj");
	texture = Texture::Get("data/textures/obstacle.png");

	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/top.fs");

	puntito = new EntityMesh(mesh, texture, shader, color);

	//create our camera
	this->camera = new Camera();
	camera->lookAt(Vector3(0.f, 100.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, Game::instance->window_width / (float)Game::instance->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
}

void World::render() {
	//set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//set flags
	glDisable(GL_BLEND);

	glDisable(GL_CULL_FACE);


	camera->enable();

	Matrix44 playerModel = player.entity->model;

	glDisable(GL_DEPTH_TEST);
	Matrix44 skyModel;
	skyModel.translate(camera->eye.x, camera->eye.y, camera->eye.z);
	sky->render(skyModel);
	glEnable(GL_DEPTH_TEST);


	renderMap();
	renderObstacles();
	player.entity->model = playerModel;
	player.Render();
	goal->render();


	if (!freecam) {

		Vector3 newEye = playerModel * Vector3(0.0f, 7.0f, 10.0f);
		newEye.x = lerp(camera->eye.x, newEye.x, 2*Game::instance->elapsed_time);
		newEye.y = lerp(camera->eye.y, newEye.y, 2*Game::instance->elapsed_time);

		Vector3 center = playerModel * Vector3(0.0f, 0.0f, -5.0f);
		Vector3 up = playerModel.topVector();
		camera->lookAt(newEye, center, up);
	}


	drawText(5, Game::instance->window_height - 25, std::to_string(player.speed.z * 100) + " Km/h", Vector3(1, 1, 1), 3);
	if (done) drawText(Game::instance->window_width / 2 - 300, Game::instance->window_height / 2 - 20, "Has ganado ", Vector3(1, 1, 0), 10);


	//Draw the floor grid
	drawGrid();

	RenderMinimap();
	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	renderGUI(100, 100, 100, 100, false, Texture::Get("data/gui/atlasGUI.png"), Vector4(0.003, 0.118, 0.0995, 0.1015));
	
	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(Game::instance->window);
}

void World::renderMap() {

	for (size_t i = 0; i < mapSize; i++)
	{
		map[i]->render();
	}

}

void World::renderGUI(float x, float y, float w, float h, bool flip, Texture* texture, Vector4 range) {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int width = Game::instance->window_width;
	int height = Game::instance->window_height;
	Camera cameraGUI;
	cameraGUI.setOrthographic(0, width, height, 0, -1, 1);
	cameraGUI.enable();


	Mesh quad;
	quad.createQuad(x, y, w, h, flip);

	Vector2 mousepos = Input::mouse_position;
	float halfW = w * 0.5;
	float halfH = h * 0.5;
	float min_x = x - halfW;
	float min_y = y - halfH;
	float max_x = x + halfW;
	float max_y = y + halfH;

	bool hover = mousepos.x > min_x && mousepos.y > min_y && mousepos.x < max_x && mousepos.y < max_y;

	if (hover && Input::isMousePressed(1)) Restart();
	

	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/gui.fs");

	shader->enable();

	shader->setUniform("u_texture_tiling", 1.0f);
	Matrix44 quadModel;
	shader->setUniform("u_model", quadModel);
	shader->setUniform("u_viewprojection", cameraGUI.viewprojection_matrix);
	shader->setTexture("u_texture", texture, 0);
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_tex_range", hover ? range + Vector4(0.1035, 0, 0, 0) : range);

	quad.render(GL_TRIANGLES);

	shader->disable();


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void World::renderObstacles() {

	for (size_t i = 0; i < obstacles.size(); i++)
	{
		obstacles.at(i)->render();
	}

}

bool World::checkCol(EntityMesh* obstacle, Vector3 playerPos)
{
	Vector3 characterCenter = playerPos + Vector3(0, 1, 0);

	Vector3 coll;
	Vector3 collnorm;

	return (obstacle->mesh->testSphereCollision(obstacle->model, characterCenter, 1, coll, collnorm));

}

void World::addObstacle(eObstacleType type) {

	Vector3 origin = camera->eye;
	int width = Game::instance->window_width;
	int height = Game::instance->window_height;

	Vector3 dir = camera->getRayDirection(Input::mouse_position.x, Input::mouse_position.y, width, height);


	Vector3 coll, normal, pos;
	for (size_t i = 0; i < mapSize; i++)
	{
		if (map[i]->mesh->testRayCollision(map[i]->model, origin, dir,
			coll, normal)) {
			
			pos = coll + Vector3(0.6, 0.6, 0.6) * normal;
		}
		
	}

	Mesh* mesh;
	Texture* texture;
	Shader* shader;
	if (type == BAD) {

		mesh = Mesh::Get("data/meshes/obstacle.obj");

		texture = Texture::Get("data/textures/obstacle.png");

		shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");

	}
	else if (type == GOOD) {

		mesh = Mesh::Get("data/meshes/good.obj");

		texture = Texture::getWhiteTexture();

		shader = Shader::Get("data/shaders/basic.vs", "data/shaders/turbo.fs");

	}


	Vector4 color = Vector4(1, 1, 1, 1);


	Obstacle* obstacle = new Obstacle(mesh, texture, shader, color, type);
	obstacle->model.setTranslation(pos.x, pos.y, pos.z);
	obstacle->model.setUpAndOrthonormalize(normal);
	obstacles.push_back(obstacle);

}

void World::update(double seconds_elapsed) {

	bool mouse_locked = Game::instance->mouse_locked;
	float camSpeed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant

	
	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
	}

	//async input to move the camera around
	if (Input::wasKeyPressed(SDL_SCANCODE_1)) freecam = !freecam; //move faster with left shift
	if (Input::wasKeyPressed(SDL_SCANCODE_0)) Restart();
		

	if (!onGround()) {

		ground_timer += seconds_elapsed;

		if (ground_timer > 0.15) {

			this->alive = false;
		}
	}
	else {
		ground_timer = 0;
	}

	if (done && ground_timer > 0.15) { 
		SendFlying();
		return;
	}

	if (!alive) {
		SendFlying();
		return;
	}

	if (freecam)
	{
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) camSpeed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * camSpeed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * camSpeed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * camSpeed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * camSpeed);
		if (Input::wasKeyPressed(SDL_SCANCODE_C)) addObstacle(BAD);
		if (Input::wasKeyPressed(SDL_SCANCODE_V)) addObstacle(GOOD);
	}
	else
	{
		if (player.speed.z > player.max_speed) { 
			//Si tienes tubo no puedes acelerar ni frenar
			player.speed.z -= seconds_elapsed * 100;
		}
		else
		{
			if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) player.accelerate(1.0);
			else if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) player.accelerate(-1.0);
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

	if (checkCol(goal, player.pos)) {
		std::cout << "ole muy bien!" << "\n";
		done = true;
	}


	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();

}

void World::onObstacle(eObstacleType type) {

	switch (type)
	{
	case BAD:
		if (player.speed.z > 20) {
			player.speed.z = player.speed.z / 2;
		}
		else {
			player.speed.z = 20;
		}
		break;

	case GOOD:
		std::cout << "nice" << "\n";
		player.speed.z = 300;
		break;

	default:
		break;
	}

}

void World::SendFlying() {

	Vector3 top = -1 * player.entity->model.topVector();
	player.entity->model.translate(top.x, top.y, top.z);
	player.entity->model.rotate(DEG2RAD / 2, Vector3(1, 1, 1));

}

bool World::onGround() {

	Vector3 coll, normal;
	bool onGround = false;
	for (size_t i = 0; i < mapSize; i++)
	{
		if (map[i]->mesh->testRayCollision(map[i]->model, Vector3(0.0, 0.0, player.pos.z), -1 * player.entity->model.topVector(),
			coll, normal)) {

			onGround = true;

		}
	}

	return onGround;

}

void World::RenderMinimap() {

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

	renderMap();


	Matrix44 puntitoModel;
	puntitoModel.setTranslation(player.pos.x, player.pos.y, player.pos.z);

	puntitoModel.rotate(90 * DEG2RAD, Vector3(1, 0, 0));

	this->puntito->render(puntitoModel);



	glViewport(0, 0, Game::instance->window_width, Game::instance->window_height);
}

void World::Restart() {
	player.entity->model.setTranslation(0, -13, 0);
	done = false;
	alive = true;
	ground_timer = 0;
}