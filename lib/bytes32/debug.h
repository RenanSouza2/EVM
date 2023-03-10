#ifndef __BYTES32_DEBUG_H__
#define __BYTES32_DEBUG_H__

#include <string.h>

#include "header.h"

#define SCALAR2 (2*SCALAR)
STRUCT(bytes64)
{
    uint v[SCALAR2];
};

void bytes32_display(bytes32_t b);

#define BYTES32_DISPLAY(BYTES32) \
    printf("\n%s: ", #BYTES32);bytes32_display(BYTES32);printf("\t\t");
    
#define BYTES32(VALUE7, VALUE6, VALUE5, VALUE4, VALUE3, VALUE2, VALUE1, VALUE0) \
    (bytes32_t){{VALUE0, VALUE1, VALUE2, VALUE3, VALUE4, VALUE5, VALUE6, VALUE7}}

#define BYTES32_UINT(UINT) BYTES32(0, 0, 0, 0, 0, 0, DECH(UINT), DECL(UINT))

#define LUINT(UINT0)    \
    (*((luint*)(&(UINT0))))

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

#define BYTES64(    \
    VALUE15, VALUE14, VALUE13, VALUE12, VALUE11, VALUE10, VALUE9, VALUE8,  \
    VALUE7, VALUE6, VALUE5, VALUE4, VALUE3, VALUE2, VALUE1, VALUE0  \
)   \
    (bytes64_t){{   \
        VALUE0, VALUE1, VALUE2, VALUE3, VALUE4, VALUE5, VALUE6, VALUE7, \
        VALUE8, VALUE9, VALUE10, VALUE11, VALUE12, VALUE13, VALUE14, VALUE15    \
    }}

#define BYTES64_UINT(UINT) BYTES64(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DECH(UINT), DECL(UINT))

#define ASSERT_BYTES64_MUTUAL(BYTES1, BYTES2) \
    assert(memcmp(BYTES1.v, BYTES2.v, 64) == 0)

#define ASSERT_BYTES64(BYTES, \
    VALUE15, VALUE14, VALUE13, VALUE12, VALUE11, VALUE10, VALUE9, VALUE8,  \
    VALUE7, VALUE6, VALUE5, VALUE4, VALUE3, VALUE2, VALUE1, VALUE0 \
) \
{ \
    bytes64_t bd_exp; \
    bd_exp = BYTES64( \
        VALUE15, VALUE14, VALUE13, VALUE12, VALUE11, VALUE10, VALUE9, VALUE8,  \
        VALUE7, VALUE6, VALUE5, VALUE4, VALUE3, VALUE2, VALUE1, VALUE0 \
    ); \
    ASSERT_BYTES64_MUTUAL(BYTES, bd_exp); \
}

#define ASSERT_BYTES64_UINT(BYTES, UINT) \
{ \
    bytes64_t bd_exp; \
    bd_exp = BYTES64_UINT(UINT); \
    ASSERT_BYTES64_MUTUAL(BYTES, bd_exp); \
}


extern const bytes32_t b_zero;
extern const bytes32_t b_one;
extern const bytes32_t b_256;
extern const bytes32_t b_max;
extern const bytes32_t b_max_1;
extern const bytes32_t b_Q255;

bytes32_t bytes32_add_uint(bytes32_t b, uint u, int i);
bytes32_dual_t bytes32_div_mod(bytes32_t b1, bytes32_t b2);

bytes64_t bytes64_add_uint(bytes64_t bd, uint u, int i);

#endif
