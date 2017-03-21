#ifndef TOKEN_H
#define TOKEN_H

#include <string>

typedef enum {
	INT,
	VAR_OPE,
	MAT_OPE
} token_type;

typedef union {
	int i;
	string s;
} token_value;

class Token {
	public:
		Token(token_value value, token_type type);
		token_value value;
		token_type type;
};

#endif
