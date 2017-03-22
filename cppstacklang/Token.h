#ifndef TOKEN_H
#define TOKEN_H

#include <string>

typedef enum {
	TOK_INT,
	TOK_VAR_OPE,
	TOK_MAT_OPE
} token_type;

typedef union {
	int i;
	std::string *s;
} token_value;

class Token {
	public:
		Token(token_value value, token_type type);
		token_value value;
		token_type type;
};

#endif
