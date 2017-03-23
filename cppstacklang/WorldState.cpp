#include "WorldState.h"
#include <iostream>
#include "Printer.h"

WorldState::WorldState() {
	blocks = std::vector<Block*>();
	stack = std::deque<data>();
}

WorldState::~WorldState() {
	int block_size = blocks.size();
	for(int i = 0 ; i < block_size ; i++)
		delete blocks[i];
	delete variables;
}

void WorldState::printStack() {
	std::cout << "[";
	std::deque<data>::iterator It;
	for ( It = stack.begin(); It != stack.end(); It++ )
	{
		std::cout << Printer::out((*It));
    }
	std::cout << "]\n";	
}
