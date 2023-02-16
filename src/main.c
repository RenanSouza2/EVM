#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#include "../submodule/c-library/struct.h"

#include "../lib/uint/header.h"
#include "../lib/bytes32/header.h"

// STRUCT(stack)
// {
//     stack_p next;
//     bytes32 value;
// };
//
// void pointer_display(handler_p p) {
//     if(p == NULL) printf("NULL");
//     else          printf("%p", p);
// }
//
// void stack_display_item(stack_p stack) {
//     printf("Stack: ");pointer_display(stack);
//     printf("\n");
// }
//
// stack_p stack_create(bytes32 value, stack_p stack_next) {
//     stack_p stack = malloc(sizeof(stack_t));
//     assert(stack);
//
//     stack->next = stack_next;
//     memcpy(stack->value, value, 32);
//     return stack;
// } 

// void test2()
// {
//     printf("\n");

//     bytes32 a, b;
//     bytes32_convert(a, 1);
//     bytes32_convert(b, 2);

//     bytes32 c;
//     bytes32_add(c, a, b);
//     bytes32_display(c);
    
//     printf("\n");
//     bytes32_convert(a, UINT_MAX);
//     bytes32_convert(b, 3);
//     bytes32_add(c, a, b);
//     bytes32_display(c);
// }

int main() 
{
    printf("\nNo implementation\n\n");

    printf("\n");
    return 0;
}