#pragma once
#include "world.h"
#include "GUI.h"

enum eStageID 
{
	INTRO,
	TUTORIAL,
	LEVEL1,
	LEVEL2,
	LEVEL3,
	END
};

class Stage
{
public:
	Camera* camera;
	GUI* gui;
	eButton buttonPressed;
	Stage();
	void goToMenuStage();
	void NextStage();
	virtual void Render() {};
	virtual void Update(double seconds_elapsed) {};
};

class MenuStage : public Stage 
{
public:
	
	EntityMesh person;
	bool dance;
	float time_dance;
	EntityMesh* sky;
	double camera_move;
	MenuStage();
	void Render();
	void Update(double seconds_elapsed);
};

class TutorialStage : public Stage
{
public:
	EntityMesh* sky;
	double camera_move;
	TutorialStage();
	void Render();
	void Update(double seconds_elapsed);
};

class PlayStage : public Stage 
{
public:
	World* world;
	PlayStage(const char* filename, const char* textureFile);
	void Render();
	void Update(double seconds_elapsed);

};

class EndStage : public Stage 
{
public:
	double camera_move;
	EntityMesh* sky;
	EndStage();
	void Render();
	void Update(double seconds_elapsed);
};