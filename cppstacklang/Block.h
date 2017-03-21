#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include "Token.h"

class Block {
	public:
		Block(int id);
		int id;
		std::vector<Token> tokens;
};

#endif
