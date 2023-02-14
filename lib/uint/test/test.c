#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include "../code.c"



void test_dec()
{
    printf("\n\ttest_dec");

    luint value = 0;
    assert(DECH(value) == 0);
    assert(DECL(value) == 0);

    value = 0x100000002;
    assert(DECH(value) == 1);
    assert(DECL(value) == 2);
}

void test_add()
{
    printf("\n\ttest add");

    luint res = uint_add(0, 0);
    assert(res == 0);

    res = uint_add(0, 5);
    assert(res == 5);

    res = uint_add(UINT_MAX, 3);
    assert(res == 0x100000002);

    res = uint_add(UINT_MAX, UINT_MAX);
    assert(res == 0x1FFFFFFFE);
}

void test_mul()
{
    printf("\n\ttest mul");

    luint res = uint_mul(2, 2);
    assert(res == 4);

    res = uint_mul(0, 5);
    assert(res == 0);

    res = uint_mul(UINT_MAX, 2);
    assert(res == 0x1FFFFFFFE);

    res = uint_mul(UINT_MAX, UINT_MAX);
    assert(res == 0xFFFFFFFE00000001);
}

void test_mix()
{
    printf("\n\ttest mix");

    uint res = uint_mix(0, 1, 3);
    assert(res == 8);

    res = uint_mix(1 << 31, 0, 3);
    assert(res == 4);

    res = uint_mix(1 << 31, 1, 3);
    assert(res == 12);

    res = uint_mix(1, 0, 0);
    assert(res == 0);
}

void test_uint()
{
    printf("\ntest uint");

    test_dec();
    test_add();
    test_mul();
    test_mix();
}



int main() {
    setbuf(stdout, NULL);
    test_uint();
    printf("\n\n\t\tTest successful\n\n");
    return 0;
}
