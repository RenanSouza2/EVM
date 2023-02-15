#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

#include "../code.c"

#define ASSERT_BYTES32_MUTUAL(BYTES1, BYTES2) \
    assert(memcmp(BYTES1.v, BYTES2.v, 32) == 0)

#define ASSERT_BYTES32(BYTES, \
    VALUE7, VALUE6, VALUE5, VALUE4, VALUE3, VALUE2, VALUE1, VALUE0 \
) \
{ \
    bytes32_t b_exp; \
    b_exp = BYTES32( \
        VALUE7, VALUE6, VALUE5, VALUE4, VALUE3, VALUE2, VALUE1, VALUE0 \
    ); \
    ASSERT_BYTES32_MUTUAL(BYTES, b_exp); \
}

#define ASSERT_BYTES32_UINT(BYTES, UINT) \
{ \
    bytes32_t b_exp; \
    b_exp = BYTES32_UINT(UINT); \
    ASSERT_BYTES32_MUTUAL(BYTES, b_exp); \
}



void test_convert()
{
    printf("\n\t\ttest convert");

    luint lu = 0x100000002;
    bytes32_t b_res = BYTES32_UINT(lu);
    ASSERT_BYTES32(b_res, 0, 0, 0, 0, 0, 0, 1, 2);
}

void test_add_uint()
{
    printf("\n\t\ttest add uint");

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

// void test_revert()
// {
//     printf("\n\t\ttest revert");
//
//     bytes32_t b_in = BYTES32(7, 6, 5, 4, 3, 2, 1, 0);
//     bytes32_t b_out = bytes32_revert(b_in);
//     ASSERT_BYTES32(b_out, 0, 1, 2, 3, 4, 5, 6, 7);
// }

void test_is_zero_bool()
{
    printf("\n\t\ttest is zero bool");

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

    b = BYTES32(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    is_zero = bytes32_is_zero_bool(b);
    assert(is_zero == false);
}

void test_cmp()
{
    printf("\n\t\ttest cmp");

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
    res = bytes32_cmp(b1, b_zero);
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

void test_shl_uint()
{
    printf("\n\t\ttest shl uint");

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
    printf("\n\t\ttest shr uint");

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

void test_tools()
{
    printf("\n\ttest tools");

    test_convert();
    test_add_uint();
    // test_revert();
    test_is_zero_bool();
    test_cmp();
    test_shl_uint();
    test_shr_uint();
}



void test_is_zero()
{
    printf("\n\t\ttest is zero");

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

void test_add()
{
    printf("\n\t\ttest add");

    bytes32_t b_res, b1, b2;
    b1 = BYTES32(7, 6, 5, 4, 3, 2, 1, 0);
    b2 = BYTES32(0, 1, 2, 3, 4, 5, 6, 7);
    b_res = bytes32_add(b1, b2);
    ASSERT_BYTES32(b_res, 7, 7, 7, 7, 7, 7, 7, 7);
    
    b1 = BYTES32(7, 6, 5, 4, 3, 2, 1, 0);
    b2 = BYTES32(0, 1, 2, 3, 4, 5, 6, 7);
    b_res = bytes32_add(b1, b2);

    b1 = BYTES32(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    b2 = BYTES32(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    b_res = bytes32_add(b1, b2);
    ASSERT_BYTES32(b_res, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX - 1);
}

void test_mul()
{
    printf("\n\t\ttest mul");

    bytes32_t b_res, b1, b2;
    b1 = BYTES32_UINT(2);
    b2 = BYTES32_UINT(3);
    b_res = bytes32_mul(b1, b2);
    ASSERT_BYTES32_UINT(b_res, 6);

    b1 = BYTES32(1, 1, 1, 1, 1, 1, 1, 1);
    b2 = BYTES32(1, 1, 1, 1, 1, 1, 1, 1);
    b_res = bytes32_mul(b1, b2);
    ASSERT_BYTES32(b_res, 8, 7, 6, 5, 4, 3, 2, 1);

    b1 = BYTES32(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    b2 = BYTES32(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    b_res = bytes32_mul(b1, b2);
    ASSERT_BYTES32_UINT(b_res, 1);
}

void test_sub()
{
    printf("\n\t\ttest sub");

    bytes32_t b_res, b1, b2;
    b1 = BYTES32_UINT(5);
    b2 = BYTES32_UINT(3);
    b_res = bytes32_sub(b1, b2);
    ASSERT_BYTES32_UINT(b_res, 2);
    
    b1 = BYTES32_UINT(0);
    b2 = BYTES32_UINT(1);
    b_res = bytes32_sub(b1, b2);
    ASSERT_BYTES32(b_res, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);

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

    b1 = BYTES32(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    b2 = BYTES32(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    b_res = bytes32_sub(b1, b2);
    ASSERT_BYTES32_UINT(b_res, 0);
}

void test_shl()
{
    printf("\n\t\ttest shl");

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
    printf("\n\t\ttest shr");

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

void test_div()
{
    printf("\n\t\ttest div");

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
    printf("\n\t\ttest mod");

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

void test_operations()
{
    printf("\n\n\ttest operation");

    test_is_zero();
    test_add();
    test_sub();
    test_mul();
    test_shl();
    test_shr();
    test_div();
    test_mod();
}



void test_bytes32()
{
    printf("\ntest bytes32");

    test_tools();
    test_operations();
}



int main() {
    setbuf(stdout, NULL);
    test_bytes32();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
