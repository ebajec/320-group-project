#ifndef BLACKJACK_HPP
#define BLACKJACK_HPP

#include "fsm.hpp"

#include <memory>

class BlackjackNode : public IFSMNode 
{
public:
	BlackjackNode();
	~BlackjackNode() override;

	virtual void render();
	virtual FSMResult update();

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

#endif // BLACKJACK_HPP
