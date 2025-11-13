#ifndef BLACKJACK_HPP
#define BLACKJACK_HPP

#include "fsm.hpp"
#include "state.hpp"

#include "raylib.h"

class BlackjackNode : public IFSMNode 
{
	virtual void render()
	{
		DrawText("BLACKJACK (Press ESC to exit)", 
		   10, 10, 20, MAROON);
	}

	virtual FSMResult update() 
	{
		if (IsKeyPressed(KEY_ESCAPE)) {
			return MAIN_STATE_MAIN_MENU;
		}
		return MAIN_STATE_BLACKJACK;
	}
};

#endif // BLACKJACK_HPP
