#include "Interpreter.h"
#include <iostream>
#include <boost/variant.hpp>
#include "Printer.h"

Interpreter::Interpreter(WorldState* ws) {
	this->ws = ws;
}

Interpreter::~Interpreter() {
	delete ws;
}

data Interpreter::pop_stack() {
	data d = ws->stack.back();
	ws->stack.pop_back();
	return d;
}

void Interpreter::executeMatOperator(mat_ope_t ope) {
	switch(ope) {
		case ADD:
		{
			int a = boost::get<int>(pop_stack());
			int b = boost::get<int>(pop_stack());
			ws->stack.push_back(b+a);
		}
		break;
		case SUB:
		{
			int a = boost::get<int>(pop_stack());
			int b = boost::get<int>(pop_stack());
			ws->stack.push_back(b-a);
		}
		break;
		case MOD:
		{
			int a = boost::get<int>(pop_stack());
			int b = boost::get<int>(pop_stack());
			ws->stack.push_back(b%a);
		}
		default:
		break;
	}
}

void Interpreter::executeReference(var_t ref) {
	data d = ws->variables->access(ref.val);
	std::cout << "Executing reference : " << d.which() << std::endl;

	if(d.which() == DATA_V_BLO_REF) {
		std::cout << "Reference is block_ref id: " << boost::get<blo_ref_t>(d).ref << " scoping up" << std::endl;
		ws->variables->scope_up();
		executeBlock(boost::get<blo_ref_t>(d).ref);
		std::cout << "Scoping down" << std::endl;
		ws->variables->scope_down();
	} else {
		ws->stack.push_back(d);
	}
}

void Interpreter::executeVarOperator(var_ope_t ope) {
	switch(ope) {
		case DEF:
		{
			data stor = pop_stack();
			var_t v = boost::get<var_t>(pop_stack());
			ws->variables->store(v.val, stor);
		}
		break;
		case IF:
		{
			blo_ref_t false_b = boost::get<blo_ref_t>(pop_stack());
			blo_ref_t true_b = boost::get<blo_ref_t>(pop_stack());
			int condition = boost::get<int>(pop_stack());
			if(condition) {
				executeBlock(true_b.ref);
			} else {
				executeBlock(false_b.ref);
			}
		}
		break;
		
		default:
			break;
	}
}

void Interpreter::executeToken(Token t) {
	switch(t.type) {
		case TOK_INT:
			ws->stack.push_back(boost::get<int>(t.value));
		break;
			
		case TOK_MAT_OPE:
			executeMatOperator(boost::get<mat_ope_t>(t.value));
		break;
		
		case TOK_BLO_REF:
			ws->stack.push_back(boost::get<blo_ref_t>(t.value));
		break;
			
		case TOK_REF:
			executeReference(boost::get<var_t>(t.value));
		break;
			
		case TOK_VAR_OPE:
			executeVarOperator(boost::get<var_ope_t>(t.value));
		break;
			
		case TOK_REF_DEF:
			ws->stack.push_back(boost::get<var_t>(t.value));
		break;
		
		default:
			break;
	}
}

void Interpreter::executeBlock(int id) {
	Block &b = (*ws->blocks[id]);
	std::cout << "Executing block " << id << " size: " << b.tokens.size() << std::endl;
	
	for(unsigned int i = 0 ; i < b.tokens.size() ; i++)
	{
		std::cout << "\t tok type : ";
		std::cout << Printer::out(b.tokens[i].type) << std::endl;
		executeToken(b.tokens[i]);
		std::cout << "Stack after token execution is : ";
		ws->printStack();
	}
	std::cout << "Getting out of block " << id << std::endl;
	return;
}

void Interpreter::execute() {
	std::cout << "Number of blocks : " << ws->blocks.size() << std::endl;

	if(ws->blocks.size() == 0) {
		throw "Nothing to execute";
	}
	
	std::cout << "Starting program" << std::endl;
	executeBlock(0);
	std::cout << "Ending program" << std::endl;
}
