#ifndef VARIABLES_H
#define VARIABLES_H

#include "Data.h"
#include <vector>
#include <unordered_map>
#include <string>

class Variables {
	public:
		std::vector<data>** variables;
		std::vector<int>** scopes;
		std::unordered_map<std::string, int> var_names; // debug and libstd
	
		int total_variables;
	
		Variables();
		~Variables();
	
		std::string findName(int varId);
		void initVariables(int total_variables);
		int  getVarAddress(const std::string& base);
		void scope_up();
		void scope_down();
		data access(int id);
		void store(int varId, data d);
		
};

#endif
