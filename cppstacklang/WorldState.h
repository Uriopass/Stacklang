#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include "Data.h"
#include "Block.h"
#include "Variables.h"
#include <deque>
#include <stack>

class WorldState {
	public:
		WorldState();
		~WorldState();
		void printStack();
		int getVarAddress(const std::string& base);
		
		std::vector<Block*> blocks; // blocks[0] is the main
		std::deque<data*>* stack;
		Variables* variables;
};

#endif
