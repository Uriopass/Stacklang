#include "WorldState.h"
#include <iostream>

WorldState::WorldState() {
	blocks = std::vector<Block*>();
	stack = std::deque<data>();
}

WorldState::~WorldState() {
	int block_size = blocks.size();
	for(int i = 0 ; i < block_size ; i++)
		delete blocks[i];
}

void WorldState::printStack() {
	std::cout << "[";
	std::deque<data>::iterator It;
	for ( It = stack.begin(); It != stack.end(); It++ )
	{
		if((*It).which() == DATA_V_BLO_REF)
    		std::cout << boost::get<blo_ref_t>(*It).ref << "r ";
    	if((*It).which() == DATA_V_INT)
    		std::cout << boost::get<int>(*It) << " ";
    	if((*It).which() == DATA_V_DOUBLE)
    		std::cout << boost::get<double>(*It) << " ";
    	if((*It).which() == DATA_V_VAR)
    		std::cout << boost::get<var_t>(*It).val << "v ";
    	if((*It).which() == DATA_V_STRING)
    		std::cout << boost::get<std::string>(*It) << "v ";
    }
	std::cout << "]\n";	
}
