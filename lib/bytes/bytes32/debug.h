#ifndef __BYTES32_DEBUG_H__
#define __BYTES32_DEBUG_H__

#include <string.h>

#include "header.h"

#define SCALAR64 (2*SCALAR32)
STRUCT(bytes64)
{
    uint v[SCALAR64];
};

STRUCT(bytes32_sign)
{
    int sign;
    bytes32_t b;
};
 
#define BYTES32(    \
        VALUE7, VALUE6, VALUE5, VALUE4, \
        VALUE3, VALUE2, VALUE1, VALUE0  \
    ) \
    (bytes32_t){{   \
        VALUE0, VALUE1, VALUE2, VALUE3, \
        VALUE4, VALUE5, VALUE6, VALUE7  \
    }}

#define BYTES32_UINT(UINT) BYTES32(0, 0, 0, 0, 0, 0, DECH(UINT), DECL(UINT))

#define LUINT(UINT0)    \
    (*((luint*)(&(UINT0))))

#define BYTES64(    \
        VALUE15, VALUE14, VALUE13, VALUE12, \
        VALUE11, VALUE10, VALUE9, VALUE8,   \
        VALUE7, VALUE6, VALUE5, VALUE4, \
        VALUE3, VALUE2, VALUE1, VALUE0  \
    )   \
    (bytes64_t){{   \
        VALUE0, VALUE1, VALUE2, VALUE3, \
        VALUE4, VALUE5, VALUE6, VALUE7, \
        VALUE8, VALUE9, VALUE10, VALUE11,   \
        VALUE12, VALUE13, VALUE14, VALUE15  \
    }}

#define BYTES64_UINT(UINT)  \
    BYTES64( \
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, DECH(UINT), DECL(UINT)    \
    )

#ifdef DEBUG

extern const bytes32_t b_zero;
extern const bytes32_t b_one;
extern const bytes32_t b_256;
extern const bytes32_t b_max;
extern const bytes32_t b_max_1;
extern const bytes32_t b_Q255;

void bytes32_display(bytes32_t b);

#define BYTES32_DISPLAY(B32) \
    printf("\n%s: ", #B32);bytes32_display(B32);printf("\t\t");

#define ASSERT_BYTES32_MUTUAL(BYTES1, BYTES2) \
    {   \
        if(memcmp(BYTES1.v, BYTES2.v, 32))  \
        {   \
            printf("\n\nassertion failled");    \
            printf("\nreturned: "); \
            bytes32_display(BYTES1);    \
            printf("\nexpected: "); \
            bytes32_display(BYTES2);    \
            printf("\n\n\t\t"); \
            assert(false);  \
        }   \
    }

#define ASSERT_BYTES32(BYTES, \
        VALUE7, VALUE6, VALUE5, VALUE4, \
        VALUE3, VALUE2, VALUE1, VALUE0  \
    ) \
    { \
        bytes32_t b_exp; \
        b_exp = BYTES32( \
            VALUE7, VALUE6, VALUE5, VALUE4, \
            VALUE3, VALUE2, VALUE1, VALUE0  \
        ); \
        ASSERT_BYTES32_MUTUAL(BYTES, b_exp); \
    }

#define ASSERT_BYTES32_UINT(BYTES, UINT) \
    { \
        bytes32_t b_exp; \
        b_exp = BYTES32_UINT(UINT); \
        ASSERT_BYTES32_MUTUAL(BYTES, b_exp); \
    }


#define ASSERT_BYTES64_MUTUAL(BYTES1, BYTES2) \
    assert(memcmp(BYTES1.v, BYTES2.v, 64) == 0)

#define ASSERT_BYTES64(BYTES, \
        VALUE15, VALUE14, VALUE13, VALUE12, \
        VALUE11, VALUE10, VALUE9, VALUE8,   \
        VALUE7, VALUE6, VALUE5, VALUE4, \
        VALUE3, VALUE2, VALUE1, VALUE0  \
    ) \
    { \
        bytes64_t bd_exp; \
        bd_exp = BYTES64( \
            VALUE15, VALUE14, VALUE13, VALUE12, \
            VALUE11, VALUE10, VALUE9, VALUE8,   \
            VALUE7, VALUE6, VALUE5, VALUE4, \
            VALUE3, VALUE2, VALUE1, VALUE0  \
        ); \
        ASSERT_BYTES64_MUTUAL(BYTES, bd_exp); \
    }

#define ASSERT_BYTES64_UINT(BYTES, UINT) \
    { \
        bytes64_t bd_exp; \
        bd_exp = BYTES64_UINT(UINT); \
        ASSERT_BYTES64_MUTUAL(BYTES, bd_exp); \
    }

#endif

#define BYTES64_BYTES32(B64, B32)   \
    {   \
        BYTES_N_RESET(SCALAR64, &B64);  \
        BYTES_N_SET(SCALAR32, &B64, &B32);  \
    }
    
#define BYTES32_BYTES64(B) (*((bytes32_p)(&(B))))   

#define BYTES_N_OP_1(OP, SCALAR, B) bytes_n_##OP(SCALAR, B.v)
#define BYTES32_OP_1(OP, B) BYTES_N_OP_1(OP, SCALAR32, B)

#define BYTES_N_OP_2(OP, SCALAR, B1, B2) bytes_n_##OP(SCALAR, B1.v, B2.v)
#define BYTES32_OP_2(OP, B1, B2) BYTES_N_OP_2(OP, SCALAR32, B1, B2)
#define BYTES64_OP_2(OP, B1, B2) BYTES_N_OP_2(OP, SCALAR64, B1, B2)

#define BYTES_N_OP_UINT(OP, SCALAR, B, ARGS...) bytes_n_##OP##_uint(SCALAR, B.v, ARGS)
#define BYTES32_OP_UINT(OP, B, ARGS...) BYTES_N_OP_UINT(OP, SCALAR32, B, ARGS)

int bytes_n_cmp(int scalar, const uint b1[scalar], const uint b2[scalar]);
int bytes32_sign_cmp(bytes32_t b1, bytes32_t b2);

void bytes_n_add_uint(int scalar, uint b[scalar], uint u, int i);
void bytes_n_shl_uint(int scalar, uint b[scalar], uint shift);
void bytes_n_shr_uint(int scalar, uint b[scalar], uint shift);

bytes32_sign_t bytes32_design(bytes32_t b);
bytes32_t bytes32_sign(bytes32_sign_t bs);

bytes64_t bytes64_add_uint(bytes64_t bd, uint u, int i);

bytes64_t bytes64_add(bytes64_t b1, bytes64_t b2);
bytes64_t bytes64_sub(bytes64_t b1, bytes64_t b2);

#endif
