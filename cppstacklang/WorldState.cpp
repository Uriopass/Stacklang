#include "WorldState.h"
#include <iostream>
#include "Printer.h"

WorldState::WorldState() {
	blocks = std::vector<Block*>();
	stack = std::deque<data>();
	var_names = std::unordered_map<std::string, int>();
}

WorldState::~WorldState() {
	int block_size = blocks.size();
	for(int i = 0 ; i < block_size ; i++)
		delete blocks[i];
	delete variables;
}

int WorldState::getVarAddress(const std::string& base) {
	std::unordered_map<std::string, int>::const_iterator got = var_names.find(base);
	if(got == var_names.end()) {
		int count = var_names.size();
		var_names.insert(std::make_pair<std::string,int>(base, count));
		return count;
	} else {
		return got->second;	
	}
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
