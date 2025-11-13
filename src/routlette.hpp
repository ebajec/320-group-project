#ifndef ROULETTE_HPP
#define ROULETTE_HPP

#include "fsm.hpp"
#include "state.hpp"

#include "raylib.h"

class RouletteNode : public IFSMNode 
{
	virtual void render()
	{
		DrawText("ROULETTE (Press ESC to exit)", 
		   10, 10, 20, MAROON);
	}
	virtual FSMResult update() 
	{
		if (IsKeyPressed(KEY_ESCAPE)) {
			return MAIN_STATE_MAIN_MENU;
		}
		return MAIN_STATE_ROULETTE;
	}

};

#endif // ROULETTE_HPP
