#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

#include "../debug.h"



void test_is_zero_bool()
{
    printf("\n\t\t\ttest is zero bool");

    bytes32_t b = BYTES32_UINT(0);
    bool is_zero = bytes32_is_zero_bool(b);
    assert(is_zero == true);

    for(int i=0; i<SCALAR32; i++)
    {
        b = BYTES32_UINT(0);
        b = bytes32_add_uint(b, 1, i);
        is_zero = bytes32_is_zero_bool(b);
        assert(is_zero == false);
    }

    is_zero = bytes32_is_zero_bool(b_max);
    assert(is_zero == false);
}

void test_cmp()
{
    printf("\n\t\t\ttest cmp");

    bytes32_t b1, b2;
    b1 = BYTES32_UINT(0);
    b2 = BYTES32_UINT(5);
    int res = bytes32_cmp(b1, b2);
    assert(res < 0);

    b1 = BYTES32_UINT(0);
    b2 = BYTES32_UINT(0);
    res = bytes32_cmp(b1, b1);
    assert(res == 0);

    b1 = BYTES32_UINT(5);
    b2 = BYTES32_UINT(5);
    res = bytes32_cmp(b1, b2);
    assert(res == 0);

    b1 = BYTES32_UINT(5);
    b2 = BYTES32_UINT(0);
    res = bytes32_cmp(b1, b2);
    assert(res > 0);

    b1 = BYTES32_UINT(5);
    b2 = BYTES32(UINT_MAX, 0, 0, 0, 0, 0, 0, 0);
    res = bytes32_cmp(b1, b2);
    assert(res < 0);

    b1 = BYTES32(UINT_MAX, 0, 0, 0, 0, 0, 0, 0);
    b2 = BYTES32_UINT(5);
    res = bytes32_cmp(b1, b2);
    assert(res > 0);

    b1 = BYTES32_UINT(255);
    b2 = BYTES32_UINT(256);
    res = bytes32_cmp(b1, b2);
    assert(res < 0); 
}

void test_base_compare()
{
    printf("\n\t\ttest base compare");

    test_is_zero_bool();
    test_cmp();
}



void test_add_uint()
{
    printf("\n\t\t\ttest add uint");

    bytes32_t b = BYTES32(7, 6, 5, 4, 3, 2, 1, 0);
    
    for(int i=0; i<SCALAR32; i++)
    {
        b = bytes32_add_uint(b, 1, i);
        assert(b.v[i] == i+1);
    }

    b = BYTES32_UINT(UINT_MAX);
    b = bytes32_add_uint(b, 1, 0);
    assert(b.v[1] == 1);
    assert(b.v[0] == 0);

    
    b = BYTES32(0, 0, 0, 0, 0, 0, UINT_MAX, UINT_MAX);
    b = bytes32_add_uint(b, 1, 0);
    assert(b.v[2] == 1);
    assert(b.v[1] == 0);
    assert(b.v[0] == 0);

    b = BYTES32_UINT(0);
    b = bytes32_add_uint(b, 1, 8);
    ASSERT_BYTES32_UINT(b, 0);
}

void test_shl_uint()
{
    printf("\n\t\t\ttest shl uint\t\t");

    bytes32_t b = BYTES32_UINT(1);
    b = bytes32_shl_uint(b, 0);
    ASSERT_BYTES32_UINT(b, 1);

    b = BYTES32_UINT(1);
    b = bytes32_shl_uint(b, 1);
    ASSERT_BYTES32_UINT(b, 2);
    
    b = BYTES32_UINT(1);
    b = bytes32_shl_uint(b, 16);
    ASSERT_BYTES32_UINT(b, 0x10000);

    b = BYTES32_UINT(1);
    b = bytes32_shl_uint(b, 31);
    ASSERT_BYTES32_UINT(b, 0x80000000);

    b = BYTES32_UINT(1);
    b = bytes32_shl_uint(b, 32);
    ASSERT_BYTES32_UINT(b, 0x100000000);
    
    b = BYTES32(    \
        0x88888888, 0x77777777, 0x66666666, 0x55555555, \
        0x44444444, 0x33333333, 0x22222222, 0x11111111  \
    );
    b = bytes32_shl_uint(b, 48);
    ASSERT_BYTES32(b,   \
        0x77776666, 0x66665555, 0x55554444, 0x44443333, \
        0x33332222, 0x22221111, 0x11110000, 0   \
    );
    
    b = BYTES32_UINT(1);
    b = bytes32_shl_uint(b, 256);
    ASSERT_BYTES32_UINT(b, 0);

    b = BYTES32_UINT(1);
    b = bytes32_shl_uint(b, 257);
    ASSERT_BYTES32_UINT(b, 0);

    b = BYTES32(0, 0, 0, 1, 0, 0, 0, 0);
    b = bytes32_shl_uint(b, 128);
    ASSERT_BYTES32_UINT(b, 0);

    b = BYTES32_UINT(1);
    b = bytes32_shl_uint(b, 255);
    ASSERT_BYTES32_MUTUAL(b, b_Q255);
}

void test_shr_uint()
{
    printf("\n\t\t\ttest shr uint\t\t");

    luint lu = 0x80000000;
    bytes32_t b = BYTES32_UINT(lu);
    b = bytes32_shr_uint(b, 0);
    ASSERT_BYTES32_UINT(b, lu);

    b = BYTES32_UINT(lu);
    b = bytes32_shr_uint(b, 1);
    ASSERT_BYTES32_UINT(b, 0x40000000);

    b = BYTES32_UINT(lu);
    b = bytes32_shr_uint(b, 16);
    ASSERT_BYTES32_UINT(b, 0x8000);

    b = BYTES32_UINT(lu);
    b = bytes32_shr_uint(b, 31);
    ASSERT_BYTES32_UINT(b, 1);

    b = BYTES32_UINT(lu);
    b = bytes32_shr_uint(b, 32);
    ASSERT_BYTES32_UINT(b, 0);

    b = BYTES32(    \
        0x88888888, 0x77777777, 0x66666666, 0x55555555, \
        0x44444444, 0x33333333, 0x22222222, 0x11111111  \
    );
    b = bytes32_shr_uint(b, 48);
    ASSERT_BYTES32(b,   \
                 0,     0x8888, 0x88887777, 0x77776666, \
        0x66665555, 0x55554444, 0x44443333, 0x33332222  \
    );
}

void test_base_operations()
{
    printf("\n\t\ttest base operations");

    test_add_uint();
    test_shl_uint();
    test_shr_uint();
}



void test_design()
{
    printf("\n\t\t\ttest design\t\t");

    bytes32_sign_t bs = bytes32_design(b_zero);
    assert(bs.sign == 1);
    ASSERT_BYTES32_UINT(bs.b, 0);
    
    bs = bytes32_design(b_one);
    assert(bs.sign == 1);
    ASSERT_BYTES32_UINT(bs.b, 1);
    
    bytes32_t b = BYTES32(    \
        0x7FFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, \
        0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF, 0XFFFFFFFF  \
    );
    bs = bytes32_design(b);
    assert(bs.sign == 1);
    ASSERT_BYTES32_MUTUAL(bs.b, b);
    
    bs = bytes32_design(b_max);
    assert(bs.sign == -1);
    ASSERT_BYTES32_UINT(bs.b, 1);

    bs = bytes32_design(b_Q255);
    assert(bs.sign == -1);
    ASSERT_BYTES32_MUTUAL(bs.b, b_Q255);
}

void test_sign()
{
    printf("\n\t\t\ttest sign\t\t");

    bytes32_sign_t bs32 = (bytes32_sign_t){1, b_zero};
    bytes32_t b = bytes32_sign(bs32);
    ASSERT_BYTES32_UINT(b, 0);
    
    bs32 = (bytes32_sign_t){-1, b_zero};
    b = bytes32_sign(bs32);
    ASSERT_BYTES32_UINT(b, 0);
    
    bs32 = (bytes32_sign_t){1, b_one};
    b = bytes32_sign(bs32);
    ASSERT_BYTES32_UINT(b, 1);
    
    bs32 = (bytes32_sign_t){-1, b_one};
    b = bytes32_sign(bs32);
    ASSERT_BYTES32_MUTUAL(b, b_max);
    
    bs32 = (bytes32_sign_t){1, b_Q255};
    b = bytes32_sign(bs32);
    ASSERT_BYTES32_MUTUAL(b, b_Q255);
    
    bs32 = (bytes32_sign_t){-1, b_Q255};
    b = bytes32_sign(bs32);
    ASSERT_BYTES32_MUTUAL(b, b_Q255);
}

void test_base_sign()
{
    printf("\n\t\ttest base sign");

    test_design();
    test_sign();
}

void test_base()
{
    printf("\n\ttest base");

    test_base_compare();
    test_base_operations();
    test_base_sign();
}



void test_is_zero()
{
    printf("\n\t\t\ttest is zero");

    bytes32_t b_in, b_out;
    b_in = BYTES32_UINT(0);
    b_out = bytes32_is_zero(b_in);
    ASSERT_BYTES32_UINT(b_out, 1);

    for(int i=0; i<SCALAR32; i++)
    {
        b_in = BYTES32_UINT(0);
        b_in.v[i] = 1;

        b_out = bytes32_is_zero(b_in);
        ASSERT_BYTES32_UINT(b_out, 0);
    }
}

void test_lt()
{
    printf("\n\t\t\ttest lt");

    bytes32_t b, b1, b2;
    b1 = BYTES32_UINT(UINT_MAX);
    b2 = BYTES32_UINT(0);
    b = bytes32_lt(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
    
    b1 = BYTES32_UINT(0);
    b2 = BYTES32_UINT(0);
    b = bytes32_lt(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
    
    b1 = BYTES32_UINT(0);
    b2 = BYTES32_UINT(UINT_MAX);
    b = bytes32_lt(b1, b2);
    ASSERT_BYTES32_UINT(b, 1);
}

void test_gt()
{
    printf("\n\t\t\ttest gt");

    bytes32_t b, b1, b2;
    b1 = BYTES32_UINT(UINT_MAX);
    b2 = BYTES32_UINT(0);
    b = bytes32_gt(b1, b2);
    ASSERT_BYTES32_UINT(b, 1);
    
    b1 = BYTES32_UINT(0);
    b2 = BYTES32_UINT(0);
    b = bytes32_gt(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
    
    b1 = BYTES32_UINT(0);
    b2 = BYTES32_UINT(UINT_MAX);
    b = bytes32_gt(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
}

void test_eq()
{
    printf("\n\t\t\ttest eq");

    bytes32_t b, b1, b2;
    b1 = BYTES32_UINT(UINT_MAX);
    b2 = BYTES32_UINT(0);
    b = bytes32_eq(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
    
    b1 = BYTES32_UINT(0);
    b2 = BYTES32_UINT(0);
    b = bytes32_eq(b1, b2);
    ASSERT_BYTES32_UINT(b, 1);
    
    b1 = BYTES32_UINT(0);
    b2 = BYTES32_UINT(UINT_MAX);
    b = bytes32_eq(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
}

void test_bytes32_compare()
{
    printf("\n\t\ttest bytes compare");

    test_is_zero();
    test_lt();
    test_gt();
    test_eq();
}



void test_shl()
{
    printf("\n\t\t\ttest shl");

    bytes32_t b, b1, b2;
    b1 = BYTES32_UINT(1);
    b2 = BYTES32_UINT(255);
    b  = bytes32_shl(b1, b2);
    ASSERT_BYTES32_MUTUAL(b, b_Q255);
    
    b1 = BYTES32_UINT(1);
    b2 = BYTES32_UINT(256);
    b  = bytes32_shl(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
}

void test_shr()
{
    printf("\n\t\t\ttest shr");

    bytes32_t b;
    b = BYTES32_UINT(255);
    b = bytes32_shr(b_Q255, b);
    ASSERT_BYTES32_UINT(b, 1);
    
    b = BYTES32_UINT(256);
    b = bytes32_shr(b_Q255, b);
    ASSERT_BYTES32_UINT(b, 0);

    b = BYTES32_UINT(2);
    b = bytes32_shr_uint(b, 1);
    ASSERT_BYTES32_UINT(b, 1);
}

void test_not()
{
    printf("\n\t\t\ttest not");

    bytes32_t b = bytes32_not(b_zero);
    ASSERT_BYTES32_MUTUAL(b, b_max);

    b = bytes32_not(b_one);
    ASSERT_BYTES32_MUTUAL(b, b_max_1);
}

void test_shift()
{
    printf("\n\t\ttest shift");

    test_shl();
    test_shr();
    test_not();
}



void test_add()
{
    printf("\n\t\t\ttest add");

    bytes32_t b_res, b1, b2;
    b1 = BYTES32(7, 6, 5, 4, 3, 2, 1, 0);
    b2 = BYTES32(0, 1, 2, 3, 4, 5, 6, 7);
    b_res = bytes32_add(b1, b2);
    ASSERT_BYTES32(b_res, 7, 7, 7, 7, 7, 7, 7, 7);
    
    b1 = BYTES32(7, 6, 5, 4, 3, 2, 1, 0);
    b2 = BYTES32(0, 1, 2, 3, 4, 5, 6, 7);
    b_res = bytes32_add(b1, b2);

    b_res = bytes32_add(b_max, b_max);
    ASSERT_BYTES32_MUTUAL(b_res, b_max_1);
}

void test_mul()
{
    printf("\n\t\t\ttest mul");

    bytes32_t b_res, b1, b2;
    b1 = BYTES32_UINT(2);
    b2 = BYTES32_UINT(3);
    b_res = bytes32_mul(b1, b2);
    ASSERT_BYTES32_UINT(b_res, 6);

    b1 = BYTES32(1, 1, 1, 1, 1, 1, 1, 1);
    b2 = BYTES32(1, 1, 1, 1, 1, 1, 1, 1);
    b_res = bytes32_mul(b1, b2);
    ASSERT_BYTES32(b_res, 8, 7, 6, 5, 4, 3, 2, 1);

    b_res = bytes32_mul(b_max, b_max);
    ASSERT_BYTES32_UINT(b_res, 1);
}

void test_sub()
{
    printf("\n\t\t\ttest sub");

    bytes32_t b_res, b1, b2;
    b1 = BYTES32_UINT(5);
    b2 = BYTES32_UINT(3);
    b_res = bytes32_sub(b1, b2);
    ASSERT_BYTES32_UINT(b_res, 2);
    
    b1 = BYTES32_UINT(0);
    b2 = BYTES32_UINT(1);
    b_res = bytes32_sub(b1, b2);
    ASSERT_BYTES32_MUTUAL(b_res, b_max);

    b1 = BYTES32_UINT(0);
    b2 = BYTES32_UINT(0);
    b_res = bytes32_sub(b1, b2);
    ASSERT_BYTES32_UINT(b_res, 0);

    b1 = BYTES32(7, 6, 5, 4, 3, 2, 1, 0);
    b2 = BYTES32(0, 1, 2, 3, 4, 5, 6, 7);
    b_res = bytes32_sub(b1, b2);
    ASSERT_BYTES32(b_res, 7, 5, 3, 0, 0xFFFFFFFE, 0xFFFFFFFC, 0xFFFFFFFA, 0xFFFFFFF9);
    
    b1 = BYTES32(7, 6, 5, 4, 3, 2, 1, 0);
    b2 = BYTES32(0, 1, 2, 3, 4, 5, 6, 7);
    b_res = bytes32_sub(b1, b2);

    b_res = bytes32_sub(b_max, b_max);
    ASSERT_BYTES32_UINT(b_res, 0);
}

void test_div()
{
    printf("\n\t\t\ttest div\t\t");

    printf("\n\t\t\t\ttest div 1\t\t");
    bytes32_t b, b1, b2;
    b1 = BYTES32_UINT(108);
    b2 = BYTES32_UINT(5);
    b = bytes32_div(b1, b2);
    ASSERT_BYTES32_UINT(b, 21);
    
    printf("\n\t\t\t\ttest div 2\t\t");
    b1 = BYTES32_UINT(80);
    b2 = BYTES32_UINT(5);
    b = bytes32_div(b1, b2);
    ASSERT_BYTES32_UINT(b, 16);
    
    printf("\n\t\t\t\ttest div 3\t\t");
    b1 = BYTES32_UINT(2);
    b2 = BYTES32_UINT(5);
    b = bytes32_div(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
    
    printf("\n\t\t\t\ttest div 4\t\t");
    b1 = BYTES32_UINT(2);
    b2 = BYTES32_UINT(0);
    b = bytes32_div(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
    
    printf("\n\t\t\t\ttest div 5\t\t");
    b1 = BYTES32_UINT(0);
    b2 = BYTES32_UINT(5);
    b = bytes32_div(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);

    printf("\n\t\t\t\ttest div 6\t\t");
    b = bytes32_div(b_Q255, b_one);
    ASSERT_BYTES32_MUTUAL(b, b_Q255);

    printf("\n\t\t\t\ttest div 7\t\t");
    b = BYTES32_UINT(5);
    b = bytes32_div(b, b);
    ASSERT_BYTES32_UINT(b, 1);

    printf("\n\t\t\t\ttest div 8\t\t");
    b = bytes32_div(b_max, b_one);
    ASSERT_BYTES32_MUTUAL(b, b_max);

    printf("\n\t\t\t\ttest div 9\t\t");
    b = bytes32_div(b_one, b_max);
    ASSERT_BYTES32_UINT(b, 0);
    

    printf("\n\t\t\t\ttest div 9\t\t");
    b = bytes32_div(b_max, b_max);
    ASSERT_BYTES32_UINT(b, 1);
}

void test_mod()
{
    printf("\n\t\t\ttest mod\t\t");

    bytes32_t b, b1, b2;
    b1 = BYTES32_UINT(108);
    b2 = BYTES32_UINT(5);
    b = bytes32_mod(b1, b2);
    ASSERT_BYTES32_UINT(b, 3);
    
    b1 = BYTES32_UINT(80);
    b2 = BYTES32_UINT(5);
    b = bytes32_mod(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
    
    b1 = BYTES32_UINT(2);
    b2 = BYTES32_UINT(5);
    b = bytes32_mod(b1, b2);
    ASSERT_BYTES32_UINT(b, 2);
    
    b1 = BYTES32_UINT(2);
    b2 = BYTES32_UINT(0);
    b = bytes32_mod(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
    
    b = bytes32_mod(b_Q255, b_one);
    ASSERT_BYTES32_UINT(b, 0);

    b = BYTES32_UINT(5);
    b = bytes32_mod(b, b);
    ASSERT_BYTES32_UINT(b, 0);
}

void test_sdiv()
{
    printf("\n\t\t\ttest sdiv\t\t");

    printf("\n\t\t\t\ttest sdiv 1\t\t");
    bytes32_t b;
    b = bytes32_sdiv(b_one, b_zero);
    ASSERT_BYTES32_UINT(b, 0);

    printf("\n\t\t\t\ttest sdiv 2\t\t");
    b = bytes32_sdiv(b_Q255, b_one);
    ASSERT_BYTES32_MUTUAL(b, b_Q255);
    
    printf("\n\t\t\t\ttest sdiv 3\t\t");
    b = bytes32_sdiv(b_Q255, b_max);
    ASSERT_BYTES32_MUTUAL(b, b_Q255);
    
    printf("\n\t\t\t\ttest sdiv 4\t\t");
    bytes32_t b1, b2;
    b1 = BYTES32_UINT(2);
    b2 = BYTES32_UINT(5);
    b = bytes32_sdiv(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
    
    printf("\n\t\t\t\ttest sdiv 5\t\t");
    b1 = BYTES32_UINT(2);
    b2 = BYTES32_UINT(5);
    b = bytes32_sdiv(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
    
    printf("\n\t\t\t\ttest sdiv 5\t\t");
    b1 = BYTES32_UINT(2);
    b2 = BYTES32_UINT(0);
    b = bytes32_sdiv(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
    
    printf("\n\t\t\t\ttest sdiv 7\t\t");
    b1 = BYTES32_UINT(0);
    b2 = BYTES32_UINT(5);
    b = bytes32_sdiv(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
}

void test_smod()
{
    printf("\n\t\t\ttest smod\t\t");

    bytes32_t b, b1, b2;
    b1 = BYTES32_UINT(5);
    b2 = BYTES32_UINT(2);
    b = bytes32_smod(b1, b2);
    ASSERT_BYTES32_UINT(b, 1);

    b1 = BYTES32_UINT(5);
    b2 = BYTES32_UINT(2);
    b2 = bytes32_sub(b_zero, b2);
    b = bytes32_smod(b1, b2);
    ASSERT_BYTES32_MUTUAL(b, b_max);

    b1 = BYTES32_UINT(5);
    b1 = bytes32_sub(b_zero, b1);
    b2 = BYTES32_UINT(2);
    b = bytes32_smod(b1, b2);
    ASSERT_BYTES32_MUTUAL(b, b_max);

    b1 = BYTES32_UINT(5);
    b1 = bytes32_sub(b_zero, b1);
    b2 = BYTES32_UINT(2);
    b2 = bytes32_sub(b_zero, b2);
    b = bytes32_smod(b1, b2);
    ASSERT_BYTES32_UINT(b, 1);

    b = bytes32_smod(b_Q255, b_Q255);
    ASSERT_BYTES32_UINT(b, 0);
    
    b1 = BYTES32_UINT(5);
    b = bytes32_smod(b1, b_zero);
    ASSERT_BYTES32_UINT(b, 0);
}

void test_exp()
{
    printf("\n\t\t\ttest exp\t\t");

    bytes32_t b, b1, b2;
    b1 = BYTES32_UINT(2);
    b2 = BYTES32_UINT(3);
    b = bytes32_exp(b1, b2);
    ASSERT_BYTES32_UINT(b, 8);

    b1 = BYTES32_UINT(2);
    b = bytes32_exp(b1, b_one);
    ASSERT_BYTES32_UINT(b, 2);

    b1 = BYTES32_UINT(2);
    b = bytes32_exp(b1, b_zero);
    ASSERT_BYTES32_UINT(b, 1);



    b2 = BYTES32_UINT(2);
    b = bytes32_exp(b_one, b2);
    ASSERT_BYTES32_UINT(b, 1);

    b = bytes32_exp(b_one, b_one);
    ASSERT_BYTES32_UINT(b, 1);

    b = bytes32_exp(b_one, b_zero);
    ASSERT_BYTES32_UINT(b, 1);

    

    b2 = BYTES32_UINT(2);
    b = bytes32_exp(b_zero, b2);
    ASSERT_BYTES32_UINT(b, 0);

    b = bytes32_exp(b_zero, b_one);
    ASSERT_BYTES32_UINT(b, 0);

    b = bytes32_exp(b_zero, b_zero);
    ASSERT_BYTES32_UINT(b, 1);



    b1 = BYTES32_UINT(5);
    b2 = BYTES32_UINT(5);
    b = bytes32_exp(b1, b2);
    ASSERT_BYTES32_UINT(b, 3125);

    

    b1 = BYTES32_UINT(2);
    b2 = BYTES32_UINT(256);
    b = bytes32_exp(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
}

void test_sign_extend()
{
    printf("\n\t\t\ttest sign extend\t\t");

    bytes32_t b, b1, b2;
    b2 = BYTES32_UINT(UINT_MAX);
    b = bytes32_sign_extend(b_zero, b2);
    ASSERT_BYTES32_MUTUAL(b, b_max);
    
    b2 = BYTES32_UINT(0x7fffffff);
    b = bytes32_sign_extend(b_zero, b2);
    ASSERT_BYTES32_UINT(b, 0x7fffffff);
    
    b2 = BYTES32_UINT(0x8000000000000000);
    b = bytes32_sign_extend(b_one, b2);
    ASSERT_BYTES32(b,   \
        UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, \
        UINT_MAX, UINT_MAX, 0x80000000, 0   \
    );

    b1 = BYTES32_UINT(2);
    b2 = BYTES32(   \
        UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, \
        UINT_MAX, 0x7fffffff, UINT_MAX, UINT_MAX  \
    );
    b = bytes32_sign_extend(b1, b2);
    ASSERT_BYTES32(b, 0, 0, 0, 0, 0, 0x7fffffff, UINT_MAX, UINT_MAX);
}

void test_arithmetic()
{
    printf("\n\t\ttest arithmetic");

    test_add();
    test_sub();
    test_mul();
    test_div();
    test_mod();
    test_sdiv();
    test_smod();
    test_exp();
    test_sign_extend();
}



void test_operations()
{
    printf("\n\ttest operation");

    test_bytes32_compare();
    test_shift();
    test_arithmetic();
}

void test_bytes32()
{
    printf("\ntest bytes32");

    test_base();
    test_operations();
}



int main() 
{
    setbuf(stdout, NULL);
    test_bytes32();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
