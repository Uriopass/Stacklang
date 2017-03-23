#include "Interpreter.h"
#include <iostream>
#include <boost/variant.hpp>

Interpreter::Interpreter(WorldState ws) {
	_ws = ws;
}

data Interpreter::pop_stack() {
	data d = _ws.stack.back();
	_ws.stack.pop_back();
	return d;
}

void Interpreter::executeMatOperator(mat_ope_t ope) {
	switch(ope) {
		case PLUS:
		{
			int a = boost::get<int>(pop_stack());
			int b = boost::get<int>(pop_stack());
			_ws.stack.push_back(b+a);
		}
		break;
		case MINUS:
		{
			int a = boost::get<int>(pop_stack());
			int b = boost::get<int>(pop_stack());
			_ws.stack.push_back(b-a);
		}
		break;
		case MOD:
		{
			int a = boost::get<int>(pop_stack());
			int b = boost::get<int>(pop_stack());
			_ws.stack.push_back(b%a);
		}
		default:
		break;
	}
}

void Interpreter::executeReference(var_t ref) {
	data d = _ws.variables.access(ref.val);
	std::cout << "Executing reference : " << d.which() << std::endl;

	if(d.which() == DATA_V_BLO_REF) {
		std::cout << boost::get<blo_ref_t>(d).ref << std::endl;
		_ws.variables.scope_up();
		executeBlock(_ws.blocks[boost::get<blo_ref_t>(d).ref]);
		_ws.variables.scope_down();
	} else {
		_ws.stack.push_back(d);
	}
}

void Interpreter::executeVarOperator(var_ope_t ope) {
	switch(ope) {
		case DEF:
		{
			data stor = pop_stack();
			var_t v = boost::get<var_t>(pop_stack());
			_ws.variables.store(v.val, stor);
		}
		break;
		default:
			break;
	}
}

void Interpreter::executeToken(Token t) {
	switch(t.type) {
		case TOK_INT:
			_ws.stack.push_back((int)boost::get<int>(t.value));
		break;
			
		case TOK_MAT_OPE:
			executeMatOperator(boost::get<mat_ope_t>(t.value));
		break;
			
		case TOK_REF:
			executeReference(boost::get<var_t>(t.value));
		break;
			
		case TOK_VAR_OPE:
			executeVarOperator(boost::get<var_ope_t>(t.value));
		break;
			
		case TOK_REF_DEF:
			_ws.stack.push_back(boost::get<var_t>(t.value));
		break;
		
		default:
			break;
	}
}

void Interpreter::executeBlock(Block b) {
	std::cout << "Executing block " << b.id << std::endl;
	
	for(int i = 0 ; i < b.tokens.size() ; i++)
	{
		std::cout << "\t" << b.tokens[i].type << std::endl;
		_ws.printStack();
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