#include <stdio.h>
#include <assert.h>

#include "../debug.h"
#include "../../bytes/bytes32/debug.h"



void test_push()
{
    printf("\n\ttest push\t\t");

    stack_t s = stack_init();
    STACK_UINT(s, 1);
    assert(s.count == 1);
    ASSERT_BYTES32_UINT(s.b[0], 1);
    stack_free(s);

    s = stack_init();
    STACK_UINT(s, 1);
    STACK_UINT(s, 2);
    assert(s.count == 2);
    ASSERT_BYTES32_UINT(s.b[1], 2);
    stack_free(s);

    s = stack_init();
    for(int i=0; i<STACK_MAX; i++)
        STACK_UINT(s, i);
    
    bytes32_t b = BYTES32_UINT(STACK_MAX);
    assert(stack_push(&s, b) == false);
    assert(s.count == STACK_MAX);
    stack_free(s);

    assert(stack_memory());
}

void test_pop()
{
    printf("\n\ttest pop\t\t");
    
    stack_t s = stack_init();
    STACK_UINT(s, 1);
    STACK_UINT(s, 2);
    
    bytes32_t b;
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

void test_dup()
{
    printf("\n\ttest dup\t\t");
    
    stack_t s = stack_init();
    assert(stack_dup(&s, 1) == false);
    stack_free(s);
    
    s = stack_init();
    STACK_UINT(s, 1);
    assert(stack_dup(&s, 1));
    assert(s.count == 2);
    ASSERT_BYTES32_UINT(s.b[1], 1);
    stack_free(s);

    s = stack_init();
    STACK_UINT(s, 1);
    assert(stack_dup(&s, 2) == false);
    assert(s.count == 1);
    stack_free(s);

    s = stack_init();
    STACK_UINT(s, 1);
    STACK_UINT(s, 2);
    assert(stack_dup(&s, 2));
    assert(s.count == 3);
    ASSERT_BYTES32_UINT(s.b[1], 2);
    stack_free(s);
    
    assert(stack_memory());
}

void test_swap()
{
    printf("\n\ttest swap\t\t");
    
    stack_t s = stack_init();
    assert(stack_swap(&s, 1) == false);
    stack_free(s);
    
    s = stack_init();
    STACK_UINT(s, 1);
    assert(stack_swap(&s, 1) == false);
    stack_free(s);
    
    s = stack_init();
    STACK_UINT(s, 1);
    STACK_UINT(s, 2);
    assert(stack_swap(&s, 1));
    ASSERT_BYTES32_UINT(s.b[0], 2);
    ASSERT_BYTES32_UINT(s.b[1], 1);
    stack_free(s);
    
    
    s = stack_init();
    STACK_UINT(s, 1);
    STACK_UINT(s, 2);
    STACK_UINT(s, 3);
    assert(stack_swap(&s, 2));
    ASSERT_BYTES32_UINT(s.b[0], 3);
    ASSERT_BYTES32_UINT(s.b[2], 1);
    stack_free(s);
    
    assert(stack_memory());
}

void test_stack()
{
    printf("\ntest stack\t\t");

    test_push();
    test_pop();
    test_dup();
    test_swap();
}


int main() 
{
    setbuf(stdout, NULL);
    test_stack();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
