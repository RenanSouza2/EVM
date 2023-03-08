#ifndef __NUMBER_DEBUG_H__
#define __NUMBER_DEBUG_H__

#include "header.h"
#include "../uint/header.h"

STRUCT(number)
{
    bytes32_t b;
    number_p next;
};

void number_display(number_p n);

#define NUMBER_DISPLAY(NUMBER)  \
    {   \
        printf("\n%s: ", #NUMBER);  \
        number_display(NUMBER); \
        printf("\n");   \
    }

#define NUMBER( \
        VALUE7, VALUE6, VALUE5, VALUE4, \
        VALUE3, VALUE2, VALUE1, VALUE0  \
    )  \
    number_create_bytes32(  \
        BYTES32(    \
            VALUE7, VALUE6, VALUE5, VALUE4, \
            VALUE3, VALUE2, VALUE1, VALUE0  \
        )   \
    )

#define NUMBER_UINT(UINT) \
    number_create_bytes32_force(BYTES32_UINT(UINT))

#define ASSERT_NUMBER_BYTES32(NUMBER, BYTES32)  \
    {   \
        assert(NUMBER != NULL); \
        ASSERT_BYTES32_MUTUAL(NUMBER->b, BYTES32);  \
    }
    
#define ASSERT_NUMBER(  \
        NUMBER,     \
        VALUE7, VALUE6, VALUE5, VALUE4, \
        VALUE3, VALUE2, VALUE1, VALUE0  \
    )\
    {   \
        bytes32_t b_exp = BYTES32(  \
            VALUE7, VALUE6, VALUE5, VALUE4, \
            VALUE3, VALUE2, VALUE1, VALUE0  \
        );  \
        ASSERT_NUMBER_BYTES32(NUMBER, b_exp);  \
    }

#define ASSERT_NUMBER_UINT(NUMBER, UINT)    \
    {   \
        assert(NUMBER != NULL); \
        ASSERT_BYTES32_UINT(NUMBER->b, UINT);   \
    }

extern int number_created;
extern int number_freed;
bool number_memory();

number_p number_create_null();
number_p number_create_uint_mult(int count, ...);
number_p number_create_bytes32_mult(int count, ...);
number_p number_copy(number_p n);
void number_free();

bool number_uint_mult(number_p n, int count, ...);
bool number_bytes32_mult(number_p n, int count, ...);

number_p number_add_bytes32(number_p n, bytes32_t b);
number_p number_sub_bytes32(number_p n, bytes32_t b);
number_p number_mul_bytes32(number_p n, number_p n1, bytes32_t b);

int number_cmp(number_p n1, number_p n2);

number_p number_create_bytes32(bytes32_t b);
number_p number_create_bytes32_force(bytes32_t b);


number_p number_add(number_p n1, number_p n2);
number_p number_sub(number_p n1, number_p n2);
number_p number_mul(number_p n1, number_p n2);

number_p number_shl(number_p n, uint u);
number_p number_shr(number_p n, uint u);

#endif