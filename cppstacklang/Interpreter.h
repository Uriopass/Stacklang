#ifndef INTEPRETER_H
#define INTEPRETER_H

#include "WorldState.h"

class Interpreter {
	public:
		Interpreter(WorldState *ws);
		~Interpreter();
		void execute();
		void executeBlock(int id);
		void executeToken(Token& t);
		void executeMatOperator(mat_ope_t ope);
		void executeVarOperator(var_ope_t ope);
		void executeReference(var_t ref);
		data* pop_stack();
		
		#ifdef DEBUG
		int cur_block;
		Token* cur_token;
		#endif
		
		WorldState* ws;
};

#endif
