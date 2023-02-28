#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include "../debug.h"
#include "../../bytes32/debug.h"



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

void test_add_uint()
{
    printf("\n\ttest add uint\t\t");

    number_p n = number_add_uint(NULL, 0, 0);
    assert(n == NULL);

    n = number_add_uint(NULL, 1, 0);
    ASSERT_NUMBER_UINT(n, 1);

    n = number_add_uint(NULL, 1, SCALAR);
    ASSERT_NUMBER_UINT(n->next, 1);

    n = number_add_uint(NULL, 1, SCALAR);
    n = number_add_uint(n, 1, SCALAR);
    ASSERT_NUMBER_UINT(n->next, 2);

    n = NUMBER_UINT(UINT_MAX);
    n = number_add_uint(n, 3, 0);
    ASSERT_NUMBER_UINT(n, 0x100000002);

    n = number_create_bytes32(b_max);
    n = number_add_uint(n, 3, 0);
    ASSERT_NUMBER_UINT(n, 2);
    ASSERT_NUMBER_UINT(n->next, 1);
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
    ASSERT_NUMBER_UINT(n, 2);
    ASSERT_NUMBER_UINT(n->next, 1);

    printf("\n\t\ttest add bytes32 3\t\t");
    n = NUMBER_UINT(1);
    b = BYTES32_UINT(1);
    n = number_add_bytes32(n, b, 1);
    ASSERT_NUMBER_UINT(n->next, 1);

    printf("\n\t\ttest add bytes32 4\t\t");
    b = BYTES32_UINT(1);
    n = number_create_bytes32_dbg(b, 2);
    n = number_add_bytes32(n, b, 1);
    ASSERT_NUMBER_UINT(n->next, 1);
}

void test_bytes32_mul()
{
    printf("\n\ttest number bytes32 mul\t\t");

    number_p n = number_bytes32_mul(b_zero, b_zero);
    assert(n == NULL);

    n = number_bytes32_mul(b_one, b_zero);
    assert(n == NULL);

    n = number_bytes32_mul(b_zero, b_one);
    assert(n == NULL);

    bytes32_t b1, b2;
    b1 = BYTES32_UINT(2);
    b2 = BYTES32_UINT(3);
    n = number_bytes32_mul(b1, b2);
    ASSERT_NUMBER_UINT(n, 6);

    n = number_bytes32_mul(b_max, b_max);
    ASSERT_NUMBER_UINT(n, 1);
    ASSERT_NUMBER_BYTES32(n->next, b_max_1);
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
    n1 = NUMBER_UINT_DBG(1, 1);
    n2 = NUMBER_UINT(2);
    n = number_add(n1, n2);
    ASSERT_NUMBER_UINT(n, 2);
    ASSERT_NUMBER_UINT(n->next, 1);
    
    n1 = NUMBER_UINT(1);
    n2 = NUMBER_UINT_DBG(2, 1);
    n = number_add(n1, n2);
    ASSERT_NUMBER_UINT(n, 1);
    ASSERT_NUMBER_UINT(n->next, 2);
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
    n1 = NUMBER_UINT(1);
    n = NUMBER_UINT_DBG(2, 1);
    n1 = number_add(n1, n);
    n2 = NUMBER_UINT(3);
    n = NUMBER_UINT_DBG(4, 1);
    n2 = number_add(n2, n);
    n = NUMBER_UINT_DBG(5, 2);
    n2 = number_add(n2, n);
    printf("\n");
    number_display(n1);
    printf("\n");
    number_display(n2);
    n = number_mul(n1, n2);
    printf("\n");
    number_display(n);
}



void test_number()
{
    printf("\ntest number library\t\t");

    test_copy();
    test_add_uint();
    test_add_bytes32();
    test_bytes32_mul();

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
