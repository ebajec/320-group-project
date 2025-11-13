#include "routlette.hpp"
#include "roulette_game.hpp"

#include "state.hpp"
#include "raylib.h"

void RouletteNode::render()
{
	DrawText("ROULETTE (Press ESC to exit)", 
	   10, 10, 30, BLACK);
}
FSMResult RouletteNode::update() 
{
	if (IsKeyPressed(KEY_ESCAPE)) {
		return MAIN_STATE_MAIN_MENU;
	}
	return MAIN_STATE_ROULETTE;
}
