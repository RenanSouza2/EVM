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

#define BYTES32_UINT(UINT) BYTES32(0, 0, 0, 0, 0, 0, 0, UINT)

#define BYTES32_MAX() BYTES32(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX)

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

#define BYTES32_MINUS(UINT) \
    bytes32_sign((bytes32_sign_t){-1, BYTES32_UINT(UINT)})

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

#define ASSERT_BYTES32_MINUS(BYTES, UINT)   \
    {   \
        bytes32_t b_exp = BYTES32_MINUS(UINT);  \
        ASSERT_BYTES32_MUTUAL(BYTES, b_exp);    \
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

bytes32_sign_t bytes32_design(bytes32_t b);
bytes32_t bytes32_sign(bytes32_sign_t bs);

int bytes_n_cmp(int scalar, const uint b1[scalar], const uint b2[scalar]);
int bytes32_scmp(bytes32_t b1, bytes32_t b2);

void bytes_n_add_uint(int scalar, uint b[scalar], uint u, int i);
void bytes_n_shl_uint(int scalar, uint b[scalar], uint shift);
void bytes_n_shr_uint(int scalar, uint b[scalar], uint shift);

#endif
