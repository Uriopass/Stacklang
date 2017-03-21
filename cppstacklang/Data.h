#ifndef VALUE_H
#define VALUE_H

typedef union{
	int i;
	std::string *s;
	double d;
	std::string *ref;
} data;

#endif