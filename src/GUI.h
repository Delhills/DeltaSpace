#ifndef GUI_H
#define GUI_H
#include "includes.h"
#include "texture.h"
#include "utils.h"

enum eTypeGui {
	MAIN_MENU,
	TUTORIAL_MENU,
	HUD,
	PAUSE_MENU,
	END_MENU,
	NO_GUI
};

enum eButton {
	MENU_PLAY,
	MENU_EXIT,
	MENU_DANCE,
	BUTTON_LEVEL1,
	BUTTON_LEVEL2,
	BUTTON_LEVEL3,
	ESC,
	WASD,
	PAUSE_REPLAY,
	PAUSE_NEXT,
	PAUSE_EXIT,
	NO_BUTTON
};

class GUI
{
public:
	eTypeGui type;
	std::vector<Vector4> ranges;
	std::vector<eButton> buttons;
	Texture* atlas;
	eButton buttonPressed;
	//bool hover;


	GUI() {};
	GUI(eTypeGui type);
	bool checkHover(float button_x, float button_y, float button_w, float button_h);
	void changeType(eTypeGui type);
	void RenderGui();
	eButton checkButtonClicked();
};


#endif