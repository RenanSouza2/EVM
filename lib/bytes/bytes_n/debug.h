#ifndef __BYTES_N_DEBUG_H__
#define __BYTES_N_DEBUG_H__

#include "header.h"

#include <string.h>

#ifdef DEBUG

#define BYTES_N_DISPLAY(SCALAR, B)  \
    printf("\n%s: ", #B);   \
    bytes_n_display(SCALAR, B); \
    printf("\t\t");

#define BYTES_N_SET_POS(SCALAR, B, UINT, POS)   \
    {   \
        assert(POS < SCALAR);   \
        BYTES_N_RESET(SCALAR, B);   \
        B[POS] = UINT;  \
    }

#define BYTES_N_SET_UINT(SCALAR, B, ARGS...)    \
    {   \
        uint b_tmp_all[] = {ARGS};  \
        assert(sizeof b_tmp_all <= (SCALAR << 2)); \
        \
        BYTES_N_RESET(SCALAR, B);   \
        memcpy(B, b_tmp_all, sizeof b_tmp_all);    \
    }

#define BYTES_N_SET_MAX(SCALAR, B) memset(B, 1, SCALAR << 2)

#define ASSERT_BYTES_N_MUTUAL(SCALAR, B1, B2) assert(memcmp(B1, B2, SCALAR << 2) == 0)

#define ASSERT_BYTES_N(SCALAR, B, UINT...)    \
    {   \
        uint b_tmp[SCALAR]; \
        BYTES_N_SET_UINT(SCALAR, b_tmp, UINT);  \
        ASSERT_BYTES_N_MUTUAL(SCALAR, B, b_tmp);    \
    }

#define ASSERT_BYTES_N_LAST(SCALAR, B, UINT)    \
    {   \
        uint b_tmp_last[SCALAR];    \
        BYTES_N_RESET(SCALAR, b_tmp_last);  \
        b_tmp_last[SCALAR-1] = UINT;    \
        ASSERT_BYTES_N_MUTUAL(SCALAR, B, b_tmp_last);   \
    }

void bytes_n_display(int scalar, const uint b[scalar]);

#endif

#endif
