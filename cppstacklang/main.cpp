#include <iostream>

#include "Interpreter.h"
#include "Parser.h"
#include "Block.h"
#include "Token.h"

using namespace std;


int main(int argc, char** argv) {
	#ifdef DEBUG
	cout << "Arguments are : (" << argc << ")" << endl;
	
	for(int i = 0 ; i < argc ; i++) {
		cout << "\t" << argv[i] << endl;
	}
	#endif
	
	Parser* p = new Parser();
	
	p->feed("5 {1 @} repeat");
	
	WorldState* ws;
	try {
		ws = p->parse();
		delete p;
	} catch( char const* Msg ) { 
		std::cerr << "Error : " << Msg << std::endl; 
		return 1;
	} catch( std::string s) {
		std::cerr << "Error : " << s << std::endl; 
		return 1;
	}
	/*
	
	*/
	
	Interpreter* a = new Interpreter(ws);
	
	try {
		a->execute();
	} catch ( char const* Msg ) { 
		std::cerr << "Error : " << Msg << std::endl; 
		return 1;
	} catch( std::string s) {
		std::cerr << "Error : " << s << std::endl; 
		return 1;
	}
	
	cout << "Stack is now ";
	a->ws->printStack();
	delete a;
	
	return 0;
}
