#include "interpreter.h"
#include <iostream>

Interpreter::Interpreter(WorldState ws) {
	_ws = ws;
}

void executeBlock(Block b) {
	std::cout << "Executing block " << b.id << std::endl;
	
	for(int i = 0 ; i < b.tokens.size() ; i++)
	{
		if(b.tokens[i].type == TOK_INT)
			std::cout << "\t" << b.tokens[i].value.i << std::endl;
		else
			std::cout << "\t" << *b.tokens[i].value.s << std::endl;
			
	}
	
}

void Interpreter::execute() {
	std::cout << "Number of blocks : " << _ws.blocks.size() << std::endl;

	if(_ws.blocks.size() == 0) {
		throw "Nothing to execute";
	}
	
	executeBlock(_ws.blocks[0]);
}