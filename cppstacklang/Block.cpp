#include <iostream>
#include "Printer.h"

#include "Block.h"

Block::Block(int id) : id(id) {
	tokens = std::vector<Token>();
}

void Block::add_token(Token t) {
	tokens.push_back(t);
}

void Block::print_content() {
	for(int i = 0 ; i < tokens.size() ; i++) {
		std::cout << Printer::out(tokens[i]) << std::endl;
	}
}