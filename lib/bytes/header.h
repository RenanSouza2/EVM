#ifndef __EXAMPLE_H__
#define __EXAMPLE_H__

#include "../../utils/struct.h"
#include "../bytes32/header.h"

STRUCT(bytes)
{
    int n;
    char *s;
};

bytes_t bytes_create_string(char s[]);

char bytes_get(bytes_t b, int n);
bytes_t bytes_get_mult(bytes_t b, int n, int size);
bytes32_t bytes32_bytes(bytes_t b);

#endif
