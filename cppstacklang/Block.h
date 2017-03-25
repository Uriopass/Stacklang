#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include "Token.h"

class Block {
	public:
		Block(int id);
		void add_token(Token t);
		void print_content();
		
		int id;
		std::vector<Token> tokens;
};

#endif
