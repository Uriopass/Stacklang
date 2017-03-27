#include "WorldState.h"
#include <iostream>
#include "Printer.h"

WorldState::WorldState() {
	blocks = std::vector<Block*>();
	stack = new std::deque<data*>();
	variables = new Variables();
	var_stack = new std::deque<var_t>();
}

WorldState::~WorldState() {
	int block_size = blocks.size();
	for(int i = 0 ; i < block_size ; i++)
		delete blocks[i];
	delete variables;
	delete stack;
}

int WorldState::getVarAddress(const std::string& base) {
	return variables->getVarAddress(base);
} 

void WorldState::printStack() {
	std::cout << "[";
	std::deque<data*>::iterator It;
	for ( It = stack->begin(); It != stack->end(); It++ )
	{
		std::cout << Printer::out(*(*It)) << " ";
    }
	std::cout << "]\n";	
}
