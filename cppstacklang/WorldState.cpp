#include "WorldState.h"
#include <iostream>

WorldState::WorldState() {
	blocks = std::vector<Block>();
	stack = std::deque<data>();
}

void WorldState::printStack() {
	std::cout << "[";
	std::deque<data>::iterator It;
	for ( It = stack.begin(); It != stack.end(); It++ )
    	std::cout << *It <<  " ";
	std::cout << "]\n";	
}