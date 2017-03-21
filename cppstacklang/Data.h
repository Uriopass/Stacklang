#ifndef VALUE_H
#define VALUE_H

typedef union{
	int i;
	string s;
	double d;
	string ref;
} data;

#endif