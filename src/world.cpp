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
	Texture* texture = Texture::Get("data/export.png");
	Mesh* mesh = Mesh::Get("data/export.obj");
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	Vector4 color = Vector4(1, 1, 1, 1);
	int offset = 40;

	Vector3 padding = mesh->box.halfsize;
	
	for (int i = 0; i < mapSize; i++)
	{
		map[i] = new EntityMesh(mesh, texture, shader, color);
		map[i]->model.setTranslation(0, 0, i * (-padding.z - offset)-10);
	}

	texture = Texture::Get("data/meta.png");
	mesh = Mesh::Get("data/meta.obj");
	EntityMesh* goal=new EntityMesh(mesh, texture, shader, color);
	goal->model.setTranslation(0, 0, mapSize * (-padding.z - offset)+35);
	this->goal = goal;

	texture = Texture::Get("data/barra2.png");
	mesh = Mesh::Get("data/barra2.obj");

	//for (int i = 0; i < mapSize * 2; i = i + 2)
	//{
	//	bars[i] = new EntityMesh(mesh, texture, shader, color);
	//	bars[i + 1] = new EntityMesh(mesh, texture, shader, color);

	//	Vector3 pos = map[i / 2]->model.getTranslation();


	//	bars[i]->model.setTranslation(pos.x, pos.y, pos.z);
	//	bars[i]->model.setTranslation(-padding.x / 2 + 10, 0, i * (-padding.z) + 5);
	//	bars[i + 1]->model.setTranslation(padding.x / 2 - 10, 0, i * (-padding.z) + 5);
	//	//map[i / 2]->addChild(bars[i]);
	//	//map[i / 2]->addChild(bars[i + 1]);
	//	obstacles.push_back(bars[i]);
	//	obstacles.push_back(bars[i+1]);
	//}

	player = Player();
	Vector3 paddingPlayer = player.entity->mesh->box.halfsize;
	player.entity->model.setTranslation(0, paddingPlayer.y/10, 0);

	texture = Texture::Get("data/SkySkybox.png");
	mesh = Mesh::Get("data/prueba.obj");
	sky = new EntityMesh(mesh, texture, shader, color);

	
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

	//set the camera as default


	//set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//create model matrix for cube
	//Matrix44 m;
	//m.rotate(angle*DEG2RAD, Vector3(0, 1, 0));

	//if(shader)
	//{
	//	//enable shader
	//	shader->enable();

	//	//upload uniforms
	//	shader->setUniform("u_color", Vector4(1,1,1,1));
	//	shader->setUniform("u_viewprojection", camera->viewprojection_matrix );
	//	shader->setUniform("u_texture", texture, 0);
	//	shader->setUniform("u_model", m);
	//	shader->setUniform("u_time", time);

	//	//do the draw call
	//	mesh->render( GL_TRIANGLES );

	//	//disable shader
	//	shader->disable();
	//}

	camera->enable();

	Matrix44 playerModel;// = player.entity->model;
	playerModel.rotate(player.rot * DEG2RAD, Vector3(0.0, 0.0, 1.0));
	playerModel.translate(player.pos.x, player.pos.y, player.pos.z);

	//playerModel.setUpAndOrthonormalize(player.normal);
	//Vector3 up = playerModel.rotateVector(Vector3(0,1,0));

	//playerModel.setUpAndOrthonormalize();

	//std::cout << up.x << ", " << up.y << ", " << up.z << ", " << "\n";

	//player.entity->model.rotate(player.rot, Vector3(0.0, 0.0, 1.0));

	//std::cout << player.rot  << "\n";

	//islas[0]->render();
	renderMap();
	renderObstacles();
	player.entity->model = playerModel;
	player.Render();
	goal->render();

	//std::cout << coll.x << ", " << coll.y << ", " << coll.z << ", " << "\n";
	if (!freecam) {

		Vector3 eye = playerModel * Vector3(0.0f, 7.0f, 10.0f);
		Vector3 center = playerModel * Vector3(0.0f, 0.0f, -5.0f);
		Vector3 up = playerModel.topVector();
		//std::cout << up.x << ", " << up.y << ", " << up.z << ", " << "\n";
		camera->lookAt(eye, center, up);
	}

	Matrix44 skyModel;
	skyModel.translate(camera->eye.x, camera->eye.y, camera->eye.z);
	sky->render(skyModel);

	drawText(5, Game::instance->window_height - 25, std::to_string(player.speed.z * 1000) + " Km/h", Vector3(1, 1, 1), 3);
	if (done) drawText(Game::instance->window_width / 2 - 300, Game::instance->window_height / 2 - 20, "Has ganado ", Vector3(1, 1, 0), 10);


	//Draw the floor grid
	drawGrid();

	//render the FPS, Draw Calls, etc
	drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(Game::instance->window);
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
	obstacles.push_back(obstacle);

	
}

void World::update(double seconds_elapsed) {

	bool mouse_locked = Game::instance->mouse_locked;
	float camSpeed = seconds_elapsed * mouse_speed; //the speed is defined by the seconds_elapsed so it goes constant
	float speed = seconds_elapsed * 10.0f;
	float turn_speed = seconds_elapsed * 50.0f;

	ComputePos();
	
	//mouse input to rotate the cam
	if ((Input::mouse_state & SDL_BUTTON_LEFT) || mouse_locked) //is left button pressed?
	{
		camera->rotate(Input::mouse_delta.x * 0.005f, Vector3(0.0f, -1.0f, 0.0f));
		camera->rotate(Input::mouse_delta.y * 0.005f, camera->getLocalVector(Vector3(-1.0f, 0.0f, 0.0f)));
	}

	//async input to move the camera around
	if (Input::wasKeyPressed(SDL_SCANCODE_1)) freecam = !freecam; //move faster with left shift
	if (Input::wasKeyPressed(SDL_SCANCODE_0)) { player.pos = Vector3(0, 0, 0); done = false; }//move faster with left shift

	if (freecam)
	{
		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) camSpeed *= 10; //move faster with left shift
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * camSpeed);
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * camSpeed);
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * camSpeed);
		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * camSpeed);
		if (Input::wasKeyPressed(SDL_SCANCODE_C)) addObstacle();
	}
	else
	{
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) player.accelerate(speed);
		else if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) player.accelerate(-speed);

		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) player.turn(turn_speed);
		else if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) player.turn(-turn_speed);


		player.pos.z = player.pos.z - player.speed.z;
		
	}

	for (size_t i = 0; i < obstacles.size(); i++)
	{
		if (checkCol(obstacles[i], player.pos)){

			std::cout << "collision!" << "\n";
			if (player.speed.z > 0.2) {
				player.speed.z = player.speed.z / 2;
			}
			else {
				player.speed.z = 0.2;
			}
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

void World::ComputePos() {

	Vector3 coll, normal;
	for (size_t i = 0; i < mapSize; i++)
	{
		if (map[i]->mesh->testRayCollision(map[i]->model, Vector3(0.0, 0.0, player.pos.z), -1 * player.entity->model.topVector(),
			coll, normal)) {
			player.pos = coll;
			player.normal = normal;
		}
	}
}
