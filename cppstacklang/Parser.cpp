#include "Parser.h"
#include <iostream>
#include <cctype>
#include <string>
#include <unordered_set>
#include "Token.h"
#include "Printer.h"
/*
typedef enum {
	TOK_INT,
	TOK_VAR_OPE,
	TOK_MAT_OPE,
	TOK_REF,
	TOK_REF_DEF,
	TOK_BLO_REF,
	TOK_BLO_DEL,
	TOK_UKN
} token_t;

typedef enum {
	ADD,
	SUB,
	MOD
} mat_ope_t;

typedef enum {
	DEF,
	IF
} var_ope_t;

#define TOKEN_VALUE_V_INT 0
#define TOKEN_VALUE_V_STRING 1
#define TOKEN_VALUE_V_VAR_OPE 2
#define TOKEN_VALUE_V_MAT_OPE 3
#define TOKEN_VALUE_V_VAR 4
#define TOKEN_VALUE_V_BLO_REF 4

typedef boost::variant<int, std::string, var_ope_t, mat_ope_t, var_t, blo_ref_t> token_value;
*/

Parser::Parser() {
	code = "";	
}

void Parser::feed(const std::string &code) {
	this->code.append(code);
}

bool is_string_delimiter(char c) {
	return c == '"' or c == '\'';
}

bool is_block_delimiter(char c) {
	return c == '{' or c == '}';
}

bool is_mat_ope(char c) {
	return c == '+' or  c == '-' or c == '%';	
}

bool is_var_ope(char c) {
	return c == '=' or  c == '?';	
}

mat_ope_t parse_mat_ope(char c) {
	switch(c) {
		case '+':
			return ADD;
		case '-':
			return SUB;
		case '%':
			return MOD;
		default:
			throw "Error parsing math operators";
	}
}

var_ope_t parse_var_ope(char c) {
	switch(c) {
		case '=':
			return DEF;
		case '?':
			return IF;
		default:
			throw "Error parsing var operators";
	}
}

int Parser::concatBlock(WorldState* ws, std::vector<Token>& tokens, int start) {
	Block* b = new Block(ws->blocks.size());
	
	for(int i = start ; i < tokens.size() ; i++) {
		Token t = tokens[i];
		if(t.type() == TOK_BLO_DEL) {
			if(t.value == "}") {
				return i;
			}
		} else {
		
		}
		
	}
}


WorldState* Parser::parse() {
	WorldState* ws = new WorldState();
	
	std::string::iterator it;
	
	std::vector<Token> tokens;
	
	for(it = code.begin() ; it != code.end(); it++) {
		char c = *it;
		
		if(isdigit(c)) {
			std::string base("");
			while(isdigit(*it) and it != code.end()) {
				base += *it;
				it++;
			}
			Token t(std::stoi(base), TOK_INT);
			tokens.push_back(t);
		} else if(is_string_delimiter(c)) {
			std::string base("");
			while(is_string_delimiter(*it) and it != code.end()) {
				base += *it;
				it++;
			}
			Token t(base, TOK_STRING);
			tokens.push_back(t);
		} else if(isalpha(c) or c == '_') {
			std::string base("");
			while((isalnum(c) or c == '_') and it != code.end()) {
				base += c;
				it++;
			}
			
			var_t var;
			var.val = ws->getVarAddress(base);
			
			Token t(var, TOK_REF);
			tokens.push_back(t);
		} else if(c == '/' and (it+1) != code.end() and isalpha(*(it+1))) {
			it += 1;
			std::string base("");
			while((isalnum(c) or c == '_') and it != code.end()) {
				base += *it;
				it++;
			}
			var_t var;
			var.val = ws->getVarAddress(base);
			
			Token t(var, TOK_REF_DEF);
			tokens.push_back(t);
		} else if(isspace(c)) {
			
		} else if(is_block_delimiter(c)) {
			std::string base("");
			base += *it;
			Token t(base, TOK_BLO_DEL);
			tokens.push_back(t);
		} else if(is_mat_ope(c)) {
			Token t(parse_mat_ope(c), TOK_MAT_OPE);
			tokens.push_back(t);
		} else if(is_var_ope(c)) {
			Token t(parse_var_ope(c), TOK_VAR_OPE);
			tokens.push_back(t);
		} else {
			std::string base("");
			base += c;
			Token t(base, TOK_UKN);
			std::cout << "Found unknown token : " << c << std::endl;
			tokens.push_back(t);
		}
	}
	
	code.clear(); // We don't need code anymore
	std::cout << "Raw token parsing (no block concat) gives : " << std::endl;
	for(int i = 0 ; i < tokens.size() ; i++) {
		Token t = tokens[i];
		std::cout << Printer::out(t) << std::endl;
	}
	
	Token endDel("}", TOK_BLO_DEL); // Emulate "main" block
	tokens.push_bacl(t);
	
	concatBlock(ws, tokens, 0, 0);
	
	ws->variables = new Variables(ws->var_names.size());
	
	return ws;
}
