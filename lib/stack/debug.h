#ifndef __STACK_DEBUG_H__
#define __STACK_DEBUG_H__

#include "header.h"

#ifdef DEBUG

#define STACK_UINT(S, UINT) \
    assert(stack_push(&S, BYTES32_UINT(UINT)))

void stack_display(stack_t s);
bool stack_memory();

#endif

#endif
