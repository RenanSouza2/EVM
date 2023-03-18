#ifndef __BYTES_H__
#define __BYTES_H__

#include "../../../utils/struct.h"
#include "../bytes32/header.h"

typedef unsigned char uchar;

STRUCT(bytes_din)
{
    int n;
    uchar *s;
};

void bytes_din_display(bytes_din_t b);

bytes_din_t bytes_din_create_string(char s[]);
void bytes_din_free(bytes_din_t b);

uchar bytes_din_get(bytes_din_t b, int n);
bytes_din_t bytes_din_get_mult(bytes_din_t b, int n, int size);
bytes32_t bytes32_bytes_din(bytes_din_t b);

#endif
