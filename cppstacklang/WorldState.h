#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include "Block.h"
#include "Data.h"
#include <deque>

class WorldState {
	public:
		WorldState();
		void printStack();
		
		std::vector<Block> blocks; // blocks[0] is the main
		std::deque<data> stack;
};

#endif
