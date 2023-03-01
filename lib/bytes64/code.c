#include "debug.h"

#ifdef DEBUG

#include <stdio.h>

void bytes64_display(bytes64_t bd)
{
    printf("0x");
    for(int i=SCALAR_2-1; i>=0; i--)
        printf("%08x", bd.v[i]);
}

#endif

const bytes64_t bd_zero = BYTES64_UINT(0);

bytes64_t bytes64_bytes32(bytes32_t b)
{
    bytes64_t bd = bd_zero;
    *((bytes32_p)&bd) = b;
    return bd;
}