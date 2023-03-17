#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "debug.h"

#ifdef DEBUG

int bytes_din_created;

#define BYTES_DIN_INC() bytes_din_created++;
#define BYTES_DIN_DEC() \
    {   \
        assert(bytes_din_created > 0);  \
        bytes_din_created--;    \
    }

bool bytes_din_memory()
{
    return bytes_din_created == 0;
}

void bytes_din_display(bytes_din_t b)
{
    printf("\nbytes size: %d", b.n);
    for(int i=0; i<b.n; i++)
        printf("\nbytes[%d]: %2x", i, b.s[i]);
    printf("\n");
}

#else
#define BYTES_DIN_INC()
#define BYTES_DIN_DEC()
#endif





uchar byte_char_1(uchar c)
{
    switch (c)
    {
        case '0' ... '9' : return c - '0';
        case 'a' ... 'z' : return c + 10 - 'a';
        case 'A' ... 'Z' : return c + 10 - 'A';
        default: assert(false);
    }
}

uchar byte_char_2(uchar c1, uchar c2)
{
    return byte_char_1(c1) << 4 | byte_char_1(c2);
}



bytes_din_t bytes_din_create_string(char s[])
{
    int len = strlen((char*)s);
    assert((len & 1) == 0);

    uchar *s_out = malloc(len >> 1);
    assert(s_out);
    BYTES_DIN_INC();

    for(int i=0; i<len; i += 2)
        s_out[i>>1] = byte_char_2(s[i], s[i+1]);
    
    return (bytes_din_t){len >> 1, s_out};
}

void bytes_din_free(bytes_din_t b)
{
    BYTES_DIN_DEC();
    free(b.s);
}


uchar bytes_din_get(bytes_din_t b, int n)
{
    if(n >= b.n) return 0;
    return b.s[n];
}

bytes_din_t bytes_din_get_mult(bytes_din_t b, int n, int size)
{
    uchar *s = malloc(size);
    assert(s);
    BYTES_DIN_INC();

    for(int i=0; i<size; i++)
        s[i] = bytes_din_get(b, n+i);
    
    return (bytes_din_t){size, s};
}

bytes32_t bytes32_bytes_din(bytes_din_t b)
{
    assert(b.n <= 32);
    bytes32_t b32;
    BYTES32_RESET(b32)
    for(int i=0; i<b.n; i++)
        ((uchar*)&b32)[i] = b.s[b.n-1-i];
    bytes_din_free(b);

    return b32;
}
