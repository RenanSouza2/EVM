#include <stdio.h>
#include <assert.h>

#include "../debug.h"
#include "../../bytes32/debug.h"
#include "../../stack/debug.h"

void test_push()
{
    printf("\n\ttest push\t\t");

    machine_t m = machine_init("ab");
    assert(machine_push(&m, 0x60));
    ASSERT_BYTES32_UINT(m.st.bl->b, 0xAB);
    
    m = machine_init("ab");
    assert(machine_push(&m, 0x61));
    ASSERT_BYTES32_UINT(m.st.bl->b, 0xAB00);
    
    m = machine_init("ab");
    assert(machine_push(&m, 0x7f));
    ASSERT_BYTES32(m.st.bl->b, 0xAB000000, 0, 0, 0, 0, 0, 0, 0);
}

void test_2_1()
{
    printf("\n\ttest 2 1\t\t");

    machine_t m = machine_init("");
    assert(machine_2_1(&m, bytes32_add) == false);

    bytes32_t b32 = BYTES32_UINT(1);
    m = machine_init("");
    stack_push(&m.st, b32);
    assert(machine_2_1(&m, bytes32_add) == false);
    
    m = machine_init("");
    b32 = BYTES32_UINT(1);
    stack_push(&m.st, b32);
    b32 = BYTES32_UINT(2);
    stack_push(&m.st, b32);
    assert(machine_2_1(&m, bytes32_add));
    ASSERT_BYTES32_UINT(m.st.bl->b, 3);
}

// bool machine_exec(machine_p m, char code[])


void test_machine()
{
    printf("\ntest machine\t\t");

    test_push();
    test_2_1();
}

int main() 
{
    setbuf(stdout, NULL);
    test_machine();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
