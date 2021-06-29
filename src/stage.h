#pragma once
#include "world.h"
#include "GUI.h"

enum eStageID {
	INTRO,
	LEVEL1,
	LEVEL2,
	LEVEL3,
	END,
	EXIT
};
class Stage
{
public:
	Camera* camera;
	GUI* gui;
	Stage();
	void NextStage();
	virtual void Render() {};
	virtual void Update(double seconds_elapsed) {};
};

class MenuStage : public Stage {
public:
	
	EntityMesh person;
	bool dance;
	float time_dance;
	MenuStage();
	void Render();
	void Update(double seconds_elapsed);
};



class PlayStage : public Stage {
public:
	World* world;
	PlayStage(const char* filename);
	void Render();
	void Update(double seconds_elapsed);
};

class EndStage : public Stage {
public:
	EndStage();
	void Render();
	void Update(double seconds_elapsed);
};