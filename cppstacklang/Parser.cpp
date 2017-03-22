#include "Parser.h"

Parser::Parser() {
	code = "";	
}

void Parser::feed(std::string code) {
	this->code.append(code);
}

WorldState Parser::parse() {
	WorldState ws = WorldState();
	
	code.clear();
	
	return ws;
}