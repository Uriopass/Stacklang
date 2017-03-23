#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include "Block.h"
#include "Data.h"
#include "Variables.h"
#include <deque>
#include <stack>

class WorldState {
	public:
		WorldState();
		~WorldState();
		void printStack();
		
		std::vector<Block*> blocks; // blocks[0] is the main
		std::deque<data> stack;
		Variables variables;
};

#endif
