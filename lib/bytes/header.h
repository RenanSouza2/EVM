#ifndef __EXAMPLE_H__
#define __EXAMPLE_H__

#include "../../utils/struct.h"

STRUCT(bytes)
{
    int n;
    char *s;
};

char bytes_get(bytes_t b, int n);
bytes_t bytes_create_string(char s[]);

#endif
