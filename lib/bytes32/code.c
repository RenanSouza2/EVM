#include <string.h>

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



#define BYTES32_SET(VALUE7, VALUE6, VALUE5, VALUE4, VALUE3, VALUE2, VALUE1, VALUE0) \
    (bytes32_t){{VALUE0, VALUE1, VALUE2, VALUE3, VALUE4, VALUE5, VALUE6, VALUE7}}

#define BYTES32_UINT(UINT) BYTES32_SET(0, 0, 0, 0, 0, 0, DECH(UINT), DECL(UINT))

const bytes32_t b_zero = (bytes32_t){{  0, 0, 0, 0, 0, 0, 0, 0}};
const bytes32_t b_256  = (bytes32_t){{256, 0, 0, 0, 0, 0, 0, 0}};

bytes32_t bytes32_add_uint(bytes32_t b, uint value, int i)
{
    if(i >= SCALAR) return b;
    if(value == 0) return b;

    luint aux = uint_add(b.v[i], value);
    b.v[i] = DECL(aux);

    return bytes32_add_uint(b, DECH(aux), i+1);
}

bytes32_t bytes32_revert(bytes32_t b)
{
    bytes32_t b_res;
    for(int i=0; i<SCALAR; i++)
        b_res.v[i] = b.v[SCALAR-1-i];
    return b_res;
}



bool bytes32_is_zero_bool(bytes32_t b)
{
    return memcmp(b.v, b_zero.v, 32) == 0;
}

int bytes32_cmp(bytes32_t b1, bytes32_t b2)
{
    b1 = bytes32_revert(b1);
    b2 = bytes32_revert(b2);
    return memcmp(b1.v, b2.v, 32);
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

