#ifndef GUI_H
#define GUI_H
#include "includes.h"
#include "texture.h"
#include "utils.h"

enum eTypeGui {
	MAIN_MENU,
	HUD,
	PAUSE_MENU,
	END_MENU,
	NO_GUI
};

enum eButton {
	PLAY,
	PLAY_M,
	REPLAY,
	EXIT_N,
	EXIT_M,
	ACCEPT,
	WINDOW,
	NO_BUTTON,
	TUTORIAL
};

class GUI
{
public:
	eTypeGui type;
	std::vector<Vector4> ranges;
	std::vector<eButton> buttons;
	Texture* atlas;
	eButton buttonPressed;
	bool hover;

	GUI() {};
	GUI(eTypeGui type);
	bool checkHover(float button_x, float button_y, float button_w, float button_h);
	void changeType(eTypeGui type);
	void RenderGui();
	eButton checkButtonClicked();
};


#endif