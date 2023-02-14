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
    b_exp = BYTES32_SET( \
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

    bytes32_t b = BYTES32_SET(7, 6, 5, 4, 3, 2, 1, 0);
    
    for(int i=0; i<SCALAR; i++)
    {
        b = bytes32_add_uint(b, 1, i);
        assert(b.v[i] == i+1);
    }

    b = BYTES32_UINT(UINT_MAX);
    b = bytes32_add_uint(b, 1, 0);
    assert(b.v[1] == 1);
    assert(b.v[0] == 0);

    
    b = BYTES32_SET(0, 0, 0, 0, 0, 0, UINT_MAX, UINT_MAX);
    b = bytes32_add_uint(b, 1, 0);
    assert(b.v[2] == 1);
    assert(b.v[1] == 0);
    assert(b.v[0] == 0);

    b = BYTES32_UINT(0);
    b = bytes32_add_uint(b, 1, 8);
    ASSERT_BYTES32_UINT(b, 0);
}

void test_revert()
{
    printf("\n\t\ttest revert");

    bytes32_t b_in = BYTES32_SET(7, 6, 5, 4, 3, 2, 1, 0);
    bytes32_t b_out = bytes32_revert(b_in);
    ASSERT_BYTES32(b_out, 0, 1, 2, 3, 4, 5, 6, 7);
}

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

    b = BYTES32_SET(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    is_zero = bytes32_is_zero_bool(b);
    assert(is_zero == false);
}

void test_cmp()
{
    printf("\n\t\ttest cmp");

    bytes32_t b1 = BYTES32_UINT(5);
    int res = bytes32_cmp(b_zero, b1);
    assert(res < 0);
    
    res = bytes32_cmp(b_zero, b_zero);
    assert(res == 0);

    res = bytes32_cmp(b1, b1);
    assert(res == 0);

    res = bytes32_cmp(b1, b_zero);
    assert(res > 0);

    bytes32_t b2 = BYTES32_SET(UINT_MAX, 0, 0, 0, 0, 0, 0, 0);
    res = bytes32_cmp(b1, b2);
    assert(res < 0);

    b2 = BYTES32_SET(UINT_MAX, 0, 0, 0, 0, 0, 0, 0);
    res = bytes32_cmp(b2, b1);
    assert(res > 0);
}

void test_tools()
{
    printf("\n\ttest tools");

    test_convert();
    test_add_uint();
    test_revert();
    test_is_zero_bool();
    test_cmp();
}



void test_add()
{
    printf("\n\t\ttest add");

    bytes32_t b_res, b1, b2;
    b1 = BYTES32_SET(7, 6, 5, 4, 3, 2, 1, 0);
    b2 = BYTES32_SET(0, 1, 2, 3, 4, 5, 6, 7);
    b_res = bytes32_add(b1, b2);
    ASSERT_BYTES32(b_res, 7, 7, 7, 7, 7, 7, 7, 7);
    
    b1 = BYTES32_SET(7, 6, 5, 4, 3, 2, 1, 0);
    b2 = BYTES32_SET(0, 1, 2, 3, 4, 5, 6, 7);
    b_res = bytes32_add(b1, b2);

    b1 = BYTES32_SET(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    b2 = BYTES32_SET(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
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

    b1 = BYTES32_SET(1, 1, 1, 1, 1, 1, 1, 1);
    b2 = BYTES32_SET(1, 1, 1, 1, 1, 1, 1, 1);
    b_res = bytes32_mul(b1, b2);
    ASSERT_BYTES32(b_res, 8, 7, 6, 5, 4, 3, 2, 1);

    b1 = BYTES32_SET(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    b2 = BYTES32_SET(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
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

    b1 = BYTES32_SET(7, 6, 5, 4, 3, 2, 1, 0);
    b2 = BYTES32_SET(0, 1, 2, 3, 4, 5, 6, 7);
    b_res = bytes32_sub(b1, b2);
    ASSERT_BYTES32(b_res, 7, 5, 3, 0, 0xFFFFFFFE, 0xFFFFFFFC, 0xFFFFFFFA, 0xFFFFFFF9);
    
    b1 = BYTES32_SET(7, 6, 5, 4, 3, 2, 1, 0);
    b2 = BYTES32_SET(0, 1, 2, 3, 4, 5, 6, 7);
    b_res = bytes32_sub(b1, b2);

    b1 = BYTES32_SET(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    b2 = BYTES32_SET(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    b_res = bytes32_sub(b1, b2);
    ASSERT_BYTES32_UINT(b_res, 0);
}

void test_operations()
{
    printf("\n\n\ttest operation");

    test_add();
    test_sub();
    test_mul();
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
