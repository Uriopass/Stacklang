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
	TOK_REF_DEF,
	TOK_BLO_REF,
	TOK_BLO_DEL,
	TOK_STRING,
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
#define TOKEN_VALUE_V_BLO_REF 5

typedef boost::variant<int, std::string, var_ope_t, mat_ope_t, var_t, blo_ref_t> token_value;

class Token {
	public:
		Token(token_value value, token_t type);
		token_value value;
		token_t type;
};

#endif
