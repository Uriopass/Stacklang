#ifndef PARSER_H
#define PARSER_H

#include <string>
#include "WorldState.h"

class Parser {
	public:
		Parser();
		void feed(const std::string& code);
		int Parser::concatBlock(WorldState* ws, std::vector<Token>& tokens, int start);
		
		std::string code;
		WorldState* parse();
};

#endif
