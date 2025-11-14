#ifndef ROULETTE_HPP
#define ROULETTE_HPP

#include "fsm.hpp"

#include <memory>

class RouletteNode : public IFSMNode 
{
public:
	RouletteNode();
	~RouletteNode() override;

	virtual void render();
	virtual FSMResult update(); 

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

#endif // ROULETTE_HPP
