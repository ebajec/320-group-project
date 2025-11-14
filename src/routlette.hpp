#ifndef ROULETTE_HPP
#define ROULETTE_HPP

#include "fsm.hpp"

#include <memory>

class RouletteNode : public IFSMNode 
{
public:
	RouletteNode();
	~RouletteNode() override;

	virtual void render() override;
	virtual FSMResult update() override; 

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

#endif // ROULETTE_HPP
