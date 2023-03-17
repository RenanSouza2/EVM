#include <stdio.h>
#include <assert.h>

#include "../debug.h"
#include "../../bytes/bytes32/debug.h"

void test_push()
{
    printf("\n\ttest push\t\t");

    stack_t s = stack_init();
    assert(stack_push(&s, b_one));
    assert(s.count == 1);
    ASSERT_BYTES32_UINT(s.b[0], 1);
    stack_free(s);

    s = stack_init();
    bytes32_t b = BYTES32_UINT(2);
    assert(stack_push(&s, b_one));
    assert(stack_push(&s, b));
    ASSERT_BYTES32_UINT(s.b[1], 2);
    stack_free(s);

    s = stack_init();
    for(int i=0; i<STACK_MAX; i++)
    {
        b = BYTES32_UINT(i);
        assert(stack_push(&s, b));
    }
    b = BYTES32_UINT(STACK_MAX);
    assert(stack_push(&s, b) == false);
    assert(s.count == STACK_MAX);
    stack_free(s);

    assert(stack_memory());
}

void test_pop()
{
    printf("\n\ttest pop\t\t");
    
    stack_t s = stack_init();
    bytes32_t b = BYTES32_UINT(2);
    assert(stack_push(&s, b_one));
    assert(stack_push(&s, b));
    
    assert(stack_pop(&b, &s));
    ASSERT_BYTES32_UINT(b, 2);
    assert(s.count == 1);
    
    assert(stack_pop(&b, &s));
    ASSERT_BYTES32_UINT(b, 1);
    assert(s.count == 0);

    assert(stack_pop(&b, &s) == false);
    stack_free(s);
    
    assert(stack_memory());
}

void test_stack()
{
    printf("\ntest stack\t\t");

    test_push();
    test_pop();
}


int main() 
{
    setbuf(stdout, NULL);
    test_stack();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
