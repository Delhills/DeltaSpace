#include "stage.h"
#include "game.h"
#include "input.h"

Stage::Stage() {}

void Stage::NextStage() {

	int stage = Game::instance->currentStage;
	stage++;

	if (stage > END) Game::instance->must_exit = true;
	
	Game::instance->currentStage = static_cast<eStageID>(stage);
}

MenuStage::MenuStage() {}

void MenuStage::Render() 
{
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
