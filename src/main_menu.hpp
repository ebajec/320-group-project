#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "globals.h"

#include "fsm.hpp"
#include "state.hpp"
#include "raylib.h"

#include "ui/button.hpp"

#include <cstring>

static inline void draw_title_text()
{
		Vector2 c;
		g_get_screen_center(&c.x, &c.y);

		const char * text = "Are you ready to Spin With Samir?";

		float fontsize = 40;
		float x = c.x - 0.25*fontsize*(float)strlen(text); 
		float y = 2*fontsize;

		DrawText(text, x, y, fontsize, LIGHTGRAY);
}

class MainMenuNode : public IFSMNode 
{
	ui::Button blackjack_button;
	ui::Button roulette_button;

	Texture2D samir_tex;
public:
	MainMenuNode() : IFSMNode()
	{
		Image samir_img = LoadImage("samir.jpg");

		ImageResize(&samir_img, 200, 200);
		samir_tex = LoadTextureFromImage(samir_img);

		if (!samir_tex.id) 
			printf("Failed to load samir texture!\n");

		Vector2 c;
		g_get_screen_center(&c.x, &c.y);

		float w = 200;
		float h = 100;

		float sh = g_.screenHeight;
		float sw = g_.screenWidth;

		blackjack_button = ui::Button(
			Rectangle{c.x - 1.5f*w,sh - 2*h,w,h},"Play Blackjack!"
		);
		roulette_button = ui::Button(
			Rectangle{c.x + 0.5f*w,sh - 2*h,w,h},"Play Roulette!"
		);
	}

	virtual void render()
	{
		Vector2 c;
		g_get_screen_center(&c.x, &c.y);

		DrawTexture(samir_tex, 
			  c.x - 0.5f*samir_tex.width, 
			  c.y - 0.5f*samir_tex.height, 
			  WHITE
			  );

		draw_title_text();
		DrawButton(blackjack_button);
		DrawButton(roulette_button);
	}
	virtual FSMResult update() 
	{
		if (ui::IsButtonPressed(blackjack_button)) {
			return MAIN_STATE_BLACKJACK;
		}
		else if (ui::IsButtonPressed(roulette_button)) {
			return MAIN_STATE_ROULETTE;
		}
		return MAIN_STATE_MAIN_MENU;
	}
};

#endif // MAIN_MENU_HPP
