#include "stage.h"
#include "game.h"
#include "input.h"
#include "animation.h"
#include "audio.h"

Stage::Stage() 
{
	this->camera = new Camera();
	this->gui = GUI();
}

void Stage::NextStage() {
	Game* game = Game::instance;
	int stage = game->currentStage;
	stage++;
	Audio::Stop();
	switch (stage)
	{
	case INTRO:
		break;
	case LEVEL1:
		Audio::Play(Songs[SAILOR]);
		break;
	case LEVEL2:
		Audio::Play(Songs[EVA]);
		break;
	case LEVEL3:
		Audio::Play(Songs[UNDER]);
		break;
	case END:
		break;
	case EXIT:
		game->must_exit = true;
	}

	game->currentStage = static_cast<eStageID>(stage);
	//Como usamos diferentes camaras en cada stage hay que ponerla al dia con las dimensiones 
	game->onResize(game->window_width, game->window_height);
}



MenuStage::MenuStage() {

	if (BASS_Init(-1, 44100, 0, 0, NULL) == false) //-1 significa usar el por defecto del sistema operativo
	{
		std::cout << " ERROR abriendo la tarjeta de sonido : " << std::endl;
	}
	//Inicialize all audio
	for (size_t i = 0; i <numSongs; i++)
	{
		Audio::Get(Songs[i]);
	}

	
	Texture* texture = Texture::Get("data/textures/person4.png");
	Mesh* mesh = Mesh::Get("data/meshes/character.mesh");
	Shader* shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	Vector4 color = Vector4(1, 1, 1, 1);
	this->person = EntityMesh(mesh, texture, shader, color);
	person.model.setTranslation(0.0f, 0.0f, 0.0f);

	this->dance = false;
	this->time_dance = 0.0f;
	this->gui = GUI(eTypeGui::MAIN_MENU);
	this->camera->lookAt(Vector3(0.f, 2.0f, 2.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	this->camera->setPerspective(70.f, Game::instance->window_width / (float)Game::instance->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
}

void MenuStage::Render() 
{
	
	camera->enable();
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//set flags
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	Animation* fall = Animation::Get("data/anims/animations_falling.skanim");
	Animation* flair = Animation::Get("data/anims/animations_flair.skanim");

	float t = fmod(Game::instance->time, 1.5) / 1.5;

	fall->assignTime(t * fall->duration);
	flair->assignTime(t * flair->duration);

	if (this->dance) {
		if (this->time_dance < 1)this->time_dance += Game::instance->elapsed_time*2;	
	}
	else {
		if (this->time_dance > 0)this->time_dance -= Game::instance->elapsed_time*2;
	}

	//creamos esqueleto intermedio para contener la postura blendeada
	Skeleton blended_skeleton;


	float time = Game::instance->time;
	float i = 0.3 * sin(time * PI) - 0.3;
	//std::cout << i << "\n";
	person.model.setTranslation(0.6f, i, 0.0f);

	//y blendeamos entre animA y animB con peso 0.5 y lo guardamos en blended_skeleton
	blendSkeleton(&fall->skeleton, &flair->skeleton, this->time_dance, &blended_skeleton);

	this->person.render_anim(&blended_skeleton);

	gui.RenderGui();
	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(Game::instance->window);
}

void MenuStage::Update(double seconds_elapsed) {

	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) NextStage();
	if (Input::wasKeyPressed(SDL_SCANCODE_1)) {
		this->dance = !this->dance;
		Audio::Play(Songs[DANCE]);
	}

	eButton button_pressed = gui.checkButtonClicked();
	//std::cout << button_pressed << "\n";
	switch (button_pressed)
	{
	case PLAY_M:
		NextStage();
		break;
	case EXIT_M:
		Game::instance->must_exit = true;
		break;
	case ACCEPT:
		break;
	case NO_BUTTON:
		break;
	}
}

 PlayStage::PlayStage(const char* filename) 
 {
	 this->world = new World(filename,this->camera);
	 this->gui = GUI(eTypeGui::PAUSE_MENU);
 }

void PlayStage::Render() {

	this->world->render();
}

void PlayStage::Update(double seconds_elapsed) {

	this->world->update(seconds_elapsed);
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) NextStage();
}


EndStage::EndStage() 
{
}

void EndStage::Render()
{
	this->gui = GUI(eTypeGui::END_MENU);
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

