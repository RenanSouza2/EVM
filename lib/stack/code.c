#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "debug.h"

#ifdef DEBUG

#include "../bytes/bytes32/debug.h"

int stack_created;

#define STACK_INC() stack_created++;
#define STACK_DEC() \
    {   \
        assert(stack_created > 0);  \
        stack_created--;    \
    }

bool stack_memory()
{
    return stack_created == 0;
}

void stack_display(stack_t s)
{
    printf("\nStack count: %d", s.count);
    for(int i=0; i<s.count; i++)
    {
        printf("\nstack[%d]: ", i);
        bytes32_display(s.b[i]);
    }
    printf("\n");
}

#else
#define STACK_INC()
#define STACK_DEC()
#endif

stack_t stack_init()
{
    bytes32_p b = calloc(STACK_MAX, 32);
    assert(b);
    STACK_INC();

    return (stack_t){0, b};
}

void stack_free(stack_t s)
{
    STACK_DEC();
    free(s.b);
}

bool stack_push(stack_p s, bytes32_t b)
{
    if(s->count == STACK_MAX) return false;

    s->b[s->count++] = b;
    return true;
}

bool stack_pop(bytes32_p b, stack_p s)
{
    if(s->count == 0) return false;
    
    *b = s->b[--s->count];
    return true;
}

bool stack_dup(stack_p s, int index)
{
    if(s->count < index) return false;

    bytes32_t b = s->b[s->count - index];
    return stack_push(s, b);
}

bool stack_swap(stack_p s, int index)
{
    if(s->count < index+1) return false;

    bytes32_t b = s->b[s->count-1];
    s->b[s->count-1] = s->b[s->count-1 - index];
    s->b[s->count-1 - index] = b;
    return true;
}
