#ifndef INTEPRETER_H
#define INTEPRETER_H

#include "WorldState.h"

class Interpreter {
	public:
		Interpreter(WorldState ws);
		void execute();
		void executeBlock(Block b);
		void executeToken(Token t);
		void executeMatOperator(mat_ope_t ope);
		void executeVarOperator(var_ope_t ope);
		void executeReference(var_t ref);
		data pop_stack();
		
		WorldState _ws;
};

#endif
