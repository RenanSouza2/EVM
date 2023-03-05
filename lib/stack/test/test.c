#include <stdio.h>
#include <assert.h>

#include "../debug.h"
#include "../../bytes32/debug.h"

void test_push()
{
    printf("\n\ttest push\t\t");

    stack_t s = stack_init();
    assert(stack_push(&s, b_one));

    bytes32_list_p bl = s.bl;
    assert(bl != NULL);
    ASSERT_BYTES32_UINT(bl->b, 1);
    assert(bl->next == NULL);
    assert(s.count == 1);

    s = stack_init();
    bytes32_t b = BYTES32_UINT(2);
    assert(stack_push(&s, b_one));
    assert(stack_push(&s, b));
    bl = s.bl;
    assert(bl != NULL);
    ASSERT_BYTES32_UINT(bl->b, 2);
    assert(bl->next != NULL);
    assert(s.count == 2);

    s = stack_init();
    for(int i=0; i<STACK_MAX; i++)
    {
        bytes32_t b32 = BYTES32_UINT(i);
        assert(stack_push(&s, b32));
    }
    bytes32_t b32 = BYTES32_UINT(STACK_MAX);
    assert(stack_push(&s, b32) == false);
    assert(s.count == STACK_MAX);
}

void test_pop()
{
    printf("\n\ttest pop\t\t");
    
    stack_t s = stack_init();
    bytes32_t b = BYTES32_UINT(2);
    stack_push(&s, b_one);
    stack_push(&s, b);
    
    b = stack_pop(&s);
    ASSERT_BYTES32_UINT(b, 2);
    assert(s.count == 1);
    assert(s.bl != NULL);
    
    b = stack_pop(&s);
    ASSERT_BYTES32_UINT(b, 1);
    assert(s.count == 0);
    assert(s.bl == NULL);
}

void test_stack()
{
    printf("\ntest example library\t\t");

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
