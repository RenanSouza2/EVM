#include <stdio.h>
#include <assert.h>

#include "../debug.h"
#include "../../bytes32/debug.h"
#include "../../stack/debug.h"

void test_push()
{
    printf("\n\ttest push\t\t");

    printf("\n\t\ttest push 1\t\t");
    machine_t m = machine_init("ab");
    assert(machine_push(&m, 0x60));
    ASSERT_BYTES32_UINT(m.st.b[0], 0xAB);
    machine_free(m);
    
    printf("\n\t\ttest push 2\t\t");
    m = machine_init("ab");
    assert(machine_push(&m, 0x61));
    ASSERT_BYTES32_UINT(m.st.b[0], 0xAB00);
    machine_free(m);
    
    printf("\n\t\ttest push 3\t\t");
    m = machine_init("ab");
    assert(machine_push(&m, 0x7f));
    ASSERT_BYTES32(m.st.b[0], 0xAB000000, 0, 0, 0, 0, 0, 0, 0);
    machine_free(m);

    assert(machine_memory());
}

void test_2_1()
{
    printf("\n\ttest 2 1\t\t");

    machine_t m = machine_init("");
    assert(machine_2_1(&m, bytes32_add) == false);
    machine_free(m);

    bytes32_t b = BYTES32_UINT(1);
    m = machine_init("");
    stack_push(&m.st, b);
    assert(machine_2_1(&m, bytes32_add) == false);
    machine_free(m);
    
    m = machine_init("");
    b = BYTES32_UINT(1);
    stack_push(&m.st, b);
    b = BYTES32_UINT(2);
    stack_push(&m.st, b);
    assert(machine_2_1(&m, bytes32_add));
    ASSERT_BYTES32_UINT(m.st.b[0], 3);
    machine_free(m);

    assert(machine_memory());
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
