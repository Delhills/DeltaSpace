#include "GUI.h"
#include "game.h"
#include "input.h"

GUI::GUI(eTypeGui type)
{
	this->type = type;
	this->atlas = Texture::Get("data/gui/atlasGUI.png");
	this->buttonPressed = NO_BUTTON;
	switch (type)
	{
		case MAIN_MENU:
			buttons.push_back(PLAY_M);
			buttons.push_back(TUTORIAL);
			buttons.push_back(EXIT_M);
			break;
		case HUD:
			break;
		case PAUSE_MENU:
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
	glBlendFunc(GL_SRC0_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int width = Game::instance->window_width;
	int height = Game::instance->window_height;
	Camera cameraGUI;
	cameraGUI.setOrthographic(0, width, height, 0, -1, 1);
	cameraGUI.enable();

	Shader* shader = Shader::Get("data/shaders/basic.vs", "data/shaders/gui.fs");

	shader->enable();
	float button_h;
	float button_w;
	float button_x;
	float button_y;
	Vector4 range;
	bool hover;

	for (int i = 0; i < buttons.size(); i++)
	{
		Mesh quad;
		Matrix44 quadModel;
		switch (buttons[i])
		{
		case REPLAY:
			button_h = 100;
			button_w = 100;
			button_x = 100;
			button_y = 100;
			range = Vector4(0.003, 0.118, 0.0995, 0.1015);


			hover = checkHover(button_x, button_y, button_w, button_h);


			quad.createQuad(button_x, button_y, button_w, button_h, false);

			shader->setUniform("u_texture_tiling", 1.0f);
			shader->setUniform("u_model", quadModel);
			shader->setUniform("u_viewprojection", cameraGUI.viewprojection_matrix);
			shader->setTexture("u_texture", atlas, 0);
			shader->setUniform("u_color", Vector4(1, 1, 1, 1));
			shader->setUniform("u_tex_range", hover ? range + Vector4(0.1035, 0, 0, 0) : range);

			quad.render(GL_TRIANGLES);
			break;
		case PLAY_M:
			button_h = 80;
			button_w = 300;
			button_x = width / 4;
			button_y = height / 4;
			range = Vector4(0.24, 0.0005, 0.355, 0.218);

			hover = checkHover(button_x, button_y, button_w, button_h);

	/*		quad.createQuad(button_x, button_y, button_w, button_h, false);

			shader->setUniform("u_texture_tiling", 1.0f);
			shader->setUniform("u_model", quadModel);
			shader->setUniform("u_viewprojection", cameraGUI.viewprojection_matrix);
			shader->setTexture("u_texture", atlas, 0);
			shader->setUniform("u_color", Vector4(1, 1, 1, 1));
			shader->setUniform("u_tex_range", range);

			quad.render(GL_TRIANGLES);*/

			drawText(button_x - button_w/6, button_y - button_h/4, "PLAY", hover ? Vector3(1, 1, 0) : Vector3(1, 1, 1), 4);
			break;

		case TUTORIAL:
			button_h = 80;
			button_w = 300;
			button_x = width / 4;
			button_y = height / 2;
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

			drawText(button_x - button_w / 6, button_y - button_h / 4, "TUTORIAL", hover ? Vector3(1, 1, 0) : Vector3(1, 1, 1), 4);
			break;

		case EXIT_M:
			button_h = 80;
			button_w = 300;
			button_x =  width / 4;
			button_y = 3 * height / 4;
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

			drawText(button_x - button_w / 6, button_y - button_h / 4, "EXIT", hover ? Vector3(1, 1, 0) : Vector3(1, 1, 1), 4);
			break;
		}
		if (hover && Input::isMousePressed(1)) {
			buttonPressed = buttons[i];
		}
		else if (!Input::isMousePressed(1)) {
			buttonPressed = eButton::NO_BUTTON;
		}
	}

	shader->disable();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

eButton GUI::checkButtonClicked()
{
	return buttonPressed;
}

