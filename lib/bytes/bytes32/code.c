#include <stdbool.h>
#include <limits.h>
#include <assert.h>

#include "debug.h"
#include "../bytes_n/header.h"

#ifdef DEBUG

#include <stdio.h>

#include "../bytes_n/debug.h"

const bytes32_t b_zero = BYTES32_UINT(0);
const bytes32_t b_one = BYTES32_UINT(1);
const bytes32_t b_32 = BYTES32_UINT(32);
const bytes32_t b_256 = BYTES32_UINT(256);
const bytes32_t b_max = BYTES32(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
const bytes32_t b_max_1 = BYTES32(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX - 1);
const bytes32_t b_Q255 = BYTES32(0x80000000, 0, 0, 0, 0, 0, 0, 0);

void bytes32_display(bytes32_t b)
{
    bytes_n_display(SCALAR32, b.v);
}

#endif


bytes32_sign_t bytes32_design(bytes32_t b)
{
    if((b.v[SCALAR32-1] >> 31) == 0) 
        return (bytes32_sign_t){1, b};
    
    BYTES32_OP_1(minus, b);
    return (bytes32_sign_t){-1, b};
}

bytes32_t bytes32_sign(bytes32_sign_t bs)
{
    if(bs.sign == 1) return bs.b;

    BYTES32_OP_1(minus, bs.b);
    return bs.b;
}

int bytes32_scmp(bytes32_t b1, bytes32_t b2)
{
    bool sign1, sign2;
    sign1 = (b1.v[SCALAR32 - 1] >> 31);
    sign2 = (b2.v[SCALAR32 - 1] >> 31);
    if(sign1 ^ sign2) return sign2 - sign1;

    return BYTES32_OP_2(cmp, b1, b2);
}



bytes32_t bytes32_lt(bytes32_t b1, bytes32_t b2)
{
    return BYTES32_UINT(BYTES32_OP_2(cmp, b1, b2) < 0);
}

bytes32_t bytes32_gt(bytes32_t b1, bytes32_t b2)
{
    return BYTES32_UINT(BYTES32_OP_2(cmp, b1, b2) > 0);
}

bytes32_t bytes32_slt(bytes32_t b1, bytes32_t b2)
{
    return BYTES32_UINT(bytes32_scmp(b1, b2) < 0);
}

bytes32_t bytes32_sgt(bytes32_t b1, bytes32_t b2)
{
    return BYTES32_UINT(bytes32_scmp(b1, b2) > 0);
}

bytes32_t bytes32_eq(bytes32_t b1, bytes32_t b2)
{
    return BYTES32_UINT(BYTES32_OP_2(cmp, b1, b2) == 0);
}

bytes32_t bytes32_is_zero(bytes32_t b)
{
    return BYTES32_UINT(BYTES32_OP_1(is_zero, b));
}



bytes32_t bytes32_and(bytes32_t b1, bytes32_t b2)
{
    for(int i=0; i<SCALAR32; i++)
        b1.v[i] &= b2.v[i];
    return b1;
}

bytes32_t bytes32_or(bytes32_t b1, bytes32_t b2)
{
    for(int i=0; i<SCALAR32; i++)
        b1.v[i] |= b2.v[i];
    return b1;
}

bytes32_t bytes32_xor(bytes32_t b1, bytes32_t b2)
{
    for(int i=0; i<SCALAR32; i++)
        b1.v[i] ^= b2.v[i];
    return b1;
}

bytes32_t bytes32_not(bytes32_t b)
{
    BYTES32_OP_1(not, b);
    return b;
}

bytes32_t bytes32_byte(bytes32_t b1, bytes32_t b2)
{
    if(BYTES32_OP_UINT(cmp, b1, 32) >= 0) return BYTES32_UINT(0);
    
    int pos = b1.v[0] >> 2;
    int off = 3 - (b1.v[0] & 3);

    uint u = (b2.v[SCALAR32-1-pos] >> (off << 3)) & 0xFF;
    BYTES_N_RESET(SCALAR32, &b2);
    b2.v[0] = u;

    return b2;
}



bytes32_t bytes32_shl(bytes32_t b1, bytes32_t b2)
{
    if(BYTES32_OP_UINT(cmp, b1, 256) >= 0) return BYTES32_UINT(0);
    BYTES32_OP_UINT(shl, b2, b1.v[0]);
    return b2;
}

bytes32_t bytes32_shr(bytes32_t b1, bytes32_t b2)
{
    if(BYTES32_OP_UINT(cmp, b1, 256) >= 0) return BYTES32_UINT(0);
    BYTES32_OP_UINT(shr, b2, b1.v[0]);
    return b2;
}

bytes32_t bytes32_sar(bytes32_t b1, bytes32_t b2)
{
    if(BYTES32_OP_1(is_zero, b1)) return b2;
    
    bool b2_sign = (b2.v[SCALAR32-1] >> 31);
    if(BYTES32_OP_UINT(cmp, b1, 256) >= 0) 
        return b2_sign ? BYTES32_MAX() : BYTES32_UINT(0);

    b2 = bytes32_shr(b1, b2);
    if(b2_sign == 0) return b2;

    bytes32_t b_fill = BYTES32(0x80000000, 0, 0, 0, 0, 0, 0, 0);
    BYTES32_OP_UINT(shr, b_fill, b1.v[0] - 1);
    BYTES32_OP_1(minus, b_fill);
    return bytes32_or(b_fill, b2);
}



bytes32_t bytes32_add(bytes32_t b1, bytes32_t b2)
{
    BYTES32_OP_2(add, b1, b2);
    return b1;
}

bytes32_t bytes32_mul(bytes32_t b1, bytes32_t b2)
{
    BYTES32_OP_2(mul, b1, b2);
    return b1;
}

bytes32_t bytes32_sub(bytes32_t b1, bytes32_t b2)
{
    BYTES32_OP_2(sub, b1, b2); \
    return b1;
}

bytes32_t bytes32_div(bytes32_t b1, bytes32_t b2)
{
    BYTES32_OP_2(div_mod, b1, b2);
    return b2;
}

bytes32_t bytes32_mod(bytes32_t b1, bytes32_t b2)
{
    BYTES32_OP_2(div_mod, b1, b2);
    return b1;
}

bytes32_t bytes32_sdiv(bytes32_t b1, bytes32_t b2)
{
    bytes32_sign_t bs1, bs2;
    bs1 = bytes32_design(b1);
    bs2 = bytes32_design(b2);

    bytes32_sign_t bs;
    bs.sign = bs1.sign * bs2.sign;
    bs.b = bytes32_div(bs1.b, bs2.b);
    return bytes32_sign(bs);
}

bytes32_t bytes32_smod(bytes32_t b1, bytes32_t b2)
{
    bytes32_sign_t bs1, bs2;
    bs1 = bytes32_design(b1);
    bs2 = bytes32_design(b2);

    bytes32_sign_t bs;
    bs.sign = bs1.sign * bs2.sign;
    bs.b = bytes32_mod(bs1.b, bs2.b);
    return bytes32_sign(bs);
}

bytes32_t bytes32_exp(bytes32_t b1, bytes32_t b2)
{
    if(BYTES32_OP_1(is_zero, b2)) return BYTES32_UINT(1);
    if(BYTES32_OP_UINT(cmp, b2, 1) == 0) return b1;

    bytes32_t b_res;
    bool is_odd = b2.v[0] & 1;
    BYTES32_OP_UINT(shr, b2, 1);
    b_res = bytes32_exp(b1, b2);
    b_res = bytes32_mul(b_res, b_res);

    if(is_odd) return bytes32_mul(b1, b_res);
    return b_res;
}

bytes32_t bytes32_sign_extend(bytes32_t b1, bytes32_t b2)
{
    if(BYTES32_OP_UINT(cmp, b1, 32) >= 0) return b2;

    int off = b1.v[0];
    uint value = (b2.v[off] >> 31) ? UINT_MAX : 0;
    for(int i=off+1; i<SCALAR32; i++)
        b2.v[i] = value;
    return b2;
}



bytes32_t bytes32_addmod(bytes32_t b1, bytes32_t b2, bytes32_t b3)
{
    bytes64_t b64_1, b64_2;
    BYTES64_BYTES32(b64_1, b1);
    BYTES64_BYTES32(b64_2, b2);
    BYTES64_OP_2(add, b64_1, b64_2);

    BYTES64_BYTES32(b64_2, b3);
    BYTES64_OP_2(div_mod, b64_1, b64_2);

    return BYTES32_BYTES64(b64_2);
}

bytes32_t bytes32_mulmod(bytes32_t b1, bytes32_t b2, bytes32_t b3)
{
    bytes64_t b64_1, b64_2;
    BYTES64_BYTES32(b64_1, b1);
    BYTES64_BYTES32(b64_2, b2);
    BYTES64_OP_2(mul, b64_1, b64_2);

    BYTES64_BYTES32(b64_2, b3);
    BYTES64_OP_2(div_mod, b64_1, b64_2);

    return BYTES32_BYTES64(b64_2);
}
