#ifndef VALUE_H
#define VALUE_H

#include <boost/variant.hpp>

#define DATA_INT 0
#define DATA_DOUBLE 1
#define DATA_STRING 2

typedef boost::variant<int, double, std::string> data;

#endif