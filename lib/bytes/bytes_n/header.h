#ifndef __BYTES_N_H__
#define __BYTES_N_H__

#include <stdbool.h>

#include "../../uint/header.h"

#define BYTES_N_RESET(SCALAR, B) memset(B, 0, SCALAR << 2)
#define BYTES_N_SET(SCALAR, B1, B2) memcpy(B1, B2, SCALAR << 2)
#define BYTES_N_SET_FIRST(SCALAR, B, UINT)    \
    {   \
        BYTES_N_RESET(SCALAR, B);    \
        B[0] = UINT;    \
    }

bool bytes_n_is_zero(int scalar, const uint b[scalar]);
int bytes_n_cmp(int scalar, const uint b1[scalar], const uint b2[scalar]);

int bytes_n_cmp_uint(int scalar, const uint b[scalar], uint u);
void bytes_n_add_uint(int scalar, uint b[scalar], uint u, int i);
void bytes_n_shl_uint(int scalar, uint b[scalar], uint shift);
void bytes_n_shr_uint(int scalar, uint b[scalar], uint shift);

void bytes_n_not(int scalar, uint b[scalar]);
void bytes_n_minus(int scalar, uint b[scalar]);
void bytes_n_add(int scalar, uint b1[scalar], const uint b2[scalar]);
void bytes_n_sub(int scalar, uint b1[scalar], const uint b2[scalar]);
void bytes_n_mul(int scalar, uint b1[scalar], const uint b2[scalar]);
void bytes_n_div_mod(int scalar, uint b1[scalar], uint b2[scalar]);

#endif
