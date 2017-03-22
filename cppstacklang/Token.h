#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include "Data.h"
#include <boost/variant.hpp>

typedef enum {
	TOK_INT,
	TOK_VAR_OPE,
	TOK_MAT_OPE,
	TOK_REF,
	TOK_REF_DEF
} token_t;

typedef enum {
	PLUS,
	MINUS,
	MOD
} mat_ope_t;

typedef enum {
	DEF
} var_ope_t;

typedef boost::variant<int, std::string, var_ope_t, mat_ope_t, var_t, blo_ref_t> token_value;

class Token {
	public:
		Token(token_value value, token_t type);
		token_value value;
		token_t type;
};

#endif
