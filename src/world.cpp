#include "world.h"
#include "game.h"
#include "input.h"

float mouse_speed = 100.0f;

Entity::Entity() {
	this->entity_type = NONE;
}


void Entity::addChild(Entity* ent) {
	this->children.push_back(ent);
}

void EntityMesh::render() {
	Camera* camera = Camera::current;
	Matrix44 model = this->model;

	//enable shader and pass uniforms
	shader->enable();
	shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setTexture("u_texture", this->texture, 0);
	shader->setUniform("u_color", this->color);
	//render the mesh using the shader
	mesh->render(GL_TRIANGLES);

	//disable the shader after finishing rendering
	shader->disable();

	for (int i = 0; i < children.size(); i++)
		children[i]->render();  //repeat for every child
}


void EntityMesh::update(float dt) {}

World* World::instance = NULL;

World::World() {
	instance = this;

	islas[0] = new EntityMesh();
	islas[0]->model = Matrix44();
	islas[0]->texture = Texture::Get("data/island_color.tga");
	islas[0]->mesh = Mesh::Get("data/island.ASE");
	islas[0]->shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	islas[0]->color = Vector4(1, 1, 1, 1);
	islas[0]->model.scale(1.5f, 1.5f, 1.5f);

	//islas[1]->model.setTranslation(0, -30, 0);


	//create our camera
	this->camera = new Camera();
	camera->lookAt(Vector3(0.f, 100.f, 100.f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	camera->setPerspective(70.f, Game::instance->window_width / (float)Game::instance->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
}

void World::render() {
	camera->enable();
	//islas[0]->render();
	renderIslands();
}

void World::renderIslands() {
	Camera* camera = Camera::current;
	//Matrix44 model = islas[0]->model;

	Shader* shader = islas[0]->shader;

	Mesh* mesh = islas[0]->mesh;
	Texture* texture = islas[0]->texture;
	Vector4 color = islas[0]->color;

	//enable shader and pass uniforms
	shader->enable();
	//shader->setUniform("u_model", model);
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setTexture("u_texture", texture, 0);
	shader->setUniform("u_color", color);

	Vector3 padding = mesh->box.halfsize;

	//render the mesh using the shader
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			Matrix44 model;
			model.setTranslation(i * padding.x, 0, j * padding.z);
			shader->setUniform("u_model", model);
			mesh->render(GL_TRIANGLES);
		}
	}

	//disable the shader after finishing rendering
	shader->disable();

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
	if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT)) speed *= 10; //move faster with left shift
	if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP)) camera->move(Vector3(0.0f, 0.0f, 1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN)) camera->move(Vector3(0.0f, 0.0f, -1.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT)) camera->move(Vector3(1.0f, 0.0f, 0.0f) * speed);
	if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT)) camera->move(Vector3(-1.0f, 0.0f, 0.0f) * speed);

	//to navigate with the mouse fixed in the middle
	if (mouse_locked)
		Input::centerMouse();


}