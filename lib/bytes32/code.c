#include <string.h>

#include <stdio.h>

#include "header.h"

#ifdef DEBUG

void bytes32_display(bytes32 value)
{
    printf("0x");
    for(int i=SCALAR-1; i>=0; i--) {
        printf("%08X", value[i]);
    }
}

void bytes32_set(bytes32 res, \
    uint value7, \
    uint value6, \
    uint value5, \
    uint value4, \
    uint value3, \
    uint value2, \
    uint value1, \
    uint value0
)
{
    res[7] = value7;
    res[6] = value6;
    res[5] = value5;
    res[4] = value4;
    res[3] = value3;
    res[2] = value2;
    res[1] = value1;
    res[0] = value0;
}

void bytes32_convert(bytes32 res, luint value_int)
{
    BYTES32_RESET(res);
    res[1] = DECH(value_int);
    res[0] = DECL(value_int);
}

#endif

void bytes32_add_uint(bytes32 res, uint value, int i)
{
    if(i >= SCALAR) return;
    if(value == 0) return;

    luint aux = uint_add(res[i], value);
    res[i] = DECL(aux);

    bytes32_add_uint(res, DECH(aux), i+1);
}

void bytes32_add(bytes32 res, bytes32 value1, bytes32 value2)
{
    BYTES32_ASSIGN(res, value1);
    for(int i=0; i<SCALAR; i++)
        bytes32_add_uint(res, value2[i], i);
}

void bytes32_mul(bytes32 res, bytes32 value1, bytes32 value2)
{
    BYTES32_RESET(res);
    for(int i=0; i<SCALAR; i++)
    for(int j=0; j+i<SCALAR; j++)
    {
        luint aux = uint_mul(value1[i], value2[j]);
        bytes32_add_uint(res, DECL(aux), i+j);
        bytes32_add_uint(res, DECH(aux), i+j+1);
    }
}
