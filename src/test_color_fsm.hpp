#ifndef COLOR_FSM_HPP
#define COLOR_FSM_HPP

#include "fsm.hpp"
#include "raylib.h"

namespace color_fsm
{

enum {
	NODE_RED,
	NODE_GREEN,
	NODE_BLUE,

	NODE_MAX_ENUM
};

class RedNode : public IFSMNode
{
	int ctr = 0;
public:
	virtual void render()
	{
        DrawRectangle(100, 100, 200, 120, RED);

	}
	virtual FSMResult update()
	{
		if (ctr++ < 5)
			return NODE_RED;
		ctr = 0;
		return NODE_GREEN;
	}
};

class GreenNode : public IFSMNode
{
	int ctr = 0;
public:
	virtual void render()
	{
        DrawRectangle(100, 100, 200, 120, GREEN);

	}
	virtual FSMResult update()
	{
		if (ctr++ < 5)
			return NODE_GREEN;
		ctr = 0;
		return NODE_BLUE;
	}
};

class BlueNode : public IFSMNode
{
	int ctr = 0;
public:
	virtual void render()
	{
        DrawRectangle(100, 100, 200, 120, BLUE);
	}
	virtual FSMResult update()
	{
		if (ctr++ < 5)
			return NODE_BLUE;
		ctr = 0;
		return NODE_RED;
	}
};

static inline FSM *create()
{
	FSM *fsm = new FSM();

	fsm->add(NODE_RED,  new RedNode);
	fsm->add(NODE_GREEN,new GreenNode);
	fsm->add(NODE_BLUE, new BlueNode);

	return fsm;
}

};

#endif // COLOR_FSM_HPP
