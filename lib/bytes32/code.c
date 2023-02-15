#include <string.h>
#include <stdbool.h>

#include "header.h"



#ifdef DEBUG
#include <stdio.h>

void bytes32_display(bytes32_t b)
{
    printf("0x");
    for(int i=SCALAR-1; i>=0; i--) {
        printf("%08X", b.v[i]);
    }
}

#define BYTES32_DISPLAY(BYTES32) \
    printf("\n%s: ", #BYTES32);bytes32_display(BYTES32);

#endif



#define BYTES32(VALUE7, VALUE6, VALUE5, VALUE4, VALUE3, VALUE2, VALUE1, VALUE0) \
    (bytes32_t){{VALUE0, VALUE1, VALUE2, VALUE3, VALUE4, VALUE5, VALUE6, VALUE7}}

#define BYTES32_UINT(UINT) BYTES32(0, 0, 0, 0, 0, 0, DECH(UINT), DECL(UINT))

const bytes32_t b_zero = BYTES32_UINT(0);
const bytes32_t b_256  = BYTES32_UINT(256);

bytes32_t bytes32_add_uint(bytes32_t b, uint u, int i)
{
    if(i >= SCALAR) return b;
    if(u == 0) return b;

    luint lu = uint_add(b.v[i], u);
    b.v[i] = DECL(lu);

    return bytes32_add_uint(b, DECH(lu), i+1);
}

// bytes32_t bytes32_revert(bytes32_t b)
// {
//     bytes32_t b_res;
//     for(int i=0; i<SCALAR; i++)
//         b_res.v[i] = b.v[SCALAR-1-i];
//
//     return b_res;
// }

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
