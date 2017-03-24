#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include "Block.h"
#include "Data.h"
#include "Variables.h"
#include <deque>
#include <stack>
#include <unordered_map>

class WorldState {
	public:
		WorldState();
		~WorldState();
		void printStack();
		int getVarAddress(const std::string& base);
		
		std::vector<Block*> blocks; // blocks[0] is the main
		std::deque<data> stack;
		std::unordered_map<std::string, int> var_names; // debugging
		Variables* variables;
};

#endif
