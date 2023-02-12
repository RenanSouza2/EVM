#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

#include "../submodule/c-library/struct.h"

#include "../lib/uint/header.h"

#define SCALAR 8
typedef uint bytes32[SCALAR];

#define BYTES32_RESET(PTR) memset(PTR, 0, 32)
#define BYTES32_ASSIGN(PTR1, PTR2) memcpy(PTR1, PTR2, 32);

void bytes32_display(bytes32 value)
{
    printf("0x");
    for(int i=SCALAR-1; i>=0; i--) {
        printf("%08X", value[i]);
    }
}

void bytes32_convert(bytes32 res, luint value_int)
{
    BYTES32_RESET(res);
    res[1] = DECH(value_int);
    res[0] = DECL(value_int);
}

void bytes32_add_uint(bytes32 res, uint a, int i)
{
    if(i >= SCALAR) return;
    if(a == 0) return;

    luint aux = uint_add(res[i], a);
    res[i] = DECL(a);

    bytes32_add_uint(res, DECH(aux), i+1);
}

void bytes32_add(bytes32 res, bytes32 a, bytes32 b)
{
    BYTES32_ASSIGN(res, a);

    for(int i=0; i<SCALAR; i++)
        bytes32_add_uint(res, b[i], i);
}

void bytes32_mul(bytes32 res, bytes32 a, bytes32 b)
{
    BYTES32_RESET(res);
    for(int i=0; i<SCALAR; i++)
    for(int j=0; j+i<SCALAR; j++)
    {
        luint aux = uint_mul(a[i], b[j]);
        bytes32_add_uint(res, DECL(aux), i+j);
        bytes32_add_uint(res, DECH(aux), i+j+1);
    }
}

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

void test1()
{
    bytes32 value;
    bytes32_convert(value, 0);
    printf("\nres: ");bytes32_display(value);
    
    bytes32_convert(value, 1);
    printf("\nres: ");bytes32_display(value);

    bytes32_convert(value, ((luint)1) << 32);
    printf("\nres: ");bytes32_display(value);
}

void test2()
{
    printf("\n");

    bytes32 a, b;
    bytes32_convert(a, 1);
    bytes32_convert(b, 2);

    bytes32 c;
    bytes32_add(c, a, b);
    bytes32_display(c);
    
    printf("\n");
    bytes32_convert(a, UINT_MAX);
    bytes32_convert(b, 3);
    bytes32_add(c, a, b);
    bytes32_display(c);
}

int main() 
{
    test2();

    printf("\n");
    return 0;
}