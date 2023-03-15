#ifndef __BYTES_N_DEBUG_H__
#define __BYTES_N_DEBUG_H__

#include "header.h"

#include <string.h>

#define BYTES_N_RESET(SCALAR, B) memset(B, 0, SCALAR << 2)
#define BYTES_N_SET(SCALAR, B1, B2) memcpy(B1, B2, SCALAR << 2)
#define BYTES_N_SET_UINT(SCALAR, B, UINT)    \
    {   \
        BYTES_N_RESET(SCALAR, B);    \
        B[0] = UINT;    \
    }

#ifdef DEBUG

#define BYTES_N_SET_ALL(SACALAR, B, ARGS...)    \
    {   \
        uint b_tmp[] = {ARGS};  \
        BYTES_N_SET(SCALAR, B, b_tmp);  \
    }

#define ASSERT_BYTES_N(SCALAR, B1, B2) memcmp(B1, B2, SCALAR << 2)

#define ASSERT_BYTES_N_UINT(SCALAR, B, UINT)    \
    {   \
        uint b_tmp[SCALAR]; \
        BYTES_N_SET_UINT(SCALAR, b_tmp, UINT);  \
        ASSERT_BYTES_N(SCLAR, B, b_tmp);    \
    }

void bytes_n_display(int scalar, const uint b[scalar]);

#endif

#endif
