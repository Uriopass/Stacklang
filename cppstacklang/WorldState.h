#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include "Block.h"
#include "Data.h"
#include <stack>

class WorldState {
	public:
		WorldState();
		std::vector<Block> blocks; // blocks[0] is the main
		std::stack<data> stack;
};

#endif
