#include "state.hpp"

#include "raylib.h"
#include "blackjack.hpp"

void BlackjackNode::render()
{
	DrawText("BLACKJACK (Press ESC to exit)", 
	   10, 10, 30, BLACK);
}

FSMResult BlackjackNode::update() 
{
	if (IsKeyPressed(KEY_ESCAPE)) {
		return MAIN_STATE_MAIN_MENU;
	}
	return MAIN_STATE_BLACKJACK;
}
