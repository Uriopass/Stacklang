#ifndef VARIABLES_H
#define VARIABLES_H

#include "Data.h"
#include <vector>

class Variables {
	public:
		Variables();
		Variables(int total_variables);
		~Variables();
	
		std::vector<data> **variables;
		std::vector<int> *scopes;
	
		int total_variables;
	
		void scope_up();
		void scope_down();
		data access(int id);
		void store(int varId, data d);
		
};

#endif
