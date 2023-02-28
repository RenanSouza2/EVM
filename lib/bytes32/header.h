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

#define BYTES32_RESET(VALUE) memset(VALUE.v, 0, 32)

bool bytes32_is_zero_bool(bytes32_t b);

bytes32_t bytes32_add_uint(bytes32_t b, uint u, int i);
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

#endif