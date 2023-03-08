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
    number_free(n1);
    number_free(n2);

    n1 = number_create_uint_mult(2, 1, 2);
    n2 = number_copy(n1);
    assert(number_uint_mult(n2, 2, 1, 2));
    number_free(n1);
    number_free(n2);

    assert(number_memory());
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
    number_free(n1);
    number_free(n2);
    
    n1 = NUMBER_UINT(1);
    res = number_cmp(n1, NULL);
    assert(res > 0);
    number_free(n1);
    
    n1 = NUMBER_UINT(1);
    res = number_cmp(NULL, n1);
    assert(res < 0);
    number_free(n1);

    n1 = NUMBER_UINT(1);
    res = number_cmp(n1, n1);
    assert(res == 0);
    number_free(n1);

    n1 = number_create_uint_mult(2, 1, 2);
    n2 = number_create_uint_mult(2, 2, 1);
    res = number_cmp(n1, n2);
    assert(res > 0);
    number_free(n1);
    number_free(n2);
    
    n1 = number_create_uint_mult(2, 2, 1);
    n2 = number_create_uint_mult(2, 1, 1);
    res = number_cmp(n1, n2);
    assert(res > 0);
    number_free(n1);
    number_free(n2);

    assert(number_memory());
}



void test_add_bytes32()
{
    printf("\n\ttest add bytes32\t\t");

    bytes32_t b = BYTES32_UINT(1);
    number_p n = number_add_bytes32(NULL, b);
    ASSERT_NUMBER_UINT(n, 1);
    number_free(n);

    b = BYTES32_UINT(3);
    n = number_create_bytes32(b_max);
    n = number_add_bytes32(n, b);
    assert(number_uint_mult(n, 2, 2, 1));
    number_free(n);

    b = BYTES32_UINT(0);
    n = number_add_bytes32(NULL, b);
    assert(n == NULL);

    assert(number_memory());
}

void test_sub_bytes32()
{
    printf("\n\ttest sub bytes32\t\t");

    printf("\n\t\ttest sub bytes32 1\t\t");
    number_p n = NUMBER_UINT(5);
    n = number_sub_bytes32(n, b_zero);
    ASSERT_NUMBER_UINT(n, 5);
    number_free(n);

    printf("\n\t\ttest sub bytes32 2\t\t");
    n = NUMBER_UINT(5);
    n = number_sub_bytes32(n, b_one);
    ASSERT_NUMBER_UINT(n, 4);
    number_free(n);

    printf("\n\t\ttest sub bytes32 3\t\t");
    n = NUMBER_UINT(5);
    bytes32_t b = BYTES32_UINT(5);
    n = number_sub_bytes32(n, b);
    assert(n == NULL);

    printf("\n\t\ttest sub bytes32 4\t\t");
    n = number_create_uint_mult(2, 0, 1);
    n = number_sub_bytes32(n, b_one);
    ASSERT_NUMBER_BYTES32(n, b_max);
    assert(n->next == NULL);
    number_free(n);
    
    printf("\n\t\ttest sub bytes32 5\t\t");
    n = number_create_uint_mult(3, 0, 0, 1);
    n = number_sub_bytes32(n, b_one);
    assert(number_bytes32_mult(n, 2, b_max, b_max));
    number_free(n);

    assert(number_memory());
}

// number_p number_mul_bytes32(number_p n, number_p n1, bytes32_t b)
void test_mul_bytes32()
{
    printf("\n\ttest mul bytes32\t\t");

    bytes32_t b = BYTES32_UINT(2);
    number_p n = number_mul_bytes32(NULL, NULL, b);
    assert(n == NULL);

    b = BYTES32_UINT(2);
    n = NUMBER_UINT(1);
    n = number_mul_bytes32(n, NULL, b);
    ASSERT_NUMBER_UINT(n, 1);
    number_free(n);

    number_p n1 = NUMBER_UINT(3);
    b = BYTES32_UINT(2);
    n = number_mul_bytes32(NULL, n1, b);
    ASSERT_NUMBER_UINT(n, 6);
    number_free(n1);
    number_free(n);

    n1 = number_create_bytes32(b_max);
    n = number_mul_bytes32(NULL, n1, b_max);
    assert(number_bytes32_mult(n, 2, b_one, b_max_1));
    number_free(n1);
    number_free(n);

    b = BYTES32(0, 0, 0, 1, 0, 0, 0, 0);
    n1 = number_create_bytes32(b);
    n = number_mul_bytes32(NULL, n1, b);
    assert(number_uint_mult(n, 2, 0, 1));
    number_free(n1);
    number_free(n);
    
    n1 = number_create_bytes32_mult(2, b_max, b_max);
    n = number_mul_bytes32(NULL, n1, b_max);
    assert(number_bytes32_mult(n, 3, b_one, b_max, b_max_1));
    number_free(n1);
    number_free(n);

    n1 = number_create_bytes32_mult(2, b_max, b_max);
    n = number_create_bytes32_mult(2, b_max, b_max);
    n = number_mul_bytes32(n, n1, b_max);
    assert(number_bytes32_mult(n, 3, b_zero, b_max, b_max));
    number_free(n1);
    number_free(n);

    assert(number_memory());
}




void test_add()
{
    printf("\n\ttest add\t\t");

    number_p n1 = number_add(NULL, NULL);
    assert(n1 == NULL);

    number_p n2;
    n1 = NUMBER_UINT(1);
    n1 = number_add(n1, NULL);
    ASSERT_NUMBER_UINT(n1, 1);
    number_free(n1);

    n2 = NUMBER_UINT(1);
    n1 = number_add(NULL, n2);
    ASSERT_NUMBER_UINT(n1, 1);
    number_free(n1);
    number_free(n2);

    n1 = NUMBER_UINT(1);
    n2 = NUMBER_UINT(1);
    n1 = number_add(n1, n2);
    ASSERT_NUMBER_UINT(n1, 2);
    number_free(n1);
    number_free(n2);

    n1 = number_create_uint_mult(2, 0, 1);
    n2 = NUMBER_UINT(2);
    n1 = number_add(n1, n2);
    assert(number_uint_mult(n1, 2, 2, 1));
    number_free(n1);
    number_free(n2);
    
    n1 = NUMBER_UINT(1);
    n2 = number_create_uint_mult(2, 0, 2);
    n1 = number_add(n1, n2);
    assert(number_uint_mult(n1, 2, 1, 2));
    number_free(n1);
    number_free(n2);
    
    assert(number_memory());
}

void test_sub()
{
    printf("\n\ttest sub\t\t");

    printf("\n\t\ttest sub 1\t\t");
    number_p n1 = number_sub(NULL, NULL);
    assert(n1 == NULL);

    printf("\n\t\ttest sub 2\t\t");
    n1 = NUMBER_UINT(1);
    n1 = number_sub(n1, NULL);
    ASSERT_NUMBER_UINT(n1, 1);
    number_free(n1);

    printf("\n\t\ttest sub 3\t\t");
    number_p n2;
    n1 = NUMBER_UINT(1);
    n2 = NUMBER_UINT(1);
    n1 = number_sub(n1, n2);
    assert(n1 == NULL);
    number_free(n2);

    n1 = number_create_uint_mult(2, 0, 1);
    n2 = NUMBER_UINT(1);
    n1 = number_sub(n1, n2);
    ASSERT_NUMBER_BYTES32(n1, b_max);
    number_free(n1);
    number_free(n2);
    
    n1 = number_create_uint_mult(2, 0, 1);
    n2 = number_create_uint_mult(2, 0, 1);
    n1 = number_sub(n1, n2);
    assert(n1 == NULL);
    number_free(n2);

    assert(number_memory());
}

void test_mul()
{
    printf("\n\ttest mul\t\t");

    number_p n1 = number_mul(NULL, NULL);
    assert(n1 == NULL);

    n1 = NUMBER_UINT(1);
    n1 = number_mul(n1, NULL);
    assert(n1 == NULL);

    assert(number_memory());

    number_p n2 = NUMBER_UINT(1);
    n1 = number_mul(NULL, n2);
    assert(n1 == NULL);
    number_free(n2);
    
    assert(number_memory());

    n1 = NUMBER_UINT(1);
    n2 = NUMBER_UINT(1);
    n1 = number_mul(n1, n2);
    ASSERT_NUMBER_UINT(n1, 1);
    number_free(n1);
    number_free(n2);
    
    assert(number_memory());

    n1 = number_create_uint_mult(2, 1, 2);
    n2 = number_create_uint_mult(3, 3, 4, 5);
    n1 = number_mul(n1, n2);
    assert(number_uint_mult(n1, 4, 3, 10, 13, 10));
    number_free(n1);
    number_free(n2);
    
    assert(number_memory());

    n1 = number_create_bytes32_mult(2, b_max, b_max);
    n2 = number_create_bytes32_mult(2, b_max, b_max);
    n1 = number_mul(n1, n2);
    assert(number_bytes32_mult(n1, 4, b_one, b_zero, b_max_1, b_max));
    number_free(n1);
    number_free(n2);

    n1 = number_create_uint_mult(2, 0, 1);
    n2 = number_create_uint_mult(2, 0, 1);
    n1 = number_mul(n1, n2);
    assert(number_uint_mult(n1, 3, 0, 0, 1));
    number_free(n1);
    number_free(n2);

    assert(number_memory());
}



void test_shl()
{
    printf("\n\ttest shl\t\t");

    number_p n = NUMBER_UINT(1);
    n = number_shl(n, 0);
    ASSERT_NUMBER_UINT(n, 1);
    number_free(n);

    assert(number_memory());

    n = NUMBER_UINT(1);
    n = number_shl(n, 1);
    ASSERT_NUMBER_UINT(n, 2);
    number_free(n);

    assert(number_memory());

    n = NUMBER_UINT(1);
    n = number_shl(n, 128);
    ASSERT_NUMBER(n, 0, 0, 0, 1, 0, 0, 0, 0);
    number_free(n);

    assert(number_memory());

    n = NUMBER_UINT(1);
    n = number_shl(n, 255);
    ASSERT_NUMBER_BYTES32(n, b_Q255);
    number_free(n);

    assert(number_memory());
}

void test_shr()
{
    printf("\n\ttest shr\t\t");

    number_p n = number_create_bytes32(b_Q255);
    n = number_shr(n, 0);
    ASSERT_NUMBER_BYTES32(n, b_Q255);
    number_free(n);

    n = number_create_bytes32(b_Q255);
    n = number_shr(n, 1);
    bytes32_t b0 = BYTES32(0x40000000, 0, 0, 0, 0, 0, 0, 0);
    ASSERT_NUMBER_BYTES32(n, b0);
    number_free(n);

    n = number_create_bytes32(b_Q255);
    n = number_shr(n, 128);
    b0 = BYTES32(0, 0, 0, 0, 0x80000000, 0, 0, 0);
    ASSERT_NUMBER_BYTES32(n, b0);
    number_free(n);

    n = number_create_bytes32(b_Q255);
    n = number_shr(n, 255);
    ASSERT_NUMBER_UINT(n, 1);
    number_free(n);

    n = number_create_bytes32(b_one);
    n = number_shr(n, 1);
    assert(n == NULL);

    assert(number_memory());
}



void test_number()
{
    printf("\ntest number library\t\t");

    test_copy();
    test_number_cmp();

    test_add_bytes32();
    test_sub_bytes32();
    test_mul_bytes32();

    test_add();
    test_sub();
    test_mul();

    test_shl();
    test_shr();
    
    assert(number_memory());
}

int main() 
{
    setbuf(stdout, NULL);
    test_number();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
