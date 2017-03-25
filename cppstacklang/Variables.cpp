#include <boost/variant.hpp>
#include <iostream>
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
	data* d = ip->pop_stack();
	data* d2 = ip->pop_stack();
	int a = boost::get<int>(*d2);
	int b = boost::get<int>(*d);
	ip->ws->stack->push_back(new data(a+b));
	delete d;
	delete d2;
}

Variables::Variables() {
	var_names = std::unordered_map<std::string, int>();
	// Initialize libstd
	int count = 0;
	var_names.insert(std::make_pair("repeat", count++));
	var_names.insert(std::make_pair("add", count++));
}

Variables::~Variables() {
	for(int i = 0 ; i < total_variables ; i++) {
		delete scopes[i];
		delete variables[i];
	}
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
}

//std::vector<data> *variables;

void Variables::scope_up() {
	for(int i = 0 ; i < total_variables ; i++) {
		scopes[i]->push_back(variables[i]->size());
	}
}
void Variables::scope_down() {
	for(int i = 0 ; i < total_variables ; i++) {
		int cut = scopes[i]->back();
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
		throw "Variable not defined";
	}
	return stack.back();
}

void Variables::store(int varId, data d) {
	#ifdef DEBUG
	std::cout << "Storing " << varId << " with data " << Printer::out(d) << std::endl;
	#endif
	variables[varId]->push_back(d);
}
