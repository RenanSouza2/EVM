#ifndef __NUMBER_DEBUG_H__
#define __NUMBER_DEBUG_H__

#include "header.h"
#include "../uint/header.h"

STRUCT(number)
{
    bytes32_t b;
    number_p next;
};

#define NUMBER(VALUE7, VALUE6, VALUE5, VALUE4, VALUE3, VALUE2, VALUE1, VALUE0)  \
    number_create_bytes32(BYTES32(VALUE7, VALUE6, VALUE5, VALUE4, VALUE3, VALUE2, VALUE1, VALUE0))

#define NUMBER_UINT(UINT) \
    number_create_bytes32(BYTES32_UINT(UINT))

#define NUMBER_UINT_OFF(UINT, INT)    \
    number_create_bytes32_off(BYTES32_UINT(UINT), INT)

#define ASSERT_NUMBER_BYTES32(NUMBER, BYTES32)  \
    {   \
        assert(NUMBER != NULL); \
        ASSERT_BYTES32_MUTUAL(NUMBER->b, BYTES32);  \
    }

#define ASSERT_NUMBER_UINT(NUMBER, UINT)    \
    {   \
        assert(NUMBER != NULL); \
        ASSERT_BYTES32_UINT(NUMBER->b, UINT);   \
    }

void number_display(number_p n);

number_p number_copy(number_p n);
number_p number_create_bytes32_off(bytes32_t b, int i);
number_p number_create_uint_mult(int count, ...);
number_p number_create_bytes32_mult(int count, ...);

bool number_uint_mult(number_p n, int count, ...);
bool number_bytes32_mult(number_p n, int count, ...);

number_p number_add_bytes32(number_p n, bytes32_t b, int i);

#endif