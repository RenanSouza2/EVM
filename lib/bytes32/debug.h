#ifndef __BYTES32_DEBUG_H__
#define __BYTES32_DEBUG_H__

#include <stdbool.h>
#include <string.h>

#include "header.h"

STRUCT(bytes32_dual)
{
    bytes32_t b1, b2;
};

void bytes32_display(bytes32_t b);

#define BYTES32_DISPLAY(BYTES32) \
    printf("\n%s: ", #BYTES32);bytes32_display(BYTES32);
    
#define BYTES32(VALUE7, VALUE6, VALUE5, VALUE4, VALUE3, VALUE2, VALUE1, VALUE0) \
    (bytes32_t){{VALUE0, VALUE1, VALUE2, VALUE3, VALUE4, VALUE5, VALUE6, VALUE7}}

#define BYTES32_UINT(UINT) BYTES32(0, 0, 0, 0, 0, 0, DECH(UINT), DECL(UINT))



#define ASSERT_BYTES32_MUTUAL(BYTES1, BYTES2) \
    assert(memcmp(BYTES1.v, BYTES2.v, 32) == 0)

#define ASSERT_BYTES32(BYTES, \
    VALUE7, VALUE6, VALUE5, VALUE4, VALUE3, VALUE2, VALUE1, VALUE0 \
) \
{ \
    bytes32_t b_exp; \
    b_exp = BYTES32( \
        VALUE7, VALUE6, VALUE5, VALUE4, VALUE3, VALUE2, VALUE1, VALUE0 \
    ); \
    ASSERT_BYTES32_MUTUAL(BYTES, b_exp); \
}

#define ASSERT_BYTES32_UINT(BYTES, UINT) \
{ \
    bytes32_t b_exp; \
    b_exp = BYTES32_UINT(UINT); \
    ASSERT_BYTES32_MUTUAL(BYTES, b_exp); \
}

extern const bytes32_t b_zero;
extern const bytes32_t b_one;
extern const bytes32_t b_256;
extern const bytes32_t b_max;
extern const bytes32_t b_max_1;


bool bytes32_is_zero_bool(bytes32_t b);
int bytes32_cmp(bytes32_t b1, bytes32_t b2);

bytes32_t bytes32_add_uint(bytes32_t b, uint u, int i);
bytes32_t bytes32_shl_uint(bytes32_t b, uint shift);
bytes32_t bytes32_shr_uint(bytes32_t b, uint shift);
bytes32_dual_t bytes32_div_mod(bytes32_t b1, bytes32_t b2);

#endif
