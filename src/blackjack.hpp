#ifndef BLACKJACK_HPP
#define BLACKJACK_HPP

#include "fsm.hpp"

class BlackjackNode : public IFSMNode 
{
	virtual void render();
	virtual FSMResult update() ;
};

#endif // BLACKJACK_HPP
