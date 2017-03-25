#include "Parser.h"
#include <iostream>
#include <cctype>
#include <string>
#include <unordered_set>
#include <utility>
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

std::pair<int, int> Parser::concatBlock(WorldState* ws, std::vector<Token>& tokens, int start) {
	int bId = ws->blocks.size();
	Block* b = new Block(bId);
	ws->blocks.push_back(b);
	
	for(int i = start ; i < tokens.size() ; i++) {
		Token t = tokens[i];
		if(t.type == TOK_BLO_DEL) {
			std::string tokval = boost::get<std::string>(t.value);
		
			if(tokval == "}") {
			
				return std::make_pair(i, bId);
			}
			if(tokval == "{") {
				std::pair<int, int> r = concatBlock(ws, tokens, i+1);
				i = r.first;
				blo_ref_t val;
				val.ref = r.second;
				Token t2(val, TOK_BLO_REF);
				b->add_token(t2);
			}
		} else {
			b->add_token(t);
		}		
	}
	throw "Error parsing blocks";
}


WorldState* Parser::parse() {

	WorldState* ws = new WorldState();
	
	std::vector<Token> tokens;
	int end = code.size();
	
	for(int it = 0 ; it < end; it++) {
		char c = code[it];
		std::cout << "Char is " << code[it] << std::endl;
		if(isdigit(c)) {
			std::string base("");
			while(isdigit(code[it]) and it !=end) {
				base += code[it];
				it++;
			}
			Token t(std::stoi(base), TOK_INT);
			tokens.push_back(t);
		} else if(is_string_delimiter(c)) {
			std::string base("");
			while(is_string_delimiter(code[it]) and it !=end) {
				base += code[it];
				it++;
			}
			Token t(base, TOK_STRING);
			tokens.push_back(t);
		} else if(isalpha(c) or c == '_') {
			std::string base("");
			while((isalnum(code[it]) or code[it] == '_') and it !=end) {
				std::cout << code[it] << std::endl;
				base += code[it];
				it++;
			}
			
			std::cout << "Parsed ref " << base << std::endl;
			var_t var;
			var.val = ws->getVarAddress(base);
			
			Token t(var, TOK_REF);
			tokens.push_back(t);
		} else if(c == '/' and (it+1) != end and isalpha(code[it+1])) {
			it += 1;
			std::string base("");
			while((isalnum(code[it]) or code[it] == '_') and it != end) {
				std::cout << code[it] << std::endl;
				base += code[it];
				it++;
			}
			std::cout << "Parsed ref def " << base << std::endl;
			
			var_t var;
			var.val = ws->getVarAddress(base);
			
			Token t(var, TOK_REF_DEF);
			tokens.push_back(t);
		} else if(isspace(c)) {
			
		} else if(is_block_delimiter(c)) {
			std::string base("");
			base += code[it];
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
	
	std::cout << "Raw token parsing (no block concat) gives : " << std::endl;
	for(int i = 0 ; i < tokens.size() ; i++) {
		Token t = tokens[i];
		std::cout << Printer::out(t) << std::endl;
	}
	
	Token endDel("}", TOK_BLO_DEL); // Emulate "main" block
	tokens.push_back(endDel);
	
	std::pair<int, int> r = concatBlock(ws, tokens, 0);
	
	if(r.first != tokens.size()) {
		std::cout << "Parsing is weird.." << r.first << " vs " << tokens.size() << std::endl;
	}
	if(r.second != 0) {
		std::cout << "Parsing is weird.." << r.second << std::endl;
	}
	
	std::cout << "Total var are : " << ws->var_names.size() << std::endl;
	for(auto i : ws->var_names) {
		std::cout << "\t" << i.first << " " << i.second << std::endl;
	}
	
	for(int i = 0 ; i < ws->blocks.size() ; i++) {
		std::cout << "Block id " << i << " contains " << std::endl;
		ws->blocks[i]->print_content();
	}
	
	ws->variables = new Variables(ws->var_names.size());
	
	code.clear(); // We don't need code anymore
	return ws;
}
