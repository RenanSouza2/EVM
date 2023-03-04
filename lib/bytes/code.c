#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "debug.h"

#ifdef DEBUG
#endif

char bytes_get(bytes_t b, int n)
{
    assert(n < b.n);
    return b.s[n];
}

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
