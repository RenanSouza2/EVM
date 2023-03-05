#include <stdio.h>
#include <assert.h>

#include "../debug.h"

void test_1()
{
    printf("\n\ttest 1\t\t");

    // machine();
    printf("\n\nNO TESTS IMPLEMENTED");
}

void test_machine()
{
    printf("\ntest machine\t\t");

    test_1();
}

int main() 
{
    setbuf(stdout, NULL);
    test_machine();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
