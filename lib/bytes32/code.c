#include "debug.h"
#include <stdbool.h>
#include <limits.h>
#include <assert.h>


#define BYTES64_BYTES32(B) (*((bytes64_p)&((bytes32_dual_t){{(B), b32_zero}})))
#define BYTES32_BYTES64(B) (*((bytes32_p)(&(B))))   

#define BYTES_N_RESET(SCALAR, B) memset(B, 0, SCALAR << 2)
#define BYTES_N_SET(SCALAR, B1, B2) memcpy(B1, B2, SCALAR << 2)

#ifdef DEBUG

#include <stdio.h>

const bytes32_t b_max = BYTES32(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
const bytes32_t b_max_1 = BYTES32(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX - 1);
const bytes32_t b_Q255 = BYTES32(0x80000000, 0, 0, 0, 0, 0, 0, 0);

void bytes_n_display(int scalar, const uint b[scalar])
{
    printf("0x");
    for(int i=scalar-1; i>=0; i--) 
        printf("%08x", b[i]);
}

void bytes32_display(bytes32_t b)
{
    bytes_n_display(SCALAR32, b.v);
}

#endif

#define b32_zero b_zero
const bytes32_t b_zero = BYTES32_UINT(0);
const bytes32_t b_one = BYTES32_UINT(1);
const bytes32_t b_32 = BYTES32_UINT(32);
const bytes32_t b_256 = BYTES32_UINT(256);

const bytes64_t b64_zero = BYTES64_UINT(0);


bool bytes_n_is_zero_bool(int scalar, const uint b[scalar])
{
    return memcmp(b, b32_zero.v, scalar << 2) == 0;
}

int bytes_n_cmp(int scalar, const uint b1[scalar], const uint b2[scalar])
{
    for(int i=scalar-1; i>=0; i--) 
    {
        if(b1[i] > b2[i]) return  1; 
        if(b1[i] < b2[i]) return -1; 
    }
    return 0;
}

int bytes32_sign_cmp(bytes32_t b1, bytes32_t b2)
{
    bytes32_sign_t bs1, bs2;
    bs1 = bytes32_design(b1);
    bs2 = bytes32_design(b2);

    switch (bs1.sign)
    {
        case  1:
        switch (bs2.sign)
        {
            case  1: return BYTES32_OP(cmp, bs1.b, bs2.b);
            case -1: return 1;
        }
        break;
        
        case -1:
        switch (bs2.sign)
        {
            case  1: return -1;
            case -1: return BYTES32_OP(cmp, bs2.b, bs1.b);
        }
        break;
    }
    assert(false);
}

void bytes_n_add_uint(int scalar, uint b[scalar], uint u, int i)
{
    if(u == 0) return;
    if(i >= scalar) return;

    luint lu = uint_add(b[i], u);
    b[i] = DECL(lu);

    return bytes_n_add_uint(scalar, b, DECH(lu), i+1);
}

void bytes_n_shl_uint(int scalar, uint b[scalar], uint shift)
{
    if(shift >= scalar << 5)
    {
        BYTES_N_RESET(scalar, b);
        return;
    }

    if(shift == 0) return;
    
    int jmp, off;
    jmp = shift >> 5;
    off = shift & 31;
    
    uint b_out[scalar];
    BYTES_N_RESET(scalar, b_out);
    if(off == 0)
    {
        memcpy(&b_out[jmp], b, (scalar-jmp) << 2);
        memcpy(b, b_out, scalar << 2);
        return;
    }
    
    b_out[jmp] = b[0] << off;
    for(int i=1; i+jmp<scalar; i++)
        b_out[i+jmp] = (b[i-1] >> (32 - off)) | (b[i] << off);

    BYTES_N_SET(scalar, b, b_out);
    return;
}

void bytes_n_shr_uint(int scalar, uint b[scalar], uint shift)
{
    if(shift >= scalar << 5)
    {
        BYTES_N_RESET(scalar, b);
        return;
    }

    int jmp = shift >> 5;
    int off = shift & 31;

    uint b_out[scalar];
    BYTES_N_RESET(scalar, b_out);
    if(off == 0)
    {
        memcpy(b_out, &b[jmp], (scalar-jmp) << 2);
        memcpy(b, b_out, scalar << 2);
        return;
    }

    b_out[scalar-1-jmp] = b[scalar-1] >> off;
    for(int i=0; i+jmp+1<scalar; i++)
    {
        b_out[i] =  (b[jmp+i+1] << (32 - off)) | (b[jmp+i] >> off);
    }

    BYTES_N_SET(scalar, b, b_out);
    return;
}

#define BYTES_N_DIV_MOD(SIZE)   \
bytes##SIZE##_dual_t bytes##SIZE##_div_mod(bytes##SIZE##_t b1, bytes##SIZE##_t b2)  \
{   \
    if(bytes_n_is_zero_bool(SCALAR##SIZE, b2.v))    \
        return (bytes##SIZE##_dual_t){{b##SIZE##_zero, b##SIZE##_zero}}; \
    \
    bytes##SIZE##_t b1_aux, b_base; \
    BYTES_N_SET(SIZE, &b1_aux, &b1);    \
    BYTES_N_OP_UINT(shr, SIZE, b1_aux, 1); \
    b_base = BYTES##SIZE##_UINT(1); \
    while(BYTES_N_OP(cmp, SIZE, b1_aux, b2) >= 0)   \
    {   \
        BYTES_N_OP_UINT(shl, SIZE, b2, 1); \
        BYTES_N_OP_UINT(shl, SIZE, b_base, 1); \
    }   \
    \
    bytes##SIZE##_t b_out = b##SIZE##_zero; \
    while(!bytes_n_is_zero_bool(SCALAR##SIZE, b_base.v))  \
    {   \
        if(BYTES_N_OP(cmp, SIZE, b1, b2) >= 0)  \
        {   \
            b1 = bytes##SIZE##_sub(b1, b2); \
            b_out = bytes##SIZE##_add(b_out, b_base);   \
        }   \
        \
        BYTES_N_OP_UINT(shr, SIZE, b2, 1);  \
        BYTES_N_OP_UINT(shr, SIZE, b_base, 1);  \
    }   \
    \
    return (bytes##SIZE##_dual_t){{b1, b_out}}; \
}

BYTES_N_DIV_MOD(32)
BYTES_N_DIV_MOD(64)

bytes32_sign_t bytes32_design(bytes32_t b)
{
    if((b.v[SCALAR32-1] & 0x80000000) == 0) 
        return (bytes32_sign_t){1, b};
    
    b = bytes32_sub(b32_zero, b);
    return (bytes32_sign_t){-1, b};
}

bytes32_t bytes32_sign(bytes32_sign_t bs)
{
    if(bs.sign == 1) return bs.b;
    return bytes32_sub(b32_zero, bs.b);
}



bytes32_t bytes32_is_zero(bytes32_t b)
{
    if(bytes_n_is_zero_bool(SCALAR32, b.v)) return b_one;
    return b32_zero;
}

bytes32_t bytes32_eq(bytes32_t b1, bytes32_t b2)
{
    if(BYTES32_OP(cmp, b1, b2) == 0) return b_one;
    return b32_zero;
}

bytes32_t bytes32_lt(bytes32_t b1, bytes32_t b2)
{
    if(BYTES32_OP(cmp, b1, b2) < 0) return b_one;
    return b32_zero;
}

bytes32_t bytes32_gt(bytes32_t b1, bytes32_t b2)
{
    if(BYTES32_OP(cmp, b1, b2) > 0) return b_one;
    return b32_zero;
}

bytes32_t bytes32_sign_lt(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_sign_cmp(b1, b2) < 0) return b_one;
    return b32_zero;
}

bytes32_t bytes32_sign_gt(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_sign_cmp(b1, b2) > 0) return b_one;
    return b32_zero;
}



bytes32_t bytes32_shl(bytes32_t b1, bytes32_t b2)
{
    if(BYTES32_OP(cmp, b2, b_256) >= 0) return b32_zero;
    BYTES32_OP_UINT(shl, b1, b2.v[0]);
    return b1;
}

bytes32_t bytes32_shr(bytes32_t b1, bytes32_t b2)
{
    BYTES32_OP_UINT(shr, b1, b2.v[0]);
    return b1;
}

#define BYTES_N_NOT(SIZE)   \
bytes##SIZE##_t bytes##SIZE##_not(bytes##SIZE##_t b)    \
{   \
    for(int i=0; i<SCALAR##SIZE; i++)   \
        b.v[i] = ~b.v[i];   \
    return b;   \
}

BYTES_N_NOT(32)
BYTES_N_NOT(64)

#define BYTES_N_ADD(SIZE)   \
bytes##SIZE##_t bytes##SIZE##_add(bytes##SIZE##_t b1, bytes##SIZE##_t b2)   \
{   \
    for(int i=0; i<SCALAR##SIZE; i++)   \
        BYTES_N_OP_UINT(add, SIZE, b1, b2.v[i], i);    \
    return b1;  \
}

BYTES_N_ADD(32) 
BYTES_N_ADD(64) 

#define BYTES_N_MUL(SIZE)   \
bytes##SIZE##_t bytes##SIZE##_mul(bytes##SIZE##_t b1, bytes##SIZE##_t b2)   \
{   \
    bytes##SIZE##_t b_res = b##SIZE##_zero; \
    for(int i=0; i<SCALAR##SIZE; i++)   \
    for(int j=0; j+i<SCALAR##SIZE; j++) \
    {   \
        luint aux = uint_mul(b1.v[i], b2.v[j]); \
        BYTES_N_OP_UINT(add, SIZE, b_res, DECL(aux), i+j);  \
        BYTES_N_OP_UINT(add, SIZE, b_res, DECH(aux), i+j+1);    \
    }   \
    return b_res;   \
}

BYTES_N_MUL(32)
BYTES_N_MUL(64)

#define BYTES_N_SUB(SIZE)   \
bytes##SIZE##_t bytes##SIZE##_sub(bytes##SIZE##_t b1, bytes##SIZE##_t b2)   \
{   \
    b2 = bytes##SIZE##_not(b2); \
    b1 = bytes##SIZE##_add(b1, b2); \
    BYTES_N_OP_UINT(add, SIZE, b1, 1, 0);   \
    return b1;    \
}

BYTES_N_SUB(32)
BYTES_N_SUB(64)

bytes32_t bytes32_div(bytes32_t b1, bytes32_t b2)
{
    bytes32_dual_t bd = bytes32_div_mod(b1, b2);
    return bd.b[1];
}

#define BYTES_N_MOD(SIZE)   \
bytes##SIZE##_t bytes##SIZE##_mod(bytes##SIZE##_t b1, bytes##SIZE##_t b2)   \
{   \
    bytes##SIZE##_dual_t bd = bytes##SIZE##_div_mod(b1, b2);    \
    return bd.b[0]; \
}

BYTES_N_MOD(32)
BYTES_N_MOD(64)

bytes32_t bytes32_sdiv(bytes32_t b1, bytes32_t b2)
{
    bytes32_sign_t bs1, bs2;
    bs1 = bytes32_design(b1);
    bs2 = bytes32_design(b2);

    bytes32_sign_t bs;
    bs.sign = bs1.sign * bs2.sign;
    bs.b = bytes32_div(bs1.b, bs2.b);
    return bytes32_sign(bs);
}

bytes32_t bytes32_smod(bytes32_t b1, bytes32_t b2)
{
    bytes32_sign_t bs1, bs2;
    bs1 = bytes32_design(b1);
    bs2 = bytes32_design(b2);

    bytes32_sign_t bs;
    bs.sign = bs1.sign * bs2.sign;
    bs.b = bytes32_mod(bs1.b, bs2.b);
    return bytes32_sign(bs);
}

bytes32_t bytes32_exp(bytes32_t b1, bytes32_t b2)
{
    if(BYTES32_OP(cmp, b2, b32_zero) == 0) return b_one;
    if(BYTES32_OP(cmp, b2, b_one) == 0) return b1;

    bytes32_t b_res;
    bool is_odd = b2.v[0] & 1;
    BYTES32_OP_UINT(shr, b2, 1);
    b_res = bytes32_exp(b1, b2);
    b_res = bytes32_mul(b_res, b_res);

    if(is_odd) return bytes32_mul(b1, b_res);
    return b_res;
}

bytes32_t bytes32_sign_extend(bytes32_t b1, bytes32_t b2)
{
    if(BYTES32_OP(cmp, b1, b_32) >= 0) return b2;

    int off = b1.v[0];
    uint value = (b2.v[off] & 0x80000000) ? UINT_MAX : 0;
    for(int i=off+1; i<SCALAR32; i++)
        b2.v[i] = value;
    return b2;
}



bytes32_t bytes32_addmod(bytes32_t b1, bytes32_t b2, bytes32_t b3)
{
    bytes64_t b64_1, b64_2;
    b64_1 = BYTES64_BYTES32(b1);
    b64_2 = BYTES64_BYTES32(b2);
    b64_1 = bytes64_add(b64_1, b64_2);

    b64_2 = BYTES64_BYTES32(b3);
    b64_1 = bytes64_mod(b64_1, b64_2);

    return BYTES32_BYTES64(b64_1);
}

bytes32_t bytes32_mulmod(bytes32_t b1, bytes32_t b2, bytes32_t b3)
{
    bytes64_t b64_1, b64_2;
    b64_1 = BYTES64_BYTES32(b1);
    b64_2 = BYTES64_BYTES32(b2);
    b64_1 = bytes64_mul(b64_1, b64_2);

    b64_2 = BYTES64_BYTES32(b3);
    b64_1 = bytes64_mod(b64_1, b64_2);

    return BYTES32_BYTES64(b64_1);
}
