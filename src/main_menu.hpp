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
		float y = c.y - 200;

		DrawText(text, x, y, fontsize, MAROON);
}

class MainMenuNode : public IFSMNode 
{
	ui::Button blackjack_button;
	ui::Button roulette_button;
public:
	MainMenuNode() : IFSMNode()
	{
		Vector2 c;
		g_get_screen_center(&c.x, &c.y);

		float w = 200;
		float h = 100;

		blackjack_button = ui::Button(
			Rectangle{c.x - 1.5f*w,c.y,w,h},"Play Blackjack!"
		);
		roulette_button = ui::Button(
			Rectangle{c.x + 0.5f*w,c.y,w,h},"Play Roulette!"
		);
	}

	virtual void render()
	{
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
