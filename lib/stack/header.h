#ifndef __STACK_H__
#define __STACK_H__

#include "../../utils/struct.h"
#include "../bytes32/header.h"

#define STACK_MAX 1024

STRUCT(stack)
{
    int count;
    bytes32_p b;
};

stack_t stack_init();
void stack_free(stack_t s);

bool stack_push(stack_p s, bytes32_t b);
bool stack_pop(bytes32_p b32, stack_p s);

bool stack_dup(stack_p s, int index);
bool stack_swap(stack_p s, int index);

#endif
