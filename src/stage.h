#pragma once
#include "world.h"

enum eStageID {
	INTRO,
	LEVEL1,
	END 
};

class Stage
{
public:
	Stage();
	void NextStage();
	virtual void Render() {};
	virtual void Update(double seconds_elapsed) {};
};

class MenuStage : public Stage {
public:
	MenuStage();
	void Render();
	void Update(double seconds_elapsed);
};



class PlayStage : public Stage {
public:
	PlayStage(World world);
	void Render();
	World world;
	void Update(double seconds_elapsed);
};

class EndStage : public Stage {
public:
	EndStage();
	void Render();
	void Update(double seconds_elapsed);
};