#include "Block.h"

Block::Block(int id) : id(id) {
	tokens = std::vector<Token>();
}

void add_token(Token t) {
	tokens.push_back(t);
}