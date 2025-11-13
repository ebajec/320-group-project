#ifndef COLOR_FSM_HPP
#define COLOR_FSM_HPP

#include "fsm.hpp"
#include "raylib.h"
#include "ui/ui.hpp"

namespace color_fsm
{

enum {
	NODE_RED,
	NODE_GREEN,
	NODE_BLUE,

	NODE_MAX_ENUM
};

class BaseColorNode : public IFSMNode
{
protected:
	Color fill{};
	uint32_t stayNode = NODE_RED;
	uint32_t nextNode = NODE_GREEN;
	ui::Button button;

public:
	BaseColorNode(Color color, uint32_t stay, uint32_t next, const char *label)
		: fill(color)
		, stayNode(stay)
		, nextNode(next)
		, button(Rectangle{ 520, 360, 240, 60 }, label)
	{}

	void render() override
	{
		DrawRectangle(100, 100, 200, 120, fill);
		ui::DrawButton(button);

		if (ui::input::MouseDown())
		{
			DrawText("Mouse down", 20, 400, 20, DARKGRAY);
		}
	}

	FSMResult update() override
	{
		if (ui::IsButtonClicked(button))
		{
			return nextNode;
		}
		return stayNode;
	}
};

class RedNode : public BaseColorNode
{
public:
	RedNode()
		: BaseColorNode(RED, NODE_RED, NODE_GREEN, "Go Green")
	{}
};

class GreenNode : public BaseColorNode
{
public:
	GreenNode()
		: BaseColorNode(GREEN, NODE_GREEN, NODE_BLUE, "Go Blue")
	{}
};

class BlueNode : public BaseColorNode
{
public:
	BlueNode()
		: BaseColorNode(BLUE, NODE_BLUE, NODE_RED, "Back to Red")
	{}
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
