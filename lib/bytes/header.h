#ifndef __BYTES_H__
#define __BYTES_H__

#include "../../utils/struct.h"
#include "../bytes32/header.h"

typedef unsigned char uchar;

STRUCT(bytes)
{
    int n;
    uchar *s;
};

void bytes_display(bytes_t b);

bytes_t bytes_create_string(char s[]);
void bytes_free(bytes_t b);

uchar bytes_get(bytes_t b, int n);
bytes_t bytes_get_mult(bytes_t b, int n, int size);
bytes32_t bytes32_bytes(bytes_t b);

#endif
