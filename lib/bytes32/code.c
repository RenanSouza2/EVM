#include "debug.h"

#define BYTES64_BYTES32(B) (*((bytes64_p)&((bytes32_dual_t){{(B), b32_zero}})))
#define DUAL(BD) (*((bytes32_dual_p)&(BD)))               

#ifdef DEBUG

#include <stdio.h>
#include <limits.h>

const bytes32_t b_max = BYTES32(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
const bytes32_t b_max_1 = BYTES32(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX - 1);
const bytes32_t b_Q255 = BYTES32(0x80000000, 0, 0, 0, 0, 0, 0, 0);

#define BYTES_N_DISPLAY(SIZE)   \
void bytes##SIZE##_display(bytes##SIZE##_t b)   \
{   \
    printf("0x");   \
    for(int i=SCALAR##SIZE-1; i>=0; i--)    \
        printf("%08x", b.v[i]); \
}   \

BYTES_N_DISPLAY(32)
BYTES_N_DISPLAY(64)

#endif

const bytes32_t b32_zero = BYTES32_UINT(0);
const bytes32_t b_one = BYTES32_UINT(1);
const bytes32_t b_256 = BYTES32_UINT(256);

const bytes64_t b64_zero = BYTES64_UINT(0);


#define BYTES_IS_ZERO_BOOL(SIZE)    \
bool bytes##SIZE##_is_zero_bool(bytes##SIZE##_t b)  \
{   \
    return memcmp(b.v, b32_zero.v, SIZE) == 0;    \
}

BYTES_IS_ZERO_BOOL(32)
BYTES_IS_ZERO_BOOL(64)

#define BYTES_N_CMP(SIZE)   \
int bytes##SIZE##_cmp(bytes##SIZE##_t b1, bytes##SIZE##_t b2)   \
{   \
    for(int i=SCALAR##SIZE-1; i>=0; i--)    \
    {   \
        if(b1.v[i] > b2.v[i]) return  1;    \
        if(b1.v[i] < b2.v[i]) return -1;    \
    }   \
    return 0;   \
}

BYTES_N_CMP(32)
BYTES_N_CMP(64)


bytes32_t bytes32_add_uint(bytes32_t b, uint u, int i)
{
    if(u == 0) return b;
    if(i >= SCALAR32) return b;

    luint lu = uint_add(b.v[i], u);
    b.v[i] = DECL(lu);

    return bytes32_add_uint(b, DECH(lu), i+1);
}

#define BYTES_N_SHL_UINT(SIZE)  \
bytes##SIZE##_t bytes##SIZE##_shl_uint(bytes##SIZE##_t b, uint shift)   \
{   \
    if(shift > 255) return b##SIZE##_zero;  \
    \
    int jmp = shift >> 5;   \
    int off = shift & 31;   \
    \
    bytes##SIZE##_t b_out = b##SIZE##_zero; \
    b_out.v[jmp] = b.v[0] << off;   \
    for(int i=1; i+jmp<SCALAR##SIZE; i++) \
    {   \
        luint lu = LUINT(b.v[i-1]) << off;  \
        b_out.v[i+jmp] = DECH(lu);  \
    }   \
    \
    return b_out;   \
}

BYTES_N_SHL_UINT(32)
BYTES_N_SHL_UINT(64)

#define BYTES_N_SHR_UINT(SIZE)  \
bytes##SIZE##_t bytes##SIZE##_shr_uint(bytes##SIZE##_t b, uint shift)   \
{   \
    if(shift > 255) return b##SIZE##_zero;  \
    \
    int jmp = shift >> 5;   \
    int off = shift & 31;   \
    \
    bytes##SIZE##_t b_out = b##SIZE##_zero; \
    b_out.v[SCALAR##SIZE-1 - jmp] = b.v[SCALAR##SIZE-1] >> off; \
    for(int i=jmp; i<SCALAR##SIZE-1; i++)   \
    {   \
        luint lu = LUINT(b.v[i]) >> off;    \
        b_out.v[i-jmp] = DECL(lu);  \
    }   \
        \
    return b_out;   \
}

BYTES_N_SHR_UINT(32)
BYTES_N_SHR_UINT(64)

#define BYTES_N_DIV_MOD(SIZE)   \
bytes##SIZE##_dual_t bytes##SIZE##_div_mod(bytes##SIZE##_t b1, bytes##SIZE##_t b2)  \
{   \
    if(bytes##SIZE##_is_zero_bool(b2)) return (bytes##SIZE##_dual_t){{b##SIZE##_zero, b##SIZE##_zero}}; \
    \
    bytes##SIZE##_t b1_aux, b_base; \
    b1_aux = bytes##SIZE##_shr_uint(b1, 1); \
    b_base = BYTES##SIZE##_UINT(1); \
    while(bytes##SIZE##_cmp(b1_aux, b2) >= 0)   \
    {   \
        b2 = bytes##SIZE##_shl_uint(b2, 1); \
        b_base = bytes##SIZE##_shl_uint(b_base, 1); \
    }   \
    \
    bytes##SIZE##_t b_out = b##SIZE##_zero; \
    while(!bytes##SIZE##_is_zero_bool(b_base))  \
    {   \
        if(bytes##SIZE##_cmp(b1, b2) >= 0)  \
        {   \
            b1 = bytes##SIZE##_sub(b1, b2); \
            b_out = bytes##SIZE##_add(b_out, b_base);   \
        }   \
        \
        b2 = bytes##SIZE##_shr_uint(b2, 1); \
        b_base = bytes##SIZE##_shr_uint(b_base, 1); \
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
    if(bytes32_is_zero_bool(b)) return b_one;
    return b32_zero;
}

bytes32_t bytes32_lt(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_cmp(b1, b2) < 0) return b_one;
    return b32_zero;
}

bytes32_t bytes32_gt(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_cmp(b1, b2) > 0) return b_one;
    return b32_zero;
}

bytes32_t bytes32_eq(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_cmp(b1, b2) == 0) return b_one;
    return b32_zero;
}



bytes32_t bytes32_shl(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_cmp(b2, b_256) >= 0) return b32_zero;
    return bytes32_shl_uint(b1, b2.v[0]);
}

bytes32_t bytes32_shr(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_cmp(b2, b_256) >= 0) return b32_zero;
    return bytes32_shr_uint(b1, b2.v[0]);
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
        b1 = bytes##SIZE##_add_uint(b1, b2.v[i], i);    \
    return b1;  \
}

BYTES_N_ADD(32) 
BYTES_N_ADD(64) 

bytes32_t bytes32_mul(bytes32_t b1, bytes32_t b2)
{
    bytes32_t b_res = b32_zero;
    for(int i=0; i<SCALAR32; i++)
    for(int j=0; j+i<SCALAR32; j++)
    {
        luint aux = uint_mul(b1.v[i], b2.v[j]);
        b_res = bytes32_add_uint(b_res, DECL(aux), i+j);
        b_res = bytes32_add_uint(b_res, DECH(aux), i+j+1);
    }
    return b_res;
}

#define BYTES_N_SUB(SIZE)   \
bytes##SIZE##_t bytes##SIZE##_sub(bytes##SIZE##_t b1, bytes##SIZE##_t b2)   \
{   \
    b2 = bytes##SIZE##_not(b2); \
    b1 = bytes##SIZE##_add(b1, b2); \
    return bytes##SIZE##_add_uint(b1, 1, 0);    \
}

BYTES_N_SUB(32)
BYTES_N_SUB(64)

bytes32_t bytes32_div(bytes32_t b1, bytes32_t b2)
{
    bytes32_dual_t bd = bytes32_div_mod(b1, b2);
    return bd.b[1];
}

bytes32_t bytes32_mod(bytes32_t b1, bytes32_t b2)
{
    bytes32_dual_t bd = bytes32_div_mod(b1, b2);
    return bd.b[0];
}

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



bytes64_t bytes64_add_uint(bytes64_t bd, uint u, int i)
{
    if(u == 0) return bd;
    if(i >= SCALAR64) return bd;

    luint lu = uint_add(bd.v[i], u);
    bd.v[i] = DECL(lu);
    return bytes64_add_uint(bd, DECH(lu), i+1);
}



bytes32_dual_t bytes32_full_add(bytes32_t b1, bytes32_t b2)
{
    bytes64_t bd = BYTES64_BYTES32(b1);
    for(int i=0; i<SCALAR32; i++)
        bd = bytes64_add_uint(bd, b2.v[i], i);
    
    return DUAL(bd);
}

bytes32_dual_t bytes32_full_mul(bytes32_t b1, bytes32_t b2)
{
    bytes64_t bd = b64_zero;
    for(int i=0; i<SCALAR32; i++)
    for(int j=0; j<SCALAR32; j++)
    {
        luint lu = uint_mul(b1.v[i], b2.v[j]);
        bd = bytes64_add_uint(bd, DECL(lu), i + j);
        bd = bytes64_add_uint(bd, DECH(lu), i + j + 1);
    }
    
    return DUAL(bd);
}
