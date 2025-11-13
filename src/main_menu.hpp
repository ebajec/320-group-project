#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "fsm.hpp"
#include "state.hpp"
#include "raylib.h"

class MainMenuNode : public IFSMNode 
{
	virtual void render()
	{
		DrawText(
			"MAIN MENU : Press B for blackjack, R for roulette",
			10, 10, 20, MAROON);
	}
	virtual FSMResult update() 
	{
		if (IsKeyPressed(KEY_B)) {
			return MAIN_STATE_BLACKJACK;
		}
		else if (IsKeyPressed(KEY_R)) {
			return MAIN_STATE_ROULETTE;
		}
		return MAIN_STATE_MAIN_MENU;
	}
};

#endif // MAIN_MENU_HPP
