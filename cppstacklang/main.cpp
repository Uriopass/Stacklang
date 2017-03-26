#include <iostream>
#include <fstream>

#include "Interpreter.h"
#include "Parser.h"
#include "Block.h"
#include "Token.h"

using namespace std;


int main(int argc, char** argv) {
	if(argc == 0) {
		std::cout << "No arguments given.. How to use : ./cppstacklang <filename>";
		return 0;
	}
	
	ifstream ifs(argv[1]);
  	string content( (std::istreambuf_iterator<char>(ifs) ),
                       (std::istreambuf_iterator<char>()    ) );
	
	Parser* p = new Parser();
	
	p->feed(content);
	
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
	#ifdef DEBUG
	cout << "Stack is now ";
	a->ws->printStack();
	#endif
	delete a;
	
	return 0;
}
