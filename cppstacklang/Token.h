#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <boost/variant.hpp>

typedef enum {
	TOK_INT,
	TOK_VAR_OPE,
	TOK_MAT_OPE
} token_t;

typedef enum {
	PLUS,
	MINUS,
	MOD
} var_ope_t;

typedef boost::variant<int, std::string, var_ope_t> token_value;

class Token {
	public:
		Token(token_value value, token_t type);
		token_value value;
		token_t type;
};

#endif
