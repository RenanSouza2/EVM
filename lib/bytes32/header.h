#ifndef __BYTES32_H__
#define __BYTES32_H__

#include <stdbool.h>

#include "../../utils/struct.h"
#include "../uint/header.h"

#define SCALAR32 8
STRUCT(bytes32)
{
    uint v[SCALAR32];
};

extern const bytes32_t b_zero;
extern const bytes32_t b_one;

bytes32_t bytes32_shl_uint(bytes32_t b, uint shift);
bytes32_t bytes32_shr_uint(bytes32_t b, uint shift);

bytes32_t bytes32_is_zero(bytes32_t b);
bytes32_t bytes32_eq(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_lt(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_gt(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_sign_lt(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_sign_gt(bytes32_t b1, bytes32_t b2);

bytes32_t bytes32_shl(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_shr(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_not(bytes32_t b);

bytes32_t bytes32_add(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_mul(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_sub(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_div(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_mod(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_sdiv(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_smod(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_exp(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_sign_extend(bytes32_t b1, bytes32_t b2);

bytes32_t bytes32_addmod(bytes32_t b1, bytes32_t b2, bytes32_t b3);
bytes32_t bytes32_mulmod(bytes32_t b1, bytes32_t b2, bytes32_t b3);

#endif