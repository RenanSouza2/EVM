#include "debug.h"

#ifdef DEBUG

#include <stdio.h>
#include <limits.h>

const bytes32_t b_max = BYTES32(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
const bytes32_t b_max_1 = BYTES32(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX - 1);

void bytes32_display(bytes32_t b)
{
    printf("0x");
    for(int i=SCALAR-1; i>=0; i--)
        printf("%08x", b.v[i]);
}

#endif

const bytes32_t b_zero = BYTES32_UINT(0);
const bytes32_t b_one = BYTES32_UINT(1);
const bytes32_t b_256 = BYTES32_UINT(256);



bool bytes32_is_zero_bool(bytes32_t b)
{
    return memcmp(b.v, b_zero.v, 32) == 0;
}

int bytes32_cmp(bytes32_t b1, bytes32_t b2)
{
    for(int i=SCALAR-1; i>=0; i--)
    {
        if(b1.v[i] > b2.v[i]) return  1;
        if(b1.v[i] < b2.v[i]) return -1;
    }
    return 0;
}



bytes32_t bytes32_add_uint(bytes32_t b, uint u, int i)
{
    if(u == 0) return b;
    if(i >= SCALAR) return b;

    luint lu = uint_add(b.v[i], u);
    b.v[i] = DECL(lu);

    return bytes32_add_uint(b, DECH(lu), i+1);
}

bytes32_t bytes32_shl_uint(bytes32_t b, uint shift)
{
    if(shift > 255) return b_zero;

    int jmp = shift >> 5;
    int off = shift & 31;

    bytes32_t b_res = b_zero;
    b_res.v[jmp] = uint_mix(0, b.v[0], off);
    for(int i=1; i+jmp<SCALAR; i++)
        b_res.v[i + jmp] = uint_mix(b.v[i-1], b.v[i], off);

    return b_res;
}

bytes32_t bytes32_shr_uint(bytes32_t b, uint shift)
{
    if(shift > 255) return b_zero;

    int jmp = shift >> 5;
    int off = 32 - (shift & 31);

    bytes32_t b_res = b_zero;
    b_res.v[SCALAR-1 - jmp] = uint_mix(b.v[SCALAR-1], 0, off);
    for(int i=jmp; i<SCALAR-1; i++)
        b_res.v[i-jmp] = uint_mix(b.v[i], b.v[i+1], off);
    
    return b_res;
}



bytes32_dual_t bytes32_div_mod(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_is_zero_bool(b2)) return (bytes32_dual_t){b_zero, b_zero};

    bytes32_t b_base = BYTES32_UINT(1);
    while(bytes32_cmp(b1, b2) >= 0)
    {
        b2 = bytes32_shl_uint(b2, 1);
        b_base = bytes32_shl_uint(b_base, 1);
    }

    b2 = bytes32_shr_uint(b2, 1);
    b_base = bytes32_shr_uint(b_base, 1);

    bytes32_t b_res = b_zero;
    while(!bytes32_is_zero_bool(b_base))
    {
        if(bytes32_cmp(b1, b2) >= 0)
        {
            b1 = bytes32_sub(b1, b2);
            b_res = bytes32_add(b_res, b_base);
        }
        
        b2 = bytes32_shr_uint(b2, 1);
        b_base = bytes32_shr_uint(b_base, 1);
    }

    return (bytes32_dual_t){b_res, b1};
}



bytes32_t bytes32_is_zero(bytes32_t b1)
{
    if(bytes32_is_zero_bool(b1)) return b_one;
    return b_zero;
}

bytes32_t bytes32_lt(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_cmp(b1, b2) < 0) return b_one;
    return b_zero;
}

bytes32_t bytes32_gt(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_cmp(b1, b2) > 0) return b_one;
    return b_zero;
}

bytes32_t bytes32_eq(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_cmp(b1, b2) == 0) return b_one;
    return b_zero;
}



bytes32_t bytes32_shl(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_cmp(b2, b_256) >= 0) return b_zero;
    return bytes32_shl_uint(b1, b2.v[0]);
}

bytes32_t bytes32_shr(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_cmp(b2, b_256) >= 0) return b_zero;
    return bytes32_shr_uint(b1, b2.v[0]);
}



bytes32_t bytes32_add(bytes32_t b1, bytes32_t b2)
{
    
    for(int i=0; i<SCALAR; i++)
        b1 = bytes32_add_uint(b1, b2.v[i], i);
    return b1;
}

bytes32_t bytes32_mul(bytes32_t b1, bytes32_t b2)
{
    bytes32_t b_res;
    BYTES32_RESET(b_res);
    for(int i=0; i<SCALAR; i++)
    for(int j=0; j+i<SCALAR; j++)
    {
        luint aux = uint_mul(b1.v[i], b2.v[j]);
        b_res = bytes32_add_uint(b_res, DECL(aux), i+j);
        b_res = bytes32_add_uint(b_res, DECH(aux), i+j+1);
    }
    return b_res;
}

bytes32_t bytes32_sub(bytes32_t b1, bytes32_t b2)
{
    for(int i=0; i<SCALAR; i++)
        b1 = bytes32_add_uint(b1, ~b2.v[i], i);
    return bytes32_add_uint(b1, 1, 0);
}

bytes32_t bytes32_div(bytes32_t b1, bytes32_t b2)
{
    bytes32_dual_t bd = bytes32_div_mod(b1, b2);
    return bd.b1;
}

bytes32_t bytes32_mod(bytes32_t b1, bytes32_t b2)
{
    bytes32_dual_t bd = bytes32_div_mod(b1, b2);
    return bd.b2;
}


