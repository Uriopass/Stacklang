#include "Parser.h"

Parser::Parser() {
	code = "";	
}

void Parser::feed(std::string code) {
	this->code.append(code);
}

WorldState* Parser::parse() {
	WorldState* ws = new WorldState();
	
	code.clear();
	
	return ws;
}
