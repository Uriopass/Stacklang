#include "interpreter.h"
#include <iostream>
#include <boost/variant.hpp>

Interpreter::Interpreter(WorldState ws) {
	_ws = ws;
}

void Interpreter::executeMatOperator(var_ope_t ope) {
	switch(ope) {
		case PLUS:
		{
			int a = boost::get<int>(_ws.stack.back());_ws.stack.pop_back();
			int b = boost::get<int>(_ws.stack.back());_ws.stack.pop_back();
			_ws.stack.push_back(b+a);
		}
		break;
		case MINUS:
		{
			int a = boost::get<int>(_ws.stack.back());_ws.stack.pop_back();
			int b = boost::get<int>(_ws.stack.back());_ws.stack.pop_back();
			_ws.stack.push_back(b-a);
		}
		break;
		case MOD:
		{
			int a = boost::get<int>(_ws.stack.back());_ws.stack.pop_back();
			int b = boost::get<int>(_ws.stack.back());_ws.stack.pop_back();
			_ws.stack.push_back(b%a);
		}
		default:
		break;
	}
}

void Interpreter::executeToken(Token t) {
	switch(t.type) {
		case TOK_INT:
			_ws.stack.push_back(t.value);
			break;
		case TOK_MAT_OPE:
			executeMatOperator(boost::get<var_ope_t>(t.value));
			break;
		default:
			break;
	}
}

void Interpreter::executeBlock(Block b) {
	std::cout << "Executing block " << b.id << std::endl;
	
	for(int i = 0 ; i < b.tokens.size() ; i++)
	{
		std::cout << "\t" << b.tokens[i].value << std::endl;
		executeToken(b.tokens[i]);
	}
	
}

void Interpreter::execute() {
	std::cout << "Number of blocks : " << _ws.blocks.size() << std::endl;

	if(_ws.blocks.size() == 0) {
		throw "Nothing to execute";
	}
	
	executeBlock(_ws.blocks[0]);
}