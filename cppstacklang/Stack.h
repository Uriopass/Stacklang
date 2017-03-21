#ifndef STACK_H
#define STACK_H

#include "Data.h"

typedef struct {
	Node* parent, child;
	data_t value;
} Node;

#endif