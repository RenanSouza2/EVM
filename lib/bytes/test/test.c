#include <stdio.h>
#include <string.h>
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

// bytes_t bytes_create_string(char s[])
void test_create_string()
{
    printf("\n\ttest crate string\t\t");

    bytes_t s = bytes_create_string("ab");
    assert(s.n == 1);
    assert(s.s[0] == (char)0xab);

    s = bytes_create_string("0123456789abcdef");
    assert(s.n == 8);
    char s_exp[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef};
    assert(memcmp(s.s, s_exp, 8) == 0);
}


void test_bytes()
{
    printf("\ntest bytes\t\t");

    test_char_1();
    test_char_2();
    test_create_string();
}

int main() 
{
    setbuf(stdout, NULL);
    test_bytes();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
