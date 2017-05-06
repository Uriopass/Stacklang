#include <boost/variant.hpp>
#include <iostream>
#include <cstdlib>
#include <string>
#include <algorithm>

#include "Token.h"
#include "Variables.h"
#include "Printer.h"
#include "Data.h"
#include "Interpreter.h"

// START STDLIB
// LOOPS
void std_repeat(void* p) {
	Interpreter* ip = (Interpreter*)p;
	data* d = ip->pop_stack();
	blo_ref_t block = boost::get<blo_ref_t>(*d);
	data* d2 = ip->pop_stack();
	int times = boost::get<int>(*d2);
	for(int i = 0 ; i < times ; i++) {
		ip->executeBlock(block.ref);
	}
	delete d;
	delete d2;
}

void std_while(void* p) {
	Interpreter* ip = (Interpreter*)p;
	
	data* d = ip->pop_stack();
	blo_ref_t block = boost::get<blo_ref_t>(*d);
	
	data* d2 = ip->pop_stack();
	blo_ref_t cond = boost::get<blo_ref_t>(*d2);
	
	while(true) {
		ip->executeBlock(cond.ref);
	
		data* c = ip->pop_stack();
		int ok = boost::get<int>(*c);
		delete c;
		if(!ok) {
			break;
		}
		ip->executeBlock(block.ref);
	}
	
	delete d;
	delete d2;
}

// VAR OPE SUGAR
void std_if(void* p) {
	Interpreter* ip = (Interpreter*)p;
	ip->executeVarOperator(IF);
}

void std_def(void* p) {
	Interpreter* ip = (Interpreter*)p;
	ip->executeVarOperator(DEF);
}

// I/O

void std_print(void* p) {
	Interpreter* ip = (Interpreter*)p;
	data* d = ip->pop_stack();
	std::cout << Printer::out(*d) << '\n';
	delete d;
}

void std_output(void* p) {
	Interpreter* ip = (Interpreter*)p;
	data* d = ip->pop_stack();
	std::cout << Printer::out(*d);
	delete d;
}

void std_input(void* p) {
	Interpreter* ip = (Interpreter*)p;
	std::string in;
	std::cin >> in;
	ip->ws->stack->push_back(new data(in));
}

// STACK OPE

void std_exch(void* p) {
	Interpreter* ip = (Interpreter*)p;
	data* d = ip->pop_stack();
	std::iter_swap(ip->ws->stack->end()-1,ip->ws->stack->end()-1-boost::get<int>(*d));
	delete d;
}

void std_swap(void* p) {
	Interpreter* ip = (Interpreter*)p;
	std::iter_swap(ip->ws->stack->end()-1,ip->ws->stack->end()-2);
}

void std_getpush(void* p) {
	Interpreter* ip = (Interpreter*)p;
	data* d = ip->pop_stack();
	int offset = boost::get<int>(*d);
	delete d;
	
	data* d2 = *(ip->ws->stack->end()-1-offset);
	ip->ws->stack->push_back(new data(*d2));
}

void std_dup(void* p) {
	Interpreter* ip = (Interpreter*)p;
	data* d = ip->ws->stack->back();
	ip->ws->stack->push_back(new data(*d));
}

void std_pop(void* p) {
	Interpreter* ip = (Interpreter*)p;
	delete ip->pop_stack();
}
// CONVERSIONS
void std_stoi(void* p) {
	Interpreter* ip = (Interpreter*)p;
	data* d = ip->pop_stack();
	int i = std::stoi(boost::get<std::string>(*d));
	delete d;
	ip->ws->stack->push_back(new data(i));
}

void std_str(void* p) {
	Interpreter* ip = (Interpreter*)p;
	data* d = ip->pop_stack();
	if(d->which() == DATA_V_INT)
		ip->ws->stack->push_back(new data(std::to_string(boost::get<int>(*d))));
	else if(d->which() == DATA_V_DOUBLE)
		ip->ws->stack->push_back(new data(std::to_string(boost::get<double>(*d))));
	delete d;
}

// RANDOM
void init_rand() {
	srand(0);
}

void std_rand(void* p) {
	Interpreter* ip = (Interpreter*)p;
	data* d = ip->pop_stack();
	ip->ws->stack->push_back(new data(1+rand()%boost::get<int>(*d)));
	delete d;
}

// BOOLEANS OP
int compare(data* d, data* d2) {
	int w1 = (*d).which();
	int w2 = (*d2).which();
	if(w1 == DATA_V_INT) {
		int a = boost::get<int>(*d);
		
		if(w2 == DATA_V_INT) {
			int b = boost::get<int>(*d2);
			return a == b ? 0 : (a > b ? 1 : -1);
		} else {
			double b = boost::get<double>(*d2);
			return a == b ? 0 : (a > b ? 1 : -1);
		}
	} else {
		double a = boost::get<double>(*d);
		
		if(w2 == DATA_V_INT) {
			int b = boost::get<int>(*d2);
			return a == b ? 0 : (a > b ? 1 : -1);
		} else {
			double b = boost::get<double>(*d2);
			return a == b ? 0 : (a > b ? 1 : -1);
		}
	}
}

void std_leq(void* p) {
	Interpreter* ip = (Interpreter*)p;
	data* d = ip->pop_stack();
	data* d2 = ip->pop_stack();
	
	ip->ws->stack->push_back(new data(compare(d2, d) <= 0));
	
	delete d;
	delete d2;
}

void std_geq(void* p) {
	Interpreter* ip = (Interpreter*)p;
	data* d = ip->pop_stack();
	data* d2 = ip->pop_stack();
	
	ip->ws->stack->push_back(new data(compare(d2, d) >= 0));
	
	delete d;
	delete d2;
}

void std_eq(void* p) {
	Interpreter* ip = (Interpreter*)p;
	data* d = ip->pop_stack();
	data* d2 = ip->pop_stack();
	
	ip->ws->stack->push_back(new data(compare(d2, d) == 0));
	
	delete d;
	delete d2;
}

void std_neq(void* p) {
	Interpreter* ip = (Interpreter*)p;
	data* d = ip->pop_stack();
	data* d2 = ip->pop_stack();
	
	ip->ws->stack->push_back(new data(compare(d2, d) != 0));
	
	delete d;
	delete d2;
}

// END STDLIB

Variables::~Variables() {
	for(int i = 0 ; i < total_variables ; i++) {
		delete scopes[i];
		for(int j = 0 ; j < variables[i]->size() ; j++)
		{
			delete variables[i]->back();
			variables[i]->pop_back();
		}
		delete variables[i];
	}
	delete[] variables;
	delete[] scopes;
}

Variables::Variables() {
	init_rand();
	var_names = std::unordered_map<std::string, int>();
	// Initialize libstd
	int count = 0;
	var_names.insert(std::make_pair("repeat", count++));
	var_names.insert(std::make_pair("if", count++));
	var_names.insert(std::make_pair("print", count++));
	var_names.insert(std::make_pair("exch", count++));
	var_names.insert(std::make_pair("swap", count++));
	var_names.insert(std::make_pair("getpush", count++));
	var_names.insert(std::make_pair("dup", count++));
	var_names.insert(std::make_pair("pop", count++));
	var_names.insert(std::make_pair("rand", count++));
	var_names.insert(std::make_pair("output", count++));
	var_names.insert(std::make_pair("input", count++));
	var_names.insert(std::make_pair("stoi", count++));
	var_names.insert(std::make_pair("str", count++));
	var_names.insert(std::make_pair("geq", count++));
	var_names.insert(std::make_pair("leq", count++));
	var_names.insert(std::make_pair("eq", count++));
	var_names.insert(std::make_pair("neq", count++));
	var_names.insert(std::make_pair("while", count++));
	var_names.insert(std::make_pair("def", count++));
	
}

void Variables::initVariables(int total_variables) {
	this->total_variables = total_variables;
	
	variables = new std::vector<data*>*[total_variables];
	scopes = new std::vector<int>*[total_variables];
	
	for(int i = 0 ; i < total_variables ; i++) {
		scopes[i] = new std::vector<int>();
		scopes[i]->push_back(0);
		variables[i] = new std::vector<data*>();
	}
	int count = 0;
	variables[count++]->push_back(new data(std_repeat));
	variables[count++]->push_back(new data(std_if));
	variables[count++]->push_back(new data(std_print));
	variables[count++]->push_back(new data(std_exch));
	variables[count++]->push_back(new data(std_swap));
	variables[count++]->push_back(new data(std_getpush));
	variables[count++]->push_back(new data(std_dup));
	variables[count++]->push_back(new data(std_pop));
	variables[count++]->push_back(new data(std_rand));
	variables[count++]->push_back(new data(std_output));
	variables[count++]->push_back(new data(std_input));
	variables[count++]->push_back(new data(std_stoi));
	variables[count++]->push_back(new data(std_str));
	variables[count++]->push_back(new data(std_geq));
	variables[count++]->push_back(new data(std_leq));
	variables[count++]->push_back(new data(std_eq));
	variables[count++]->push_back(new data(std_neq));
	variables[count++]->push_back(new data(std_while));
	variables[count++]->push_back(new data(std_def));
}

//std::vector<data> *variables;

void Variables::scope_up() {
	for(int i = 0 ; i < total_variables ; i++) {
		scopes[i]->push_back(variables[i]->size());
	}
}
void Variables::scope_down() {
	for(int i = 0 ; i < total_variables ; i++) {
		unsigned int cut = scopes[i]->back();
		scopes[i]->pop_back();
		if(cut < variables[i]->size())
		{
			for(int j = 0 ; j < variables[i]->size()-cut ; j++)
			{
				delete variables[i]->back();
				variables[i]->pop_back();
			}
		}
	}
}

int Variables::getVarAddress(const std::string& base) {
	std::unordered_map<std::string, int>::const_iterator got = var_names.find(base);
	if(got == var_names.end()) {
		int count = var_names.size();
		var_names.insert(std::make_pair(base, count));
		return count;
	} else {
		return got->second;	
	}
} 

std::string Variables::findName(int varId) {
	std::string name;
	for(auto& a : var_names) {
		if(a.second == varId) {
			name = a.first;
			break;
		}
	}
	return name;
}

data* Variables::access(int varId) {
	#ifdef DEBUG
	std::cout << "Accessing " << varId << " (" << findName(varId) << ")" << std::endl;
	#endif
	std::vector<data*> stack = *variables[varId];
	if(stack.size() == 0) {
		std::string name = findName(varId);
		throw std::string("Variable not defined : ") + std::string(name);
	}
	return stack.back();
}

void Variables::store(int varId, data* d) {
	#ifdef DEBUG
	std::cout << "Storing " << varId << " with data " << Printer::out(*d) << std::endl;
	#endif
	if(scopes[varId]->back() == variables[varId]->size()) {
		#ifdef DEBUG
		std::cout << "Storing by pushing" << std::endl;
		#endif
		variables[varId]->push_back(d);
	} else {
		#ifdef DEBUG
		std::cout << "Storing in place" << std::endl;
		#endif
		data* p = variables[varId]->back();
		*p = *d;
		delete d;
	}
}
