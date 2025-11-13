#ifndef ROULETTE_HPP
#define ROULETTE_HPP

#include "fsm.hpp"

class RouletteNode : public IFSMNode 
{
	virtual void render();
	virtual FSMResult update(); 
};

#endif // ROULETTE_HPP
