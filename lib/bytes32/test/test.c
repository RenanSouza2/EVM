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
    b_exp = bytes32_set( \
        VALUE7, VALUE6, VALUE5, VALUE4, VALUE3, VALUE2, VALUE1, VALUE0 \
    ); \
    ASSERT_BYTES32_MUTUAL(BYTES, b_exp); \
}

#define ASSERT_BYTES32_UINT(BYTES, UINT) \
{ \
    bytes32_t b_exp; \
    b_exp = bytes32_convert(UINT); \
    ASSERT_BYTES32_MUTUAL(BYTES, b_exp); \
}



void test_convert()
{
    printf("\n\ttest convert");

    luint lu = 0x100000002;
    bytes32_t b_res = bytes32_convert(lu);
    ASSERT_BYTES32(b_res, 0, 0, 0, 0, 0, 0, 1, 2);
}



void test_add_uint()
{
    printf("\n\ttest add uint");

    bytes32_t b = bytes32_set(7, 6, 5, 4, 3, 2, 1, 0);
    
    for(int i=0; i<SCALAR; i++)
    {
        b = bytes32_add_uint(b, 1, i);
        assert(b.v[i] == i+1);
    }

    b = bytes32_convert(UINT_MAX);
    b = bytes32_add_uint(b, 1, 0);
    assert(b.v[1] == 1);
    assert(b.v[0] == 0);

    
    b = bytes32_set(0, 0, 0, 0, 0, 0, UINT_MAX, UINT_MAX);
    b = bytes32_add_uint(b, 1, 0);
    assert(b.v[2] == 1);
    assert(b.v[1] == 0);
    assert(b.v[0] == 0);

    b = bytes32_convert(0);
    b = bytes32_add_uint(b, 1, 8);
    ASSERT_BYTES32_UINT(b, 0);
}

void test_add()
{
    printf("\n\ttest add");

    bytes32_t b_res, b1, b2;
    b1 = bytes32_set(7, 6, 5, 4, 3, 2, 1, 0);
    b2 = bytes32_set(0, 1, 2, 3, 4, 5, 6, 7);
    b_res = bytes32_add(b1, b2);
    ASSERT_BYTES32(b_res, 7, 7, 7, 7, 7, 7, 7, 7);
    
    b1 = bytes32_set(7, 6, 5, 4, 3, 2, 1, 0);
    b2 = bytes32_set(0, 1, 2, 3, 4, 5, 6, 7);
    b_res = bytes32_add(b1, b2);

    b1 = bytes32_set(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    b2 = bytes32_set(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    b_res = bytes32_add(b1, b2);
    ASSERT_BYTES32(b_res, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX - 1);
}

void test_mul()
{
    printf("\n\ttest mul");

    bytes32_t b_res, b1, b2;
    b1 = bytes32_convert(2);
    b2 = bytes32_convert(3);
    b_res = bytes32_mul(b1, b2);
    ASSERT_BYTES32_UINT(b_res, 6);

    b1 = bytes32_set(1, 1, 1, 1, 1, 1, 1, 1);
    b2 = bytes32_set(1, 1, 1, 1, 1, 1, 1, 1);
    b_res = bytes32_mul(b1, b2);
    ASSERT_BYTES32(b_res, 8, 7, 6, 5, 4, 3, 2, 1);

    b1 = bytes32_set(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    b2 = bytes32_set(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    b_res = bytes32_mul(b1, b2);
    ASSERT_BYTES32_UINT(b_res, 1);
}



void test_bytes32()
{
    printf("\ntest bytes32");

    test_convert();
    test_add_uint();
    test_add();
    test_mul();
}



int main() {
    setbuf(stdout, NULL);
    test_bytes32();
    printf("\n\n\t\tTest successful\n\n");
    return 0;
}
