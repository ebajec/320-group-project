#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "fsm.hpp"
#include "raylib.h"

#include "ui/button.hpp"

class MainMenuNode : public IFSMNode 
{
	ui::Button blackjack_button;
	ui::Button roulette_button;

	Texture2D samir_tex;
public:
	MainMenuNode();	
	virtual void render();
	virtual FSMResult update(); 
};

#endif // MAIN_MENU_HPP
