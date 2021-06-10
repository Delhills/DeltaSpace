#include "stage.h"
#include "game.h"
#include "input.h"
#include "animation.h"


Stage::Stage() {}

void Stage::NextStage() {
	int stage = Game::instance->currentStage;
	stage++;
	if (stage > END) Game::instance->must_exit = true;
	Game::instance->currentStage = static_cast<eStageID>(stage);
}

MenuStage::MenuStage() {

	Texture* texture = Texture::Get("data/textures/person4.png");
	Mesh* mesh = Mesh::Get("data/meshes/person.obj");
	Shader* shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	Vector4 color = Vector4(1, 1, 1, 1);
	this->person = EntityMesh(mesh, texture, shader, color);
	person.model.setTranslation(0.0f, 0.0f, 0.0f);


	this->menu_cam = Camera();
	menu_cam.lookAt(Vector3(0.f, 2.0f, 2.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	menu_cam.setPerspective(70.f, Game::instance->window_width / (float)Game::instance->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
}

void MenuStage::Render() 
{
	menu_cam.enable();
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//set flags
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	Animation* fall = Animation::Get("data/anims/animations_falling.skanim");

	fall->assignTime(Game::instance->time);
	this->person.render_anim(fall);

	drawText(Game::instance->window_width / 2 - 300, Game::instance->window_height / 2 - 20, "Z para continuar ", Vector3(1, 1, 0), 10);


	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(Game::instance->window);
}

void MenuStage::Update(double seconds_elapsed) {

	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) NextStage();
}

 PlayStage::PlayStage(World world) {

	 this->world = world;
}

void PlayStage::Render() {

	this->world.render();
}

void PlayStage::Update(double seconds_elapsed) {

	this->world.update(seconds_elapsed);
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) NextStage();
}

EndStage::EndStage() {}

void EndStage::Render()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawText(Game::instance->window_width / 2 - 300, Game::instance->window_height / 2 - 20, "Fin  ", Vector3(1, 1, 0), 10);

	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(Game::instance->window);
}

void EndStage::Update(double seconds_elapsed) {

	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) NextStage();
}
