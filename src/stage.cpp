#include "stage.h"
#include "game.h"
#include "input.h"
#include "animation.h"
#include "audio.h"

Stage::Stage() 
{
	this->camera = new Camera();
	this->gui = NULL;
}

void Stage::goToMenuStage() 
{
	Audio::Stop();
	Game::instance->currentStage = eStageID::INTRO;
}

void Stage::NextStage() {
	Game* game = Game::instance;
	int stage = game->currentStage;
	stage++;
	Audio::Stop();
	PlayStage* a;
	switch (stage)
	{
	case TUTORIAL:
		break;

	case LEVEL1:
		a = (PlayStage*)game->stages[LEVEL1];
		a->world->Restart();
		Audio::Play(Songs[SAILOR]);
		break;
	case LEVEL2:
		a = (PlayStage*)game->stages[LEVEL2];
		a->world->Restart();
		Audio::Play(Songs[EVA]);
		break;
	case LEVEL3:
		a = (PlayStage*)game->stages[LEVEL3];
		a->world->Restart();
		Audio::Play(Songs[UNDER]);
		break;
	case END:
		Audio::Play(Songs[CREDITS]);
		break;
	}

	game->currentStage = static_cast<eStageID>(stage);
	//Como usamos diferentes camaras en cada stage hay que ponerla al dia con las dimensiones 
	game->onResize(game->window_width, game->window_height);
}

MenuStage::MenuStage()
{

	if (BASS_Init(-1, 44100, 0, 0, NULL) == false) //-1 significa usar el por defecto del sistema operativo
	{
		std::cout << " ERROR abriendo la tarjeta de sonido : " << std::endl;
	}
	//Inicialize all audio
	for (size_t i = 0; i <numSongs; i++)
	{
		Audio::Get(Songs[i]);
	}

	buttonPressed = NO_BUTTON;
	Texture* texture = Texture::Get("data/textures/person4.png");
	Mesh* mesh = Mesh::Get("data/meshes/character.mesh");
	Shader* shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	Vector4 color = Vector4(1, 1, 1, 1);
	this->person = EntityMesh(mesh, texture, shader, color);
	person.model.setTranslation(0.0f, 0.0f, 0.0f);

	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	texture = Texture::Get("data/textures/skybox.PNG");
	mesh = Mesh::Get("data/meshes/esfera.obj");
	this->sky = new EntityMesh(mesh, texture, shader, color);
	camera_move = 0;

	this->dance = false;
	this->time_dance = 0.0f;
	this->gui = new GUI(eTypeGui::MAIN_MENU);
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
	Matrix44 skymodel;
	skymodel.setTranslation(camera->eye.x, camera->eye.y, camera->eye.z);
	sky->render(skymodel);
	glEnable(GL_DEPTH_TEST);
	
	camera->rotate(DEG2RAD * camera_move, Vector3(0, 0, 1));
	
	drawText(0, 0, "Do not press 1:", Vector3(1, 1, 1), 0.7);

	Animation* fall = Animation::Get("data/anims/animations_falling.skanim");
	Animation* flair = Animation::Get("data/anims/animations_flair.skanim");

	float t = fmod(Game::instance->time, 1.5) / 1.5;

	fall->assignTime(t * fall->duration);
	flair->assignTime(t * flair->duration);

	if (this->dance) {
		if (this->time_dance < 1) this->time_dance += Game::instance->elapsed_time*2;	
	}
	else {
		if (this->time_dance > 0) this->time_dance -= Game::instance->elapsed_time*2;
	}

	//creamos esqueleto intermedio para contener la postura blendeada
	Skeleton blended_skeleton;


	float time = Game::instance->time;
	float i = 0.3 * sin(time * PI) - 0.3;
	//std::cout << i << "\n";
	person.model.setTranslation(camera->center.x+0.6, camera->center.y+ i, camera->center.z);

	//y blendeamos entre animA y animB con peso 0.5 y lo guardamos en blended_skeleton
	blendSkeleton(&fall->skeleton, &flair->skeleton, this->time_dance, &blended_skeleton);

	this->person.render_anim(&blended_skeleton);

	gui->RenderGui();
	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(Game::instance->window);
}

void MenuStage::Update(double seconds_elapsed)
{


	if (Input::wasMousePressed(1))
	{
		std::cout << "holi" << "\n";
		buttonPressed = gui->buttonPressed;
	}

	camera_move = seconds_elapsed*10;

	switch (buttonPressed)
	{
	case MENU_PLAY:
		std::cout << "play" << "\n";
		NextStage();
		break;
	case MENU_EXIT:
		std::cout << "exit" << "\n";
		Game::instance->must_exit = true;
		break;
	case MENU_DANCE:
		std::cout << "dance" << "\n";
		this->dance = !this->dance;
		if (this->dance)
		{
			Audio::Play(Songs[DANCE]);
		}
		else {
			Audio::Stop();
		}
		break;
	}

	buttonPressed = NO_BUTTON;

	if (Input::wasKeyPressed(SDL_SCANCODE_1)) Audio::Play(Songs[DONOT]);
}

TutorialStage::TutorialStage() 
{
	camera_move = 0;
	this->gui = new GUI(eTypeGui::END_MENU);
	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	Texture* texture = Texture::Get("data/textures/skybox.PNG");
	Mesh* mesh = Mesh::Get("data/meshes/esfera.obj");
	Vector4 color = Vector4(1, 1, 1, 1);
	this->sky = new EntityMesh(mesh, texture, shader, color);
	this->camera->lookAt(Vector3(0.f, 2.0f, 2.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	this->camera->setPerspective(70.f, Game::instance->window_width / (float)Game::instance->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
}

void TutorialStage::Render() 
{
	camera->enable();
	glClearColor(0.0, 0.0, 0.0, 1.0);


	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//set flags
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);


	glDisable(GL_DEPTH_TEST);
	Matrix44 skymodel;
	skymodel.setTranslation(camera->eye.x, camera->eye.y, camera->eye.z);
	sky->render(skymodel);
	glEnable(GL_DEPTH_TEST);

	camera->rotate(DEG2RAD * camera_move, Vector3(0, 0, 1));

	SDL_GL_SwapWindow(Game::instance->window);
}

void TutorialStage::Update(double seconds_elapsed) 
{
	camera_move = seconds_elapsed * 10;
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) NextStage();

}

PlayStage::PlayStage(const char* filename, const char* textureFile)
 {
	 this->gui = new GUI(eTypeGui::PAUSE_MENU);
	 this->world = new World(filename,this->camera, this->gui, textureFile);
 }

void PlayStage::Render() 
{
	this->world->render();
}

void PlayStage::Update(double seconds_elapsed) 
{

	this->world->update(seconds_elapsed);
	if (this->world->nextLevel) {
		this->world->nextLevel = false;
		NextStage();
	}
	else if (this->world->goToMenu)
	{
		//this->world->Restart();
		goToMenuStage();
	}
	//std::cout << this->world->goToMenu << "\n";
}

EndStage::EndStage() 
{
	this->gui = new GUI(eTypeGui::END_MENU);
	camera_move = 0;
	
	Shader* shader= Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	Texture* texture= Texture::Get("data/textures/skybox.PNG");
	Mesh* mesh= Mesh::Get("data/meshes/esfera.obj");
	Vector4 color = Vector4(1, 1, 1, 1);
	this->sky = new EntityMesh(mesh, texture, shader, color);
	this->camera->lookAt(Vector3(0.f, 2.0f, 2.0f), Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f)); //position the camera and point to 0,0,0
	this->camera->setPerspective(70.f, Game::instance->window_width / (float)Game::instance->window_height, 0.1f, 10000.f); //set the projection, we want to be perspective
}

void EndStage::Render()
{
	camera->enable();
	glClearColor(0.0, 0.0, 0.0, 1.0);


	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//set flags
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);


	glDisable(GL_DEPTH_TEST);
	Matrix44 skymodel;
	skymodel.setTranslation(camera->eye.x, camera->eye.y, camera->eye.z);
	sky->render(skymodel);
	glEnable(GL_DEPTH_TEST);

	camera->rotate(DEG2RAD * camera_move, Vector3(0, 0, 1));

	int h = Game::instance->window_height;
	int w = Game::instance->window_width;


	drawText(w / 2 - 300, h / 4 - 20, "Developers:", Vector3(1, 1, 0), 5);
	drawText(w / 2 - 300, 2*h / 5 - 50, "Daniel Salvado", Vector3(1, 1, 0), 5);
	drawText(w / 2 - 300, 2*h / 5, "Andrea Borrell", Vector3(1, 1, 0), 5);
	drawText(w / 2 - 300, 2*h / 5 + 70, "Music:", Vector3(1, 1, 0), 5);
	drawText(w / 2 - 300, 3*h / 5, "Rifat Yurham", Vector3(1, 1, 0), 5);


	//swap between front buffer and back buffer
	SDL_GL_SwapWindow(Game::instance->window);
}

void EndStage::Update(double seconds_elapsed)
{
	camera_move = seconds_elapsed * 10;
	if (Input::wasKeyPressed(SDL_SCANCODE_Z)) Game::instance->must_exit = true;
}

