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

data* Interpreter::pop_stack() {
	data* d = ws->stack->back();
	ws->stack->pop_back();
	return d;
}

void Interpreter::executeMatOperator(mat_ope_t ope) {
	switch(ope) {
		case ADD:
		{
			data* d = pop_stack();
			data* d2 = pop_stack();
			int a = boost::get<int>(*d);
			int b = boost::get<int>(*d2);
			delete d;
			delete d2;
			ws->stack->push_back(new data(b+a));
		}
		break;
		case SUB:
		{
			data* d = pop_stack();
			data* d2 = pop_stack();
			int a = boost::get<int>(*d);
			int b = boost::get<int>(*d2);
			delete d;
			delete d2;
			ws->stack->push_back(new data(b-a));
		}
		break;
		case MOD:
		{
			data* d = pop_stack();
			data* d2 = pop_stack();
			int a = boost::get<int>(*d);
			int b = boost::get<int>(*d2);
			delete d;
			delete d2;
			ws->stack->push_back(new data(b%a));
		}
		default:
		break;
	}
}

void Interpreter::executeReference(var_t ref) {
	data d = ws->variables->access(ref.val);
	#ifdef DEBUG
	std::cout << "Executing reference : " << d.which() << std::endl;
	#endif
	if(d.which() == DATA_V_BLO_REF) {
		int bid = boost::get<blo_ref_t>(d).ref;
		#ifdef DEBUG
		std::cout << "Reference is block_ref id: " << bid << " scoping up" << std::endl;
		#endif
		
		ws->variables->scope_up();
		executeBlock(bid);
		
		#ifdef DEBUG
		std::cout << "Scoping down" << std::endl;
		#endif
		ws->variables->scope_down();
	} else if(d.which() == DATA_V_FUNC) {
		boost::get<func_t>(d)(this);
	} else {
		ws->stack->push_back(new data(d));
	}
}

void Interpreter::executeVarOperator(var_ope_t ope) {
	switch(ope) {
		case DEF:
		{
			data* stor = pop_stack();
			data* d = pop_stack();
			var_t v = boost::get<var_t>(*d);
			ws->variables->store(v.val, *stor);
			delete d;
		}
		break;
		case IF:
		{
			data* d = pop_stack();
			data* d2 = pop_stack();
			data* d3 = pop_stack();
			blo_ref_t false_b = boost::get<blo_ref_t>(*d);
			blo_ref_t true_b = boost::get<blo_ref_t>(*d2);
			int condition = boost::get<int>(*d3);
			if(condition) {
				executeBlock(true_b.ref);
			} else {
				executeBlock(false_b.ref);
			}
			delete d;
			delete d2;
			delete d3;
		}
		break;
		
		default:
			break;
	}
}

void Interpreter::executeToken(Token t) {
	switch(t.type) {
		case TOK_INT:
			ws->stack->push_back(new data(boost::get<int>(t.value)));
		break;
			
		case TOK_MAT_OPE:
			executeMatOperator(boost::get<mat_ope_t>(t.value));
		break;
		
		case TOK_BLO_REF:
			ws->stack->push_back(new data(boost::get<blo_ref_t>(t.value)));
		break;
			
		case TOK_REF:
			executeReference(boost::get<var_t>(t.value));
		break;
			
		case TOK_VAR_OPE:
			executeVarOperator(boost::get<var_ope_t>(t.value));
		break;
			
		case TOK_REF_DEF:
			ws->stack->push_back(new data(boost::get<var_t>(t.value)));
		break;
		
		case TOK_UKN:
		{
			std::string val = boost::get<std::string>(t.value);
			data* d = pop_stack();
			if(val == "@") {
				std::cout << Printer::out(*d) << std::endl;
			}
			delete d;
		}
		break;
		
		default:
			break;
	}
}

void Interpreter::executeBlock(int id) {
	Block &b = (*ws->blocks[id]);
	#ifdef DEBUG
	std::cout << "Executing block " << id << " size: " << b.tokens.size() << std::endl;
	#endif
	for(unsigned int i = 0 ; i < b.tokens.size() ; i++)
	{
		#ifdef DEBUG
		std::cout << "\t tok type : ";
		std::cout << Printer::out(b.tokens[i].type) << std::endl;
		#endif
		
		executeToken(b.tokens[i]);
		
		#ifdef DEBUG
		std::cout << "Stack after token execution is : ";
		ws->printStack();
		#endif
	}
	#ifdef DEBUG
	std::cout << "Getting out of block " << id << std::endl;
	#endif
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
