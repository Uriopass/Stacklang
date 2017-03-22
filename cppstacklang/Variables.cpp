#include "Variables.h"
#include <iostream>

Variables::Variables() {
	
}

Variables::Variables(int total_variables) {
	this->total_variables = total_variables;
	variables = (std::vector<data>**) malloc(total_variables*sizeof(std::vector<data>*));
	scopes = (std::vector<int>*) malloc(total_variables*sizeof(std::vector<int>));
	
	for(int i = 0 ; i < total_variables ; i++) {
		scopes[i] = std::vector<int>();
		variables[i] = new std::vector<data>();
	}
}

//std::vector<data> *variables;

void Variables::scope_up() {
	for(int i = 0 ; i < total_variables ; i++) {
		scopes[i].push_back(variables[i]->size());
	}
}
void Variables::scope_down() {
	for(int i = 0 ; i < total_variables ; i++) {
		int cut = scopes[i].back();
		if(cut < variables[i]->size())
		{
			scopes[i].pop_back();
			variables[i]->erase(variables[i]->begin()+cut, variables[i]->end());
		}
	}
}

data Variables::access(int varId) {
	std::cout << "Accessing " << varId << std::endl;

	std::vector<data> stack = *variables[varId];
	if(stack.size() == 0) {
		throw "Variable not defined";
	}
	return stack.back();
}

void Variables::store(int varId, data d) {
	std::cout << "Storing " << varId << " with type " << d.which() << std::endl;
	variables[varId]->push_back(d);
}