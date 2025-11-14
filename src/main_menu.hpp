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
	bool samirTextureLoaded = false;
	float lastWidth = 0.0f;
	float lastHeight = 0.0f;
	void refreshLayout();
public:
	MainMenuNode();	
	~MainMenuNode() override;
	virtual void render() override;
	virtual FSMResult update() override; 
};

#endif // MAIN_MENU_HPP
