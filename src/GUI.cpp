#include "GUI.h"
#include "game.h"
#include "input.h"
#include "utils.h"

GUI::GUI(eTypeGui type)
{
	this->type = type;
	this->atlas = Texture::Get("data/gui/levelsAtlas.png");
	this->buttonPressed = NO_BUTTON;

	//hover = false;
	switch (type)
	{
		case MAIN_MENU:
			buttons.push_back(MENU_EXIT);
			buttons.push_back(MENU_PLAY);
			buttons.push_back(MENU_DANCE);
			break;
		case TUTORIAL_MENU:
			buttons.push_back(BUTTON_LEVEL1);
			buttons.push_back(BUTTON_LEVEL2);
			buttons.push_back(BUTTON_LEVEL3);
			break;
		case HUD:
			break;
		case PAUSE_MENU:
			buttons.push_back(PAUSE_REPLAY);
			buttons.push_back(PAUSE_NEXT);
			buttons.push_back(PAUSE_EXIT);
			break;
		case END_MENU:
			break;
		case NO_GUI:
			break;
	}
}

bool GUI::checkHover(float button_x, float button_y, float button_w, float button_h)
{

	Vector2 mousepos = Input::mouse_position;
	float halfW = button_w * 0.5;
	float halfH = button_h * 0.5;
	float min_x = button_x - halfW;
	float min_y = button_y - halfH;
	float max_x = button_x + halfW;
	float max_y = button_y + halfH;

	bool hover = mousepos.x > min_x && mousepos.y > min_y && mousepos.x < max_x&& mousepos.y < max_y;
	return hover;
}

void GUI::changeType(eTypeGui type)
{
	this->type = type;
}

void GUI::RenderGui()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int width = Game::instance->window_width;
	int height = Game::instance->window_height;
	Camera cameraGUI;
	cameraGUI.setOrthographic(0, width, height, 0, -1, 1);
	cameraGUI.enable();

	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/gui.fs");

	float button_h;
	float button_w;
	float button_x;
	float button_y;
	Vector4 range;
	bool hover = false;
	buttonPressed = NO_BUTTON;
	//Mesh quad;
	//Matrix44 quadModel;

	if (this->type == TUTORIAL_MENU)
	{
		Mesh quad3;
		Matrix44 quadModel3;
		shader->enable();
		button_h = 170;
		button_w = 700;

		//button_x = width - width / 5;
		button_x = width / 2;
		button_y = 4 * height / 5;
		range = Vector4(0.0, 0.0, 1.0, 1.0);

		hover = checkHover(button_x, button_y, button_w, button_h);

		quad3.createQuad(button_x, button_y, button_w, button_h, false);

		shader->setUniform("u_texture_tiling", 1.0f);
		shader->setUniform("u_model", quadModel3);
		shader->setUniform("u_viewprojection", cameraGUI.viewprojection_matrix);
		shader->setTexture("u_texture", atlas, 2);
		shader->setUniform("u_color", Vector4(1, 1, 1, 1));
		shader->setUniform("u_tex_range", range);

		quad3.render(GL_TRIANGLES);
		shader->disable();
	}


	for (int i = 0; i < buttons.size(); i++)
	{
		switch (buttons[i])
		{
		case MENU_PLAY:
			button_h = 80;
			button_w = 300;
			button_x = width / 4;
			button_y = height / 4;
			range = Vector4(0.24, 0.0005, 0.355, 0.218);

			hover = checkHover(button_x, button_y, button_w, button_h);

			drawText(button_x - button_w/6, button_y - button_h/4, "PLAY", hover ? Vector3(1, 1, 0) : Vector3(1, 1, 1), 4);
			if (hover) buttonPressed = buttons[i];
			break;

		case MENU_EXIT:
			button_h = 80;
			button_w = 300;
			button_x =  width / 4;
			button_y = 3 * height / 4;
			range = Vector4(0.24, 0.0005, 0.355, 0.218);

			hover = checkHover(button_x, button_y, button_w, button_h);

			drawText(button_x - button_w / 6, button_y - button_h / 4, "SALIR", hover ? Vector3(1, 1, 0) : Vector3(1, 1, 1), 4);
			if (hover) buttonPressed = buttons[i];
			break;


		case MENU_DANCE:
			button_h = 80;
			button_w = 300;
			button_x = width / 4;
			button_y = height / 2;
			range = Vector4(0.24, 0.0005, 0.355, 0.218);

			hover = checkHover(button_x, button_y, button_w, button_h);

			drawText(button_x - button_w / 6, button_y - button_h / 4, "??????", hover ? Vector3(1, 1, 0) : Vector3(1, 1, 1), 4);
			if (hover) buttonPressed = buttons[i];
			break;
		case BUTTON_LEVEL1: {

			button_h = 170;
			button_w = 225;
			button_x = width/2 - 250;
			button_y = 4 * height / 5;

			hover = checkHover(button_x, button_y, button_w, button_h);


			drawText(button_x - button_w / 4, button_y - button_h / 2 + 7, "MOON TIARA", hover ? Vector3(1, 1, 0) : Vector3(1, 1, 1), 2);
			if (hover) buttonPressed = buttons[i];
			break;
		}
		case BUTTON_LEVEL2: {

			button_h = 170;
			button_w = 225;
			button_x = width / 2;
			button_y = 4 * height / 5;


			hover = checkHover(button_x, button_y, button_w, button_h);


			drawText(button_x - button_w / 6, button_y - button_h / 2 + 7, "NERVE", hover ? Vector3(1, 1, 0) : Vector3(1, 1, 1), 2);
			if (hover) buttonPressed = buttons[i];
			break;
		}
		case BUTTON_LEVEL3: {

			button_h = 170;
			button_w = 225;
			button_x = width / 2 + 250;
			button_y = 4 * height / 5;
			hover = checkHover(button_x, button_y, button_w, button_h);


			drawText(button_x - button_w / 4, button_y - button_h / 2 + 7, "LAST CORRIDOR", hover ? Vector3(1, 1, 0) : Vector3(1, 1, 1), 2);
			if (hover) buttonPressed = buttons[i];
			break;
		}
		case PAUSE_NEXT:
			button_h = 80;
			button_w = 300;
			button_x = width / 2 - 40;
			button_y = 5 * height / 8;
			range = Vector4(0.24, 0.0005, 0.355, 0.218);

			hover = checkHover(button_x, button_y, button_w, button_h);

			//quad.createQuad(button_x, button_y, button_w, button_h, false);

			//shader->setUniform("u_texture_tiling", 1.0f);
			//shader->setUniform("u_model", quadModel);
			//shader->setUniform("u_viewprojection", cameraGUI.viewprojection_matrix);
			//shader->setTexture("u_texture", atlas, 0);
			//shader->setUniform("u_color", Vector4(1, 1, 1, 1));
			//shader->setUniform("u_tex_range", range);

			//quad.render(GL_TRIANGLES);

			drawText(button_x - button_w / 6, button_y - button_h / 4, "SIGUIENTE", hover ? Vector3(1, 1, 0) : Vector3(1, 1, 1), 4);
			if (hover) buttonPressed = buttons[i];
			break;

		case PAUSE_REPLAY:
			button_h = 80;
			button_w = 300;
			button_x = width / 2 - 40;
			button_y = 6 * height / 8;
			//range = Vector4(0.24, 0.0005, 0.355, 0.218);

			hover = checkHover(button_x, button_y, button_w, button_h);

			drawText(button_x - button_w / 6, button_y - button_h / 4, "REPLAY", hover ? Vector3(1, 1, 0) : Vector3(1, 1, 1), 4);
			if (hover) buttonPressed = buttons[i];
			break;

		case PAUSE_EXIT:
			button_h = 80;
			button_w = 300;
			button_x = width / 2 - 40;
			button_y = 7 * height / 8;
			range = Vector4(0.24, 0.0005, 0.355, 0.218);

			hover = checkHover(button_x, button_y, button_w, button_h);

			drawText(button_x - button_w / 6, button_y - button_h / 4, "SALIR", hover ? Vector3(1, 1, 0) : Vector3(1, 1, 1), 4);
			if (hover) buttonPressed = buttons[i];
			break;
		}

	}


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

eButton GUI::checkButtonClicked()
{

	return buttonPressed;
}

