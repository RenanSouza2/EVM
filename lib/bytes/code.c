#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "debug.h"

#ifdef DEBUG
#endif

char byte_char_1(char c)
{
    switch (c)
    {
        case '0' ... '9' : return c - '0';
        case 'a' ... 'z' : return c + 10 - 'a';
        case 'A' ... 'Z' : return c + 10 - 'A';
        default: assert(false);
    }
}

char byte_char_2(char c1, char c2)
{
    return byte_char_1(c1) << 4 | byte_char_1(c2);
}

bytes_t bytes_create_string(char s[])
{
    int len = strlen(s);
    assert((len & 1) == 0);

    char *s_out = malloc(len >> 1);
    assert(s_out);

    for(int i=0; i<len; i += 2)
        s_out[i>>1] = byte_char_2(s[i], s[i+1]);
    
    return (bytes_t){len >> 1, s_out};
}

char bytes_get(bytes_t b, int n)
{
    if(n >= b.n) return 0;
    return b.s[n];
}

bytes_t bytes_get_mult(bytes_t b, int n, int size)
{
    char *s = malloc(size);
    assert(s);

    for(int i=0; i<size; i++)
        s[i] = bytes_get(b, n+i);
    
    return (bytes_t){size, s};
}

bytes32_t bytes32_bytes(bytes_t b)
{
    bytes32_t b32 = b_zero;
    for(int i=0; i<b.n; i++)
        ((char*)&b32)[i] = b.s[b.n-1-i];

    return b32;
}
