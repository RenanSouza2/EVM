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

#define NUMBER_UINT_DBG(UINT, INT)    \
    number_create_bytes32_dbg(BYTES32_UINT(UINT), INT)

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
number_p number_create_bytes32_dbg(bytes32_t b, int i);

number_p number_add_uint(number_p n, uint u, int i);
number_p number_add_bytes32(number_p n, bytes32_t b, int i);
number_p number_bytes32_mul(bytes32_t b1, bytes32_t b2);

#endif