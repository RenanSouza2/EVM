#include <stdio.h>
#include <limits.h>
#include <assert.h>

#include "../debug.h"

void test_cmp_uint(int scalar)
{
    printf("\n\t\t\ttest cmp uint\t\t");

    uint b[scalar];
    BYTES_N_RESET(scalar, b);
    int res = bytes_n_cmp_uint(scalar, b, 0);
    assert(res == 0);

    BYTES_N_RESET(scalar, b);
    res = bytes_n_cmp_uint(scalar, b,  1);
    assert(res < 0);

    BYTES_N_SET_UINT(scalar, b, 1);
    res = bytes_n_cmp_uint(scalar, b,  0);
    assert(res > 0);

    BYTES_N_SET_UINT(scalar, b, 1);
    res = bytes_n_cmp_uint(scalar, b,  1);
    assert(res == 0);

    BYTES_N_SET_UINT(scalar, b, 1);
    res = bytes_n_cmp_uint(scalar, b,  2);
    assert(res < 0);

    BYTES_N_SET_UINT(scalar, b, 1);
    res = bytes_n_cmp_uint(scalar, b,  UINT_MAX);
    assert(res < 0);

    BYTES_N_SET_UINT(scalar, b, UINT_MAX);
    res = bytes_n_cmp_uint(scalar, b,  0);
    assert(res > 0);

    BYTES_N_SET_UINT(scalar, b, UINT_MAX);
    res = bytes_n_cmp_uint(scalar, b,  UINT_MAX);
    assert(res == 0);
}

void test_add_uint(int scalar)
{
    printf("\n\t\t\ttest add uint\t\t");

    uint b[scalar];
    BYTES_N_RESET(scalar, b);
    bytes_n_add_uint(scalar, b, 0, 0);
    ASSERT_BYTES_N(scalar, b, 0);
    
    BYTES_N_SET_UINT(scalar, b, 2);
    bytes_n_add_uint(scalar, b, 3, 0);
    ASSERT_BYTES_N(scalar, b, 5);

    BYTES_N_SET_UINT(scalar, b, UINT_MAX);
    bytes_n_add_uint(scalar, b, UINT_MAX, 0);
    ASSERT_BYTES_N(scalar, b, UINT_MAX - 1, 1);

    BYTES_N_SET_POS(scalar, b, UINT_MAX, scalar - 1);
    bytes_n_add_uint(scalar, b, 1, scalar-1);
    ASSERT_BYTES_N(scalar, b, 0);
}

void test_shl_uint(int scalar)
{
    printf("\n\t\t\ttest shl uint\t\t");

    uint b[scalar];
    BYTES_N_SET_UINT(scalar, b, 1);
    bytes_n_shl_uint(scalar, b, 0);
    ASSERT_BYTES_N(scalar, b, 1);

    BYTES_N_SET_UINT(scalar, b, 1);
    bytes_n_shl_uint(scalar, b, 1);
    ASSERT_BYTES_N(scalar, b, 2);

    BYTES_N_SET_UINT(scalar, b, 1);
    bytes_n_shl_uint(scalar, b, 32);
    ASSERT_BYTES_N(scalar, b, 0, 1);

    BYTES_N_SET_UINT(scalar, b, 0x11111111, 0x22222222);
    bytes_n_shl_uint(scalar, b, 16);
    ASSERT_BYTES_N(scalar, b, 0x11110000, 0x22221111, 0x00002222);

    BYTES_N_SET_POS(scalar, b, UINT_MAX, scalar-2);
    bytes_n_shl_uint(scalar, b, 48);
    ASSERT_BYTES_N_LAST(scalar, b, 0xFFFF0000);

    BYTES_N_SET_MAX(scalar, b);
    bytes_n_shl_uint(scalar, b, 1000);
    ASSERT_BYTES_N(scalar, b, 0);
}

void test_shr_uint(int scalar)
{
    printf("\n\t\t\ttest shr uint\t\t");

    const uint last = 0x80000000;

    uint b[scalar];
    BYTES_N_SET_UINT(scalar, b, last);
    bytes_n_shr_uint(scalar, b, 0);
    ASSERT_BYTES_N(scalar, b, last);

    BYTES_N_SET_UINT(scalar, b, last);
    bytes_n_shr_uint(scalar, b, 1);
    ASSERT_BYTES_N(scalar, b, 0x40000000);

    BYTES_N_SET_UINT(scalar, b, 0, 1);
    bytes_n_shr_uint(scalar, b, 32);
    ASSERT_BYTES_N(scalar, b, 1);

    BYTES_N_SET_UINT(scalar, b, 0x11111111, 0x22222222);
    bytes_n_shr_uint(scalar, b, 16);
    ASSERT_BYTES_N(scalar, b, 0x22221111, 0x00002222);

    BYTES_N_SET_MAX(scalar, b);
    bytes_n_shr_uint(scalar, b, 1000);
    ASSERT_BYTES_N(scalar, b, 0);
}



void test_uint(int scalar)
{
    printf("\n\t\ttest uint\t\t");

    test_cmp_uint(scalar);
    test_add_uint(scalar);
    test_shl_uint(scalar);
    test_shr_uint(scalar);
}



void test_suit(int scalar)
{
    printf("\n\ttest suit %d", scalar << 2);

    test_uint(scalar);
}

void test_bytes_n()
{
    printf("\ntest example library\t\t");

    test_suit(8);
    // test_suit(16);
    // test_suit(20);
}

void test_is_zero_bool(int scalar)
{
    printf("\n\t\t\ttest is zero bool\t\t");

    uint b[scalar];
    BYTES_N_RESET(scalar, b);
    bool is_zero = bytes_n_is_zero(scalar, b);
    assert(is_zero == true);

    for(int i=0; i<scalar; i++)
    {
        BYTES_N_RESET(scalar, b);
        b[i] = 1;
        is_zero = bytes_n_is_zero(scalar, b);
        assert(is_zero == false);
    }

    memset(b, 255, scalar << 2);
    is_zero = bytes_n_is_zero(scalar, b);
    assert(is_zero == false);
}



int main() 
{
    setbuf(stdout, NULL);
    test_bytes_n();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
