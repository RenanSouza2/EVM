#include <stdio.h>
#include <assert.h>

#include "../debug.h"
#include "../../bytes32/debug.h"

void test_bytes32()
{
    printf("\n\ttest bytes32\t\t");

    bytes32_t b = BYTES32(8, 7, 6, 5, 4, 3, 2, 1);
    bytes64_t bd = bytes64_bytes32(b);
    ASSERT_BYTES64(bd, 0, 0, 0, 0, 0, 0, 0, 0, 8, 7, 6, 5, 4, 3, 2, 1);
}

void test_bytes64()
{
    printf("\ntest bytes32\t\t");

    test_bytes32();
}


int main() 
{
    setbuf(stdout, NULL);
    test_bytes64();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
