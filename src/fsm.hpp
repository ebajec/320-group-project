#ifndef FSM_HPP
#define FSM_HPP

#include <memory>
#include <unordered_map>
#include <cstdint>

enum FSMStatus
{
	FSM_OK,
	FSM_ENODE_NOT_FOUND = -1,
	FSM_EUNKNOWN = -2,
};

struct FSMResult
{
	uint32_t node;
	FSMStatus status;

	FSMResult(uint32_t _node) : 
		node(_node) , status(FSM_OK)
	{}

	explicit operator bool() const {
        return status == FSM_OK;
    }
};

class IFSMNode
{
public:
	virtual ~IFSMNode() {}

	virtual void render() = 0;
	virtual FSMResult update() = 0;
};

class FSM
{
	std::unordered_map<
		uint32_t,
		std::unique_ptr<IFSMNode>
	> nodes;

	uint32_t current = 0;
public:
	FSM() 
	{
	}

	void add(uint32_t index, IFSMNode *node)
	{
		nodes[index] = std::unique_ptr<IFSMNode>(node);
	}

	void render() 
	{
		auto it = nodes.find(current);
		if (it == nodes.end())
			return;

		it->second->render();
	}

	FSMStatus update()
	{
		auto it = nodes.find(current);

		if (it == nodes.end())
			return FSM_ENODE_NOT_FOUND;

		FSMResult pair = it->second->update(); 

		current = pair.node;
		return pair.status;
	}
};

#endif //FSM_HPP
