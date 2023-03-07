#include "debug.h"

#define BYTES64_BYTES32(B) (*((bytes64_p)&((bytes32_dual_t){{(B), b_zero}})))
#define DUAL(BD) (*((bytes32_dual_p)&(BD)))               

#ifdef DEBUG

#include <stdio.h>
#include <limits.h>

const bytes32_t b_max = BYTES32(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
const bytes32_t b_max_1 = BYTES32(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX - 1);
const bytes32_t b_Q255 = BYTES32(0x80000000, 0, 0, 0, 0, 0, 0, 0);

void bytes32_display(bytes32_t b)
{
    printf("0x");
    for(int i=SCALAR-1; i>=0; i--)
        printf("%08x", b.v[i]);
}

void bytes64_display(bytes64_t bd)
{
    printf("0x");
    for(int i=SCALAR2-1; i>=0; i--)
        printf("%08x", bd.v[i]);
}

#endif


const bytes32_t b_zero = BYTES32_UINT(0);
const bytes32_t b_one = BYTES32_UINT(1);
const bytes32_t b_256 = BYTES32_UINT(256);

const bytes64_t bd_zero = BYTES64_UINT(0);



bool bytes32_is_zero_bool(bytes32_t b)
{
    return memcmp(b.v, b_zero.v, 32) == 0;
}

int bytes32_cmp(bytes32_t b1, bytes32_t b2)
{
    for(int i=SCALAR-1; i>=0; i--)
    {
        if(b1.v[i] > b2.v[i]) return  1;
        if(b1.v[i] < b2.v[i]) return -1;
    }
    return 0;
}



bytes32_t bytes32_add_uint(bytes32_t b, uint u, int i)
{
    if(u == 0) return b;
    if(i >= SCALAR) return b;

    luint lu = uint_add(b.v[i], u);
    b.v[i] = DECL(lu);

    return bytes32_add_uint(b, DECH(lu), i+1);
}

bytes32_t bytes32_shl_uint(bytes32_t b, uint shift)
{
    if(shift > 255) return b_zero;

    int jmp = shift >> 5;
    int off = shift & 31;

    bytes32_t b_out = b_zero;
    b_out.v[jmp] = b.v[0] << off;
    for(int i=1; i+jmp<SCALAR; i++)
    {
        luint lu = LUINT(b.v[i-1]) << off;
        b_out.v[i+jmp] = DECH(lu);
    }

    return b_out;
}

bytes32_t bytes32_shr_uint(bytes32_t b, uint shift)
{
    if(shift > 255) return b_zero;

    int jmp = shift >> 5;
    int off = shift & 31;

    bytes32_t b_out = b_zero;
    b_out.v[SCALAR-1 - jmp] = b.v[SCALAR-1] >> off;
    for(int i=jmp; i<SCALAR-1; i++)
    {
        luint lu = LUINT(b.v[i]) >> off;
        b_out.v[i-jmp] = DECL(lu);
    }
    
    return b_out;
}

bytes32_dual_t bytes32_div_mod(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_is_zero_bool(b2)) return (bytes32_dual_t){{b_zero, b_zero}};

    bytes32_t b1_aux, b_base;
    b1_aux = bytes32_shr_uint(b1, 1);
    b_base = BYTES32_UINT(1);
    while(bytes32_cmp(b1_aux, b2) >= 0)
    {
        b2 = bytes32_shl_uint(b2, 1);
        b_base = bytes32_shl_uint(b_base, 1);
    }

    bytes32_t b_out = b_zero;
    while(!bytes32_is_zero_bool(b_base))
    {
        if(bytes32_cmp(b1, b2) >= 0)
        {
            b1 = bytes32_sub(b1, b2);
            b_out = bytes32_add(b_out, b_base);
        }
        
        b2 = bytes32_shr_uint(b2, 1);
        b_base = bytes32_shr_uint(b_base, 1);
    }

    return (bytes32_dual_t){{b1, b_out}};
}



bytes32_sign_t bytes32_design(bytes32_t b)
{
    if((b.v[SCALAR-1] & 0x80000000) == 0) 
        return (bytes32_sign_t){1, b};
    
    b = bytes32_sub(b_zero, b);
    return (bytes32_sign_t){-1, b};
}

bytes32_t bytes32_sign(bytes32_sign_t bs)
{
    if(bs.sign == 1) return bs.b;
    return bytes32_sub(b_zero, bs.b);
}



bytes32_t bytes32_is_zero(bytes32_t b1)
{
    if(bytes32_is_zero_bool(b1)) return b_one;
    return b_zero;
}

bytes32_t bytes32_lt(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_cmp(b1, b2) < 0) return b_one;
    return b_zero;
}

bytes32_t bytes32_gt(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_cmp(b1, b2) > 0) return b_one;
    return b_zero;
}

bytes32_t bytes32_eq(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_cmp(b1, b2) == 0) return b_one;
    return b_zero;
}



bytes32_t bytes32_shl(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_cmp(b2, b_256) >= 0) return b_zero;
    return bytes32_shl_uint(b1, b2.v[0]);
}

bytes32_t bytes32_shr(bytes32_t b1, bytes32_t b2)
{
    if(bytes32_cmp(b2, b_256) >= 0) return b_zero;
    return bytes32_shr_uint(b1, b2.v[0]);
}



bytes32_t bytes32_add(bytes32_t b1, bytes32_t b2)
{
    
    for(int i=0; i<SCALAR; i++)
        b1 = bytes32_add_uint(b1, b2.v[i], i);
    return b1;
}

bytes32_t bytes32_mul(bytes32_t b1, bytes32_t b2)
{
    bytes32_t b_res = b_zero;
    for(int i=0; i<SCALAR; i++)
    for(int j=0; j+i<SCALAR; j++)
    {
        luint aux = uint_mul(b1.v[i], b2.v[j]);
        b_res = bytes32_add_uint(b_res, DECL(aux), i+j);
        b_res = bytes32_add_uint(b_res, DECH(aux), i+j+1);
    }
    return b_res;
}

bytes32_t bytes32_sub(bytes32_t b1, bytes32_t b2)
{
    for(int i=0; i<SCALAR; i++)
        b1 = bytes32_add_uint(b1, ~b2.v[i], i);
    return bytes32_add_uint(b1, 1, 0);
}

bytes32_t bytes32_div(bytes32_t b1, bytes32_t b2)
{
    bytes32_dual_t bd = bytes32_div_mod(b1, b2);
    return bd.b[1];
}

bytes32_t bytes32_mod(bytes32_t b1, bytes32_t b2)
{
    bytes32_dual_t bd = bytes32_div_mod(b1, b2);
    return bd.b[0];
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



bytes64_t bytes64_add_uint(bytes64_t bd, uint u, int i)
{
    if(u == 0) return bd;
    if(i >= SCALAR2) return bd;

    luint lu = uint_add(bd.v[i], u);
    bd.v[i] = DECL(lu);
    return bytes64_add_uint(bd, DECH(lu), i+1);
}

bytes32_dual_t bytes32_full_add(bytes32_t b1, bytes32_t b2)
{
    bytes64_t bd = BYTES64_BYTES32(b1);
    for(int i=0; i<SCALAR; i++)
        bd = bytes64_add_uint(bd, b2.v[i], i);
    
    return DUAL(bd);
}

bytes32_dual_t bytes32_full_mul(bytes32_t b1, bytes32_t b2)
{
    bytes64_t bd = bd_zero;
    for(int i=0; i<SCALAR; i++)
    for(int j=0; j<SCALAR; j++)
    {
        luint lu = uint_mul(b1.v[i], b2.v[j]);
        bd = bytes64_add_uint(bd, DECL(lu), i + j);
        bd = bytes64_add_uint(bd, DECH(lu), i + j + 1);
    }
    
    return DUAL(bd);
}
