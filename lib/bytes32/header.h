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

#define BYTES32_RESET(B) memset(&B, 0, 32);

bytes32_t bytes32_is_zero(bytes32_t b);
bytes32_t bytes32_eq(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_lt(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_gt(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_sign_lt(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_sign_gt(bytes32_t b1, bytes32_t b2);

bytes32_t bytes32_and(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_or(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_xor(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_not(bytes32_t b);
bytes32_t bytes32_byte(bytes32_t b1, bytes32_t b2);

bytes32_t bytes32_shl(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_shr(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_sar(bytes32_t b1, bytes32_t b2);

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