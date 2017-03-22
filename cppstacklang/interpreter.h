#ifndef INTEPRETER_H
#define INTEPRETER_H

#include "WorldState.h"

class Interpreter {
	public:
		Interpreter(WorldState ws);
		void execute();
		
		WorldState _ws;
};

#endif
