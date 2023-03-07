#ifndef __BYTES32_H__
#define __BYTES32_H__

#include <stdbool.h>

#include "../../utils/struct.h"
#include "../uint/header.h"

#define SCALAR 8
STRUCT(bytes32)
{
    uint v[SCALAR];
};

STRUCT(bytes32_dual)
{
    bytes32_t b[2];
};

extern const bytes32_t b_zero;

bool bytes32_is_zero_bool(bytes32_t b);
int bytes32_cmp(bytes32_t b1, bytes32_t b2);

bytes32_t bytes32_shl_uint(bytes32_t b, uint shift);
bytes32_t bytes32_shr_uint(bytes32_t b, uint shift);

bytes32_t bytes32_is_zero(bytes32_t b1);
bytes32_t bytes32_lt(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_gt(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_eq(bytes32_t b1, bytes32_t b2);

bytes32_t bytes32_shl(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_shr(bytes32_t b1, bytes32_t b2);

bytes32_t bytes32_add(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_mul(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_sub(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_div(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_mod(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_sdiv(bytes32_t b1, bytes32_t b2);

bytes32_dual_t bytes32_full_add(bytes32_t b1, bytes32_t b2);
bytes32_dual_t bytes32_full_mul(bytes32_t b1, bytes32_t b2);

#endif