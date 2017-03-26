#ifndef VALUE_H
#define VALUE_H
#include <boost/variant.hpp>

#define DATA_V_INT 0
#define DATA_V_DOUBLE 1
#define DATA_V_STRING 2
#define DATA_V_BLO_REF 3
#define DATA_V_VAR 4
#define DATA_V_FUNC 5

typedef struct {int ref;} blo_ref_t;
typedef struct {int val;} var_t;

typedef void(*func_t)(void*);

typedef boost::variant<int, double, std::string, blo_ref_t, var_t, func_t> data;

#endif