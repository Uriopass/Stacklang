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
	} catch ( char const* Msg ) { 
		std::cerr << "Error : " << Msg << std::endl; 
		return 1;
	} catch( std::string s) {
		std::cerr << "Error : " << s << std::endl; 
		return 1;
	}
	
	Block test = Block(0);
	
	test.tokens.push_back(Token(3, TOK_INT));
	test.tokens.push_back(Token(2, TOK_INT));
	test.tokens.push_back(Token(PLUS, TOK_MAT_OPE));
	
	test.tokens.push_back(Token(3, TOK_INT));
	test.tokens.push_back(Token(2, TOK_INT));
	test.tokens.push_back(Token(PLUS, TOK_MAT_OPE));
	test.tokens.push_back(Token(PLUS, TOK_MAT_OPE));
	
	ws.blocks.push_back(test);
	
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
