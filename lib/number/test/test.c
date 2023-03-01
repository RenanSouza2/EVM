#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include "../debug.h"
#include "../../bytes32/debug.h"

#include <stdlib.h>



void test_copy()
{
    printf("\n\ttest copy\t\t");

    number_p n1 = number_copy(NULL);
    assert(n1 == NULL);

    n1 = NUMBER_UINT(1);
    number_p n2 = number_copy(n1);
    assert(n2 != n1);
    ASSERT_NUMBER_UINT(n2, 1);

    n1 = NUMBER_UINT(1);
    n1->next = NUMBER_UINT(2);
    n2 = number_copy(n1);
    ASSERT_NUMBER_UINT(n2, 1);
    assert(n2->next != n1->next);
    ASSERT_NUMBER_UINT(n2->next, 2);
}

void test_add_bytes32()
{
    printf("\n\ttest add bytes32\t\t");

    printf("\n\t\ttest add bytes32 1\t\t");
    bytes32_t b = BYTES32_UINT(1);
    number_p n = number_add_bytes32(NULL, b, 0);
    ASSERT_NUMBER_UINT(n, 1);

    printf("\n\t\ttest add bytes32 2\t\t");
    b = BYTES32_UINT(3);
    n = number_create_bytes32(b_max);
    n = number_add_bytes32(n, b, 0);
    assert(number_uint_mult(n, 2, 2, 1));

    printf("\n\t\ttest add bytes32 3\t\t");
    n = NUMBER_UINT(1);
    b = BYTES32_UINT(2);
    n = number_add_bytes32(n, b, 1);
    assert(number_uint_mult(n, 2, 1, 2));

    printf("\n\t\ttest add bytes32 4\t\t");
    n = NUMBER_UINT_OFF(2, 2);
    b = BYTES32_UINT(1);
    n = number_add_bytes32(n, b, 1);
    assert(number_uint_mult(n, 3, 0, 1, 2));
    
    n = number_create_bytes32_mult(2, b_zero, b_max);
    n = number_add_bytes32(n, b_one, 1);
    assert(number_uint_mult(n, 3, 0, 0, 1));
}

void test_is_zero()
{
    printf("\n\ttest is zero\t\t");

    bool res = number_is_zero(NULL);
    assert(res == true);

    number_p n = number_create_null();
    res = number_is_zero(n);
    assert(res == true);

    n = NUMBER_UINT(1);
    res = number_is_zero(n);
    assert(res == false);

    n = number_create_uint_mult(2, 1, 0);
    res = number_is_zero(n);
    assert(res == false);
}

void test_number_cmp()
{
    printf("\n\ttest cmp\t\t");

    int res = number_cmp(NULL, NULL);
    assert(res == 0);

    number_p n1, n2;
    n1 = NUMBER_UINT(1);
    n2 = NUMBER_UINT(2);
    res = number_cmp(n1, n2);
    assert(res < 0);
    
    n1 = NUMBER_UINT(1);
    res = number_cmp(n1, NULL);
    assert(res > 0);
    
    n1 = NUMBER_UINT(1);
    res = number_cmp(NULL, n1);
    assert(res < 0);

    n1 = NUMBER_UINT(1);
    res = number_cmp(n1, n1);
    assert(res == 0);

    n1 = number_create_null();
    res = number_cmp(NULL, n1);
    assert(res == 0);

    n1 = number_create_uint_mult(2, 1, 2);
    n2 = number_create_uint_mult(2, 2, 1);
    res = number_cmp(n1, n2);
    assert(res > 0);
    n1 = number_create_uint_mult(2, 2, 1);
    n2 = number_create_uint_mult(2, 1, 1);
    res = number_cmp(n1, n2);
    assert(res > 0);
}




void test_add()
{
    printf("\n\ttest add\t\t");

    number_p n = number_add(NULL, NULL);
    assert(n == NULL);

    n = NUMBER_UINT(1);
    n = number_add(n, NULL);
    ASSERT_NUMBER_UINT(n, 1);

    n = NUMBER_UINT(1);
    n = number_add(NULL, n);
    ASSERT_NUMBER_UINT(n, 1);

    n = NUMBER_UINT(1);
    n = number_add(n, n);
    ASSERT_NUMBER_UINT(n, 2);

    number_p n1, n2;
    n1 = NUMBER_UINT_OFF(1, 1);
    n2 = NUMBER_UINT(2);
    n = number_add(n1, n2);
    assert(number_uint_mult(n, 2, 2, 1));
    
    n1 = NUMBER_UINT(1);
    n2 = NUMBER_UINT_OFF(2, 1);
    n = number_add(n1, n2);
    assert(number_uint_mult(n, 2, 1, 2));
}

void test_mul()
{
    printf("\n\ttest mul\t\t");

    number_p n = number_mul(NULL, NULL);
    assert(n == NULL);

    n = NUMBER_UINT(1);
    n = number_mul(n, NULL);
    assert(n == NULL);

    n = NUMBER_UINT(1);
    n = number_mul(NULL, n);
    assert(n == NULL);

    n = NUMBER_UINT(1);
    n = number_mul(n, n);
    ASSERT_NUMBER_UINT(n, 1);

    number_p n1, n2;
    n1 = number_create_uint_mult(2, 1, 2);
    n2 = number_create_uint_mult(3, 3, 4, 5);
    n = number_mul(n1, n2);
    assert(number_uint_mult(n, 4, 3, 10, 13, 10));

    n1 = number_create_bytes32_mult(2, b_max, b_max);
    n2 = number_create_bytes32_mult(2, b_max, b_max);
    n = number_mul(n1, n2);
    assert(number_bytes32_mult(n, 4, b_one, b_zero, b_max_1, b_max));
}



void test_number()
{
    printf("\ntest number library\t\t");

    test_copy();
    test_add_bytes32();
    test_is_zero();
    test_number_cmp();

    test_add();
    test_mul();
}



int main() 
{
    setbuf(stdout, NULL);
    test_number();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
