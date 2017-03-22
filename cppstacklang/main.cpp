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
	WorldState ws = p.parse();
	
	Block test = Block(0);
	
	token_value v;
	v.i = 3;
	test.tokens.push_back(Token(v, TOK_INT));
	token_value v2;
	v2.i = 2;
	test.tokens.push_back(Token(v2, TOK_INT));
	token_value v3;
	std::string r = std::string("+");
	v3.s = &r;
	test.tokens.push_back(Token(v3, TOK_MAT_OPE));
	
	ws.blocks.push_back(test);
	
	Interpreter a = Interpreter(ws);
	try {
		a.execute();
	}
	catch ( char const* Msg ) 
	{ 
		std::cerr << "Error : " << Msg << std::endl; 
	}
	return 0;
}
