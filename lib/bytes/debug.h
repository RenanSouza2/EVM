#ifndef __BYTES_DEBUG_H__
#define __BYTES_DEBUG_H__

#include <string.h>

#include "header.h"

#define ASSERT_BYTES(BYTES, SIZE, ARGS...)  \
    {   \
        assert(BYTES.n == SIZE); \
        uchar b_exp[] = {ARGS};  \
        assert(memcmp(BYTES.s, b_exp, SIZE) == 0);  \
    }

uchar byte_char_1(uchar c);
uchar byte_char_2(uchar c1, uchar c2);

bool bytes_memory();

#endif
