#include <string.h>

#include <stdio.h>

#include "header.h"

#ifdef DEBUG

#define BYTES32_DISPLAY(BYTES32) \
    printf("\n%s: ", #BYTES32);bytes32_display(BYTES32);

void bytes32_display(bytes32_t b)
{
    printf("0x");
    for(int i=SCALAR-1; i>=0; i--) {
        printf("%08X", b.v[i]);
    }
}

bytes32_t bytes32_set( \
    uint value7, uint value6, uint value5, uint value4, \
    uint value3, uint value2, uint value1, uint value0
)
{
    return (bytes32_t){{ \
        value0, value1, value2, value3, \
        value4, value5, value6, value7 \
    }};
}

bytes32_t bytes32_convert(luint u)
{
    bytes32_t b;
    BYTES32_RESET(b);
    b.v[1] = DECH(u);
    b.v[0] = DECL(u);
    return b;
}

#endif

bytes32_t bytes32_add_uint(bytes32_t b, uint value, int i)
{
    if(i >= SCALAR) return b;
    if(value == 0) return b;

    luint aux = uint_add(b.v[i], value);
    b.v[i] = DECL(aux);

    return bytes32_add_uint(b, DECH(aux), i+1);
}

bytes32_t bytes32_add(bytes32_t b1, bytes32_t b2)
{
    
    for(int i=0; i<SCALAR; i++)
        b1 = bytes32_add_uint(b1, b2.v[i], i);
    return b1;
}

bytes32_t bytes32_sub(bytes32_t b1, bytes32_t b2)
{
    for(int i=0; i<SCALAR; i++)
        b1 = bytes32_add_uint(b1, ~b2.v[i], i);
    return bytes32_add_uint(b1, 1, 0);
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

