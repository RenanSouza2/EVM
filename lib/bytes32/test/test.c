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

    for(int i=0; i<SCALAR; i++)
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
    
    for(int i=0; i<SCALAR; i++)
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
    printf("\n\t\t\ttest shl uint");

    bytes32_t b = BYTES32_UINT(1);
    b = bytes32_shl_uint(b, 14);
    ASSERT_BYTES32_UINT(b, 0x4000);

    b = BYTES32_UINT(1);
    b = bytes32_shl_uint(b, 0);
    ASSERT_BYTES32_UINT(b, 1);
    
    b = BYTES32_UINT(2);
    b = bytes32_shl_uint(b, 31);
    ASSERT_BYTES32_UINT(b, 0x100000000);

    b = BYTES32_UINT(0x11111111);
    b = bytes32_shl_uint(b, 16);
    ASSERT_BYTES32_UINT(b, 0x111111110000);
    
    b = BYTES32_UINT(1);
    b = bytes32_shl_uint(b, 48);
    ASSERT_BYTES32_UINT(b, 0x1000000000000);

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
    ASSERT_BYTES32(b, 0x80000000, 0, 0, 0, 0, 0, 0, 0);
}

void test_shr_uint()
{
    printf("\n\t\t\ttest shr uint");

    bytes32_t b = BYTES32(UINT_MAX, 0, 0, 0, 0, 0, 0, 0);
    b = bytes32_shr_uint(b, 80);
    ASSERT_BYTES32(b, 0, 0, 0xFFFF, 0xFFFF0000, 0, 0, 0, 0);

    b = BYTES32_UINT(1);
    b = bytes32_shr_uint(b, 1);
    ASSERT_BYTES32_UINT(b, 0);

    b = BYTES32_UINT(1);
    b = bytes32_shr_uint(b, 0);
    ASSERT_BYTES32_UINT(b, 1);

    b = BYTES32_UINT(0x80000000);
    b = bytes32_shr_uint(b, 16);
    ASSERT_BYTES32_UINT(b, 0x8000);
    
    b = BYTES32_UINT(0x80000000);
    b = bytes32_shr_uint(b, 31);
    ASSERT_BYTES32_UINT(b, 1);
    
    b =  BYTES32(UINT_MAX, 0, 0, 0, 0, 0, 0, 0);
    b = bytes32_shl_uint(b, 256);
    ASSERT_BYTES32_UINT(b, 0);
    
    b =  BYTES32(UINT_MAX, 0, 0, 0, 0, 0, 0, 0);
    b = bytes32_shl_uint(b, 257);
    ASSERT_BYTES32_UINT(b, 0);
}

void test_base_operations()
{
    printf("\n\t\ttest base operations");

    test_add_uint();
    test_shl_uint();
    test_shr_uint();
}

void test_base()
{
    printf("\n\ttest base");

    test_base_compare();
    test_base_operations();
}



void test_is_zero()
{
    printf("\n\t\t\ttest is zero");

    bytes32_t b_in, b_out;
    b_in = BYTES32_UINT(0);
    b_out = bytes32_is_zero(b_in);
    ASSERT_BYTES32_UINT(b_out, 1);

    for(int i=0; i<SCALAR; i++)
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
    ASSERT_BYTES32(b, 0x80000000, 0, 0, 0, 0, 0, 0, 0);
    
    b1 = BYTES32_UINT(1);
    b2 = BYTES32_UINT(256);
    b  = bytes32_shl(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
}

void test_shr()
{
    printf("\n\t\t\ttest shr");

    bytes32_t b, b1, b2;
    b1 = BYTES32(0x80000000, 0, 0, 0, 0, 0, 0, 0);
    b2 = BYTES32_UINT(255);
    b  = bytes32_shr(b1, b2);
    ASSERT_BYTES32_UINT(b, 1);
    
    b1 = BYTES32(0x80000000, 0, 0, 0, 0, 0, 0, 0);
    b2 = BYTES32_UINT(256);
    b  = bytes32_shr(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
}

void test_shift()
{
    printf("\n\t\ttest shift");

    test_shl();
    test_shr();
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
    printf("\n\t\t\ttest div");

    bytes32_t b, b1, b2;
    b1 = BYTES32_UINT(108);
    b2 = BYTES32_UINT(5);
    b = bytes32_div(b1, b2);
    ASSERT_BYTES32_UINT(b, 21);
    
    b1 = BYTES32_UINT(80);
    b2 = BYTES32_UINT(5);
    b = bytes32_div(b1, b2);
    ASSERT_BYTES32_UINT(b, 16);
    
    b1 = BYTES32_UINT(2);
    b2 = BYTES32_UINT(5);
    b = bytes32_div(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
    
    b1 = BYTES32_UINT(2);
    b2 = BYTES32_UINT(0);
    b = bytes32_div(b1, b2);
    ASSERT_BYTES32_UINT(b, 0);
}

void test_mod()
{
    printf("\n\t\t\ttest mod");

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
}

void test_arithmetic()
{
    printf("\n\t\ttest arithmetic");

    test_add();
    test_sub();
    test_mul();
    test_div();
    test_mod();
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



void delete()
{
    uint a, b;
    a = 1;
    b = 2;

    luint c;
    ((uint*)&c)[0] = a;
    ((uint*)&c)[1] = b;
    printf("\nc: %lx", c);

    luint d = c << 5;
    printf("\nd: %lx", d);
}

int main() 
{
    setbuf(stdout, NULL);
    // test_bytes32();
    delete();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
