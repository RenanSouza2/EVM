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

// void bytes_n_add_uint(int scalar, uint b[scalar], uint u, int i)
void test_add_uint(int scalar)
{
    printf("\n\t\t\ttest add uint\t\t");

    uint b[scalar];
    BYTES_N_RESET(scalar, b);
    bytes_n_add_uint(scalar, b, 0, 0);

}

void test_uint(int scalar)
{
    printf("\n\t\ttest uint\t\t");

    test_cmp_uint(scalar);
    test_add_uint(scalar);
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
