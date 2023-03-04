#ifndef __EXAMPLE_DEBUG_H__
#define __EXAMPLE_DEBUG_H__

#include <string.h>

#include "header.h"

#define ASSERT_BYTES(BYTES, SIZE, ARGS...)  \
    {   \
        assert(BYTES.n == SIZE); \
        char b_exp[] = {ARGS};  \
        assert(memcmp(b.s, b_exp, SIZE) == 0);  \
    }

char byte_char_1(char c);
char byte_char_2(char c1, char c2);

#endif
