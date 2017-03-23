#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "WorldState.h"

class Parser {
	public:
		Parser();
		void feed(std::string code);
		
		std::string code;
		WorldState* parse();
};

#endif
