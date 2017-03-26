#include <boost/variant.hpp>
#include <iostream>
#include <cstdlib>
#include <string>

#include "Token.h"
#include "Variables.h"
#include "Printer.h"
#include "Data.h"
#include "Interpreter.h"

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

void std_add(void* p) {
	Interpreter* ip = (Interpreter*)p;
	ip->executeMatOperator(ADD);
}

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

void std_dup(void* p) {
	Interpreter* ip = (Interpreter*)p;
	data* d = ip->ws->stack->back();
	ip->ws->stack->push_back(new data(*d));
}

void std_stoi(void* p) {
	Interpreter* ip = (Interpreter*)p;
	data* d = ip->pop_stack();
	ip->ws->stack->push_back(new data(std::stoi(boost::get<std::string>(*d))));
	delete d;
}

void init_rand() {
	srand(0);
}

void std_rand(void* p) {
	Interpreter* ip = (Interpreter*)p;
	data* d = ip->pop_stack();
	ip->ws->stack->push_back(new data(1+rand()%boost::get<int>(*d)));
}

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
		double a = boost::get<double>(*d2);
		
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


Variables::~Variables() {
	for(int i = 0 ; i < total_variables ; i++) {
		delete scopes[i];
		delete variables[i];
	}
}

Variables::Variables() {
	init_rand();
	var_names = std::unordered_map<std::string, int>();
	// Initialize libstd
	int count = 0;
	var_names.insert(std::make_pair("repeat", count++));
	var_names.insert(std::make_pair("add", count++));
	var_names.insert(std::make_pair("print", count++));
	var_names.insert(std::make_pair("dup", count++));
	var_names.insert(std::make_pair("rand", count++));
	var_names.insert(std::make_pair("output", count++));
	var_names.insert(std::make_pair("input", count++));
	var_names.insert(std::make_pair("stoi", count++));
	var_names.insert(std::make_pair("geq", count++));
	var_names.insert(std::make_pair("leq", count++));
	var_names.insert(std::make_pair("eq", count++));
	
}

void Variables::initVariables(int total_variables) {
	this->total_variables = total_variables;
	variables = new std::vector<data>*[total_variables];
	scopes = new std::vector<int>*[total_variables];
	
	for(int i = 0 ; i < total_variables ; i++) {
		scopes[i] = new std::vector<int>();
		variables[i] = new std::vector<data>();
	}
	int count = 0;
	variables[count++]->push_back(std_repeat);
	variables[count++]->push_back(std_add);
	variables[count++]->push_back(std_print);
	variables[count++]->push_back(std_dup);
	variables[count++]->push_back(std_rand);
	variables[count++]->push_back(std_output);
	variables[count++]->push_back(std_input);
	variables[count++]->push_back(std_stoi);
	variables[count++]->push_back(std_geq);
	variables[count++]->push_back(std_leq);
	variables[count++]->push_back(std_eq);
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
		if(cut < variables[i]->size())
		{
			scopes[i]->pop_back();
			variables[i]->erase(variables[i]->begin()+cut, variables[i]->end());
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

data Variables::access(int varId) {
	#ifdef DEBUG
	std::cout << "Accessing " << varId << std::endl;
	#endif
	std::vector<data> stack = *variables[varId];
	if(stack.size() == 0) {
		std::string name;
		for(auto& a : var_names) {
			if(a.second == varId) {
				name = a.first;
				break;
			}
		}
		throw std::string("Variable not defined : ") + std::string(name);
	}
	return stack.back();
}

void Variables::store(int varId, data d) {
	#ifdef DEBUG
	std::cout << "Storing " << varId << " with data " << Printer::out(d) << std::endl;
	#endif
	variables[varId]->push_back(d);
}
