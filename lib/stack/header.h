#ifndef __STACK_H__
#define __STACK_H__

#include "../../utils/struct.h"
#include "../bytes32/header.h"

#define STACK_MAX 1024

PLACEHOLDER(bytes32_list);

STRUCT(stack)
{
    int count;
    bytes32_list_p bl;
};

stack_t stack_init();
void stack_push(stack_p s, bytes32_t b);
bytes32_t stack_pop(stack_p s);

#endif
