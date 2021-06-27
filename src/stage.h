#pragma once
#include "world.h"

enum eStageID {
	INTRO,
	LEVEL1,
	LEVEL2,
	END, 
	EXIT
};

class Stage
{
public:
	Camera* camera;
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
	PlayStage(const char* filename);
	void Render();
	World* world;
	void Update(double seconds_elapsed);
};

class EndStage : public Stage {
public:
	EndStage();
	void Render();
	void Update(double seconds_elapsed);
};