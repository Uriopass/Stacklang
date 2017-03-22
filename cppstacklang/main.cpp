#include <iostream>

#include "Interpreter.h"
#include "Parser.h"
#include "Block.h"
#include "Token.h"

using namespace std;

int main(int argc, char** argv) {
	cout << "Arguments are : (" << argc << ")" << endl;

	for(int i = 0 ; i < argc ; i++) {
		cout << "\t" << argv[i] << endl;
	}
	
	Parser p = Parser();
	
	p.feed("3 2 +");
	WorldState ws;
	try {
		ws = p.parse();
	} catch( char const* Msg ) { 
		std::cerr << "Error : " << Msg << std::endl; 
		return 1;
	} catch( std::string s) {
		std::cerr << "Error : " << s << std::endl; 
		return 1;
	}
	
	Block root = Block(0);
	
	root.tokens.push_back(Token((var_t){0}, TOK_REF_DEF));
	root.tokens.push_back(Token(3, TOK_INT));
	root.tokens.push_back(Token(DEF, TOK_VAR_OPE));
	root.tokens.push_back(Token((var_t){0}, TOK_REF));
	root.tokens.push_back(Token((var_t){0}, TOK_REF));
	root.tokens.push_back(Token(PLUS, TOK_MAT_OPE));
	
	ws.blocks.push_back(root);
	ws.variables = Variables(1);
	
	Interpreter a = Interpreter(ws);
	
	try {
		a.execute();
	} catch ( char const* Msg ) { 
		std::cerr << "Error : " << Msg << std::endl; 
		return 1;
	} catch( std::string s) {
		std::cerr << "Error : " << s << std::endl; 
		return 1;
	}
	
	cout << "Stack is now ";
	a._ws.printStack();
	
	return 0;
}
