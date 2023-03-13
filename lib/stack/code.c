#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "debug.h"

#ifdef DEBUG

#include "../bytes32/debug.h"

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
