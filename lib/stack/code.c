#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "debug.h"

#ifdef DEBUG
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

void stack_push(stack_p s, bytes32_t b)
{
    assert(s->count < STACK_MAX);
    s->bl = stack_list_crate(b, s->bl);
    s->count++;
}

bytes32_t stack_pop(stack_p s)
{
    assert(s->count > 0);
    
    bytes32_list_p bl = s->bl;
    s->bl = bl->next;

    bytes32_t b = bl->b;
    free(bl);

    s->count--;
    return b;
}
