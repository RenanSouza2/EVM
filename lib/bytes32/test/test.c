#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <string.h>

#include "../code.c"

#define ASSERT_BYTES32_MUTUAL(VALUE1, VALUE2) \
    assert(memcmp(VALUE1, VALUE2, 32) == 0)

#define ASSERT_BYTES32(VALUE, \
    VALUE7, VALUE6, VALUE5, VALUE4, VALUE3, VALUE2, VALUE1, VALUE0 \
) \
{ \
    bytes32 expected; \
    bytes32_set(expected, \
        VALUE7, VALUE6, VALUE5, VALUE4, VALUE3, VALUE2, VALUE1, VALUE0 \
    ); \
    ASSERT_BYTES32_MUTUAL(VALUE, expected); \
}

#define ASSERT_BYTES32_UINT(VALUE, UINT) \
{ \
    bytes32 res; \
    bytes32_convert(res, UINT); \
    ASSERT_BYTES32_MUTUAL(VALUE, res); \
}

#define BYTES32_DISPLAY(STRING, VALUE) \
    printf("%s", STRING);bytes32_display(VALUE);printf("\n")

void test_convert()
{
    printf("\n\ttest convert");

    luint value = 0x100000002;

    bytes32 res;
    bytes32_convert(res, value);
    ASSERT_BYTES32(res, 0, 0, 0, 0, 0, 0, 1, 2);
}

void test_add_uint()
{
    printf("\n\ttest add uint");

    bytes32 value;
    bytes32_set(value, 7, 6, 5, 4, 3, 2, 1, 0);
    
    for(int i=0; i<SCALAR; i++)
    {
        bytes32_add_uint(value, 1, i);
        assert(value[i] == i+1);
    }

    bytes32_convert(value, UINT_MAX);
    bytes32_add_uint(value, 1, 0);
    assert(value[1] == 1);
    assert(value[0] == 0);

    
    bytes32_set(value, 0, 0, 0, 0, 0, 0, UINT_MAX, UINT_MAX);
    bytes32_add_uint(value, 1, 0);
    assert(value[2] == 1);
    assert(value[1] == 0);
    assert(value[0] == 0);

    bytes32_convert(value, 0);
    bytes32_add_uint(value, 1, 8);
    ASSERT_BYTES32_UINT(value, 0);
}

void test_add()
{
    printf("\n\ttest add");

    bytes32 value, value1, value2;
    bytes32_set(value1, 7, 6, 5, 4, 3, 2, 1, 0);
    bytes32_set(value2, 0, 1, 2, 3, 4, 5, 6, 7);
    bytes32_add(value, value1, value2);
    ASSERT_BYTES32(value, 7, 7, 7, 7, 7, 7, 7, 7);
    
    bytes32_set(value1, 7, 6, 5, 4, 3, 2, 1, 0);
    bytes32_set(value2, 0, 1, 2, 3, 4, 5, 6, 7);
    bytes32_add(value, value1, value2);

    bytes32_set(value1, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    bytes32_set(value2, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    bytes32_add(value, value1, value2);
    ASSERT_BYTES32(value, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX - 1);
}

void test_mul()
{
    printf("\n\ttest mul");

    bytes32 value, value1, value2;
    bytes32_convert(value1, 2);
    bytes32_convert(value2, 3);
    bytes32_mul(value, value1, value2);
    ASSERT_BYTES32_UINT(value, 6);

    bytes32_set(value1, 1, 1, 1, 1, 1, 1, 1, 1);
    bytes32_set(value2, 1, 1, 1, 1, 1, 1, 1, 1);
    bytes32_mul(value, value1, value2);
    ASSERT_BYTES32(value, 8, 7, 6, 5, 4, 3, 2, 1);

    bytes32_set(value1, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    bytes32_set(value2, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX);
    bytes32_mul(value, value1, value2);
    ASSERT_BYTES32_UINT(value, 1);
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
