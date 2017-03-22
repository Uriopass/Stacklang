#ifndef INTEPRETER_H
#define INTEPRETER_H

#include "WorldState.h"

class Interpreter {
	public:
		Interpreter(WorldState ws);
		void execute();
		void executeBlock(Block b);
		void executeToken(Token t);
		void executeMatOperator(var_ope_t ope);
		
		
		WorldState _ws;
};

#endif
