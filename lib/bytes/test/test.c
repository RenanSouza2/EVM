#include <stdio.h>
#include <assert.h>

#include "../debug.h"



void test_char_1()
{
    printf("\n\ttest char 1\t\t");

    char c = byte_char_1('0');
    assert(c == 0);

    c = byte_char_1('9');
    assert(c == 9);

    c = byte_char_1('a');
    assert(c == 10);

    c = byte_char_1('A');
    assert(c == 10);

    c = byte_char_1('f');
    assert(c == 15);

    c = byte_char_1('F');
    assert(c == 15);
}

void test_char_2()
{
    printf("\n\ttest char 2\t\t");

    char c = byte_char_2('0', '0');
    assert(c == 0);

    c = byte_char_2('0', 'F');
    assert(c == 0xf);

    c = byte_char_2('f', '0');
    assert(c == (char)0xf0);

    c = byte_char_2('f', 'f');
    assert(c == (char)0xff);
}

void test_create_string()
{
    printf("\n\ttest crate string\t\t");

    bytes_t b = bytes_create_string("0123456789abcdef");
    ASSERT_BYTES(b, 8, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef);
}

void test_get()
{
    printf("\n\ttest get\t\t");

    bytes_t b = bytes_create_string("0123456789abcdef");
    char b_exp[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x00};
    for(int i=0; i<=8; i++)
    {
        char c = bytes_get(b, i);
        assert(c == b_exp[i]);
    }
}

void test_get_mult()
{
    printf("\n\ttest get mult\t\t");

    bytes_t b = bytes_create_string("0123456789abcdef");
    b = bytes_get_mult(b, 1, 1);
    ASSERT_BYTES(b, 1, 0x23);

    b = bytes_create_string("0123456789abcdef");
    b = bytes_get_mult(b, 10, 1);
    ASSERT_BYTES(b, 1, 0x00);

    b = bytes_create_string("0123456789abcdef");
    b = bytes_get_mult(b, 4, 8);
    ASSERT_BYTES(b, 8, 0x89, 0xab, 0xcd, 0xef, 0x00, 0x00, 0x00, 0x00);
}

// bytes32_t bytes32_bytes(bytes_t b);



void test_bytes()
{
    printf("\ntest bytes\t\t");

    test_char_1();
    test_char_2();
    test_create_string();
    test_get();
    test_get_mult();
}

int main() 
{
    setbuf(stdout, NULL);
    test_bytes();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
