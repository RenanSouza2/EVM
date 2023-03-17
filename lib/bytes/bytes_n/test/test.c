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



void test_is_zero(int scalar)
{
    printf("\n\t\t\ttest is zero\t\t");

    uint b[scalar];
    BYTES_N_RESET(scalar, b);
    bool res = bytes_n_is_zero(scalar, b);
    assert(res);

    for(int i=0; i<scalar; i++)
    {
        BYTES_N_SET_POS(scalar, b, 1, i);
        res = bytes_n_is_zero(scalar, b);
        assert(res == false);
    }
}

void test_cmp(int scalar)
{
    printf("\n\t\t\ttest cmp\t\t");

    uint b1[scalar], b2[scalar];
    BYTES_N_RESET(scalar, b1);
    BYTES_N_RESET(scalar, b2);
    int res = bytes_n_cmp(scalar, b1, b2);
    assert(res == 0);

    BYTES_N_RESET(scalar, b1);
    BYTES_N_SET_UINT(scalar, b2, 1);
    res = bytes_n_cmp(scalar, b1, b2);
    assert(res < 0);

    BYTES_N_SET_UINT(scalar, b1, 1);
    BYTES_N_RESET(scalar, b2);
    res = bytes_n_cmp(scalar, b1, b2);
    assert(res > 0);

    BYTES_N_SET_UINT(scalar, b1, 1);
    BYTES_N_SET_UINT(scalar, b2, 1);
    res = bytes_n_cmp(scalar, b1, b2);
    assert(res == 0);

    BYTES_N_SET_UINT(scalar, b1, 0, 1);
    BYTES_N_SET_UINT(scalar, b2, 1);
    res = bytes_n_cmp(scalar, b1, b2);
    assert(res > 0);

    BYTES_N_SET_UINT(scalar, b1, 1);
    BYTES_N_SET_UINT(scalar, b2, 0, 1);
    res = bytes_n_cmp(scalar, b1, b2);
    assert(res < 0);
}

void test_cmps(int scalar)
{
    printf("\n\t\ttest cmps\t\t");
    
    test_is_zero(scalar);
    test_cmp(scalar);
}



void test_not(int scalar)
{
    printf("\n\t\t\ttest not\t\t");

    uint b[scalar];
    BYTES_N_RESET(scalar, b);
    bytes_n_not(scalar, b);
    {
        uint b_res[scalar];
        BYTES_N_SET_MAX(scalar, b_res);
        ASSERT_BYTES_N_MUTUAL(scalar, b, b_res);
    }
}

void test_minus(int scalar)
{
    printf("\n\t\t\ttest minus\t\t");

    uint b[scalar];
    BYTES_N_RESET(scalar, b);
    bytes_n_minus(scalar, b);
    ASSERT_BYTES_N(scalar, b, 0);
    
    BYTES_N_SET_UINT(scalar, b, 1);
    bytes_n_minus(scalar, b);
    {
        uint b_res[scalar];
        BYTES_N_SET_MAX(scalar, b_res);
        ASSERT_BYTES_N_MUTUAL(scalar, b, b_res);
    }
    
    
    BYTES_N_SET_UINT(scalar, b, 2);
    bytes_n_minus(scalar, b);
    {
        uint b_res[scalar];
        BYTES_N_SET_MAX(scalar, b_res);
        b_res[0] = UINT_MAX - 1;
        ASSERT_BYTES_N_MUTUAL(scalar, b, b_res);
    }

    BYTES_N_SET_POS(scalar, b, 0x80000000, scalar - 1);
    bytes_n_minus(scalar, b);
    {
        uint b_res[scalar];
        BYTES_N_SET_POS(scalar, b_res, 0x80000000, scalar - 1);
        ASSERT_BYTES_N_MUTUAL(scalar, b, b_res);
    }
}

void test_unary(int scalar)
{
    printf("\n\t\ttest unary\t\t");
    
    test_not(scalar);
    test_minus(scalar);
}



void test_add(int scalar)
{
    printf("\n\t\t\ttest add\t\t");

    uint b1[scalar], b2[scalar];
    BYTES_N_SET_UINT(scalar, b1, 1, 2);
    BYTES_N_SET_UINT(scalar, b2, 3, 4);
    bytes_n_add(scalar, b1, b2);
    ASSERT_BYTES_N(scalar, b1, 4, 6);
    
    BYTES_N_SET_UINT(scalar, b1, UINT_MAX, UINT_MAX);
    BYTES_N_SET_UINT(scalar, b2, 1, UINT_MAX);
    bytes_n_add(scalar, b1, b2);
    ASSERT_BYTES_N(scalar, b1, 0, UINT_MAX, 1);
    
    BYTES_N_SET_MAX(scalar, b1);
    BYTES_N_SET_UINT(scalar, b2, 1);
    bytes_n_add(scalar, b1, b2);
    ASSERT_BYTES_N(scalar, b1, 0);
}


void test_sub(int scalar)
{
    printf("\n\t\t\ttest sub\t\t");

    uint b1[scalar], b2[scalar];
    BYTES_N_SET_UINT(scalar, b1, 3, 4);
    BYTES_N_SET_UINT(scalar, b2, 1, 2);
    bytes_n_sub(scalar, b1, b2);
    ASSERT_BYTES_N(scalar, b1, 2, 2);
    
    BYTES_N_SET_UINT(scalar, b1, UINT_MAX, UINT_MAX);
    BYTES_N_SET_UINT(scalar, b2, 1, UINT_MAX);
    bytes_n_sub(scalar, b1, b2);
    ASSERT_BYTES_N(scalar, b1, UINT_MAX - 1);
    
    BYTES_N_SET_UINT(scalar, b1, 3);
    BYTES_N_SET_UINT(scalar, b2, 5);
    bytes_n_sub(scalar, b1, b2);
    {
        BYTES_N_SET_MAX(scalar, b2);
        b2[0] = UINT_MAX - 1;
        ASSERT_BYTES_N_MUTUAL(scalar, b1, b2);
    }
}

void test_binary(int scalar)
{
    printf("\n\t\ttest binary\t\t");
    
    test_add(scalar);
    test_sub(scalar);
}



void test_suit(int scalar)
{
    printf("\n\ttest suit %d", scalar << 2);

    test_uint(scalar);
    test_cmps(scalar);
    test_unary(scalar);
    test_binary(scalar);
}

void test_bytes_n()
{
    printf("\ntest example library\t\t");

    test_suit(8);
    // test_suit(16);
    // test_suit(20);
}



int main() 
{
    setbuf(stdout, NULL);
    test_bytes_n();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
