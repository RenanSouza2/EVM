#include <stdio.h>
#include <assert.h>

#include "../debug.h"
#include "../../bytes32/debug.h"



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

    bytes_din_t b = bytes_din_create_string("0123456789abcdef");
    ASSERT_BYTES(b, 8, 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef);
    bytes_din_free(b);

    assert(bytes_din_memory());
}

void test_get()
{
    printf("\n\ttest get\t\t");

    bytes_din_t b = bytes_din_create_string("0123456789abcdef");
    char b_exp[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x00};
    for(int i=0; i<=8; i++)
    {
        char c = bytes_din_get(b, i);
        assert(c == b_exp[i]);
    }
    bytes_din_free(b);

    assert(bytes_din_memory());
}

void test_get_mult()
{
    printf("\n\ttest get mult\t\t");

    bytes_din_t b1 = bytes_din_create_string("0123456789abcdef");
    bytes_din_t b2 = bytes_din_get_mult(b1, 1, 1);
    ASSERT_BYTES(b2, 1, 0x23);
    bytes_din_free(b1);
    bytes_din_free(b2);

    b1 = bytes_din_create_string("0123456789abcdef");
    b2 = bytes_din_get_mult(b1, 10, 1);
    ASSERT_BYTES(b2, 1, 0x00);
    bytes_din_free(b1);
    bytes_din_free(b2);

    b1 = bytes_din_create_string("0123456789abcdef");
    b2 = bytes_din_get_mult(b1, 4, 8);
    ASSERT_BYTES(b2, 8, 0x89, 0xab, 0xcd, 0xef, 0x00, 0x00, 0x00, 0x00);
    bytes_din_free(b1);
    bytes_din_free(b2);

    assert(bytes_din_memory());
}

void test_bytes32()
{
    printf("\n\ttest bytes\t\t");

    bytes_din_t b = bytes_din_create_string("ab");
    bytes32_t b32 = bytes32_bytes_din(b);
    ASSERT_BYTES32_UINT(b32, 0xab);
    
    b = bytes_din_create_string("abcd");
    b32 = bytes32_bytes_din(b);
    ASSERT_BYTES32_UINT(b32, 0xabcd);
    
    b = bytes_din_create_string("0123456789abcdef");
    b32 = bytes32_bytes_din(b);
    ASSERT_BYTES32(b32, 0, 0, 0, 0, 0, 0, 0x01234567, 0x89abcdef);

    assert(bytes_din_memory());
}




void test_bytes_din()
{
    printf("\ntest bytes din\t\t");

    test_char_1();
    test_char_2();
    test_create_string();
    test_get();
    test_get_mult();
    test_bytes32();
}

int main() 
{
    setbuf(stdout, NULL);
    test_bytes_din();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
