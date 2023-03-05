#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "debug.h"

#ifdef DEBUG

#include "../bytes32/debug.h"

void stack_display(stack_t s)
{
    printf("\nStack count: %d", s.count);
    bytes32_list_p bl = s.bl;
    for(int i=0; bl; i++, bl = bl->next)
    {
        printf("\nstack[%d]: ", i);bytes32_display(bl->b);
    }
    printf("\n");
}

#endif

bytes32_list_p stack_list_crate(bytes32_t b, bytes32_list_p next)
{
    bytes32_list_p bl = calloc(1, sizeof(bytes32_list_t));
    assert(bl);

    bl->b = b;
    bl->next = next;
    return bl;
}

stack_t stack_init()
{
    return (stack_t){0, NULL};
}

bool stack_push(stack_p s, bytes32_t b)
{
    if(s->count == STACK_MAX) return false;

    s->bl = stack_list_crate(b, s->bl);
    s->count++;
    return true;
}

bool stack_pop(bytes32_p b32, stack_p s)
{
    if(s->count == 0) return false;
    
    bytes32_list_p bl = s->bl;
    s->bl = bl->next;

    *b32 = bl->b;
    free(bl);

    s->count--;
    return true;
}
