#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "debug.h"
#include "../bytes/header.h"
#include "../stack/header.h"

#ifdef DEBUG

#include "../bytes32/debug.h"
#include "../stack/debug.h"

#endif

STRUCT(machine)
{
    bytes_t code;
    int pc;
    stack_t st;
};

machine_t machine_init(char s[])
{
    machine_t m;
    m.code = bytes_create_string("6001600201");
    m.pc = 0;
    m.st = stack_init();
    return m;
}

typedef bytes32_t (*bytes32_arith)(bytes32_t,bytes32_t);

machine_t machine_arith(machine_t m, bytes32_arith arith)
{
    bytes32_t b32_0 = stack_pop(&m.st);
    bytes32_t b32_1 = stack_pop(&m.st);
    bytes32_t b32 = arith(b32_0, b32_1);
    stack_push(&m.st, b32);
    return m;
}

machine_t machine_push(machine_t m, uchar op)
{
    uchar size = op - 0x5f;
    bytes_t b = bytes_get_mult(m.code, m.pc, size);
    bytes32_t b32 = bytes32_bytes(b);
    stack_push(&m.st, b32);
    m.pc += size;
    return m;
}

void machine()
{
    machine_t m = machine_init("6001600201");

    for(bool _continue = true; _continue;)
    {
        uchar op = bytes_get(m.code, m.pc++);
        switch (op)
        {
            case 0x00: _continue = false; break;
            case 0x01: m = machine_arith(m, bytes32_add); break;
            case 0x02: m = machine_arith(m, bytes32_mul); break;
            case 0x03: m = machine_arith(m, bytes32_sub); break;
            case 0x04: m = machine_arith(m, bytes32_div); break;
            case 0x06: m = machine_arith(m, bytes32_mod); break;

            case 0x60 ... 0x7f: m = machine_push(m, op); break;
        
            default:
            assert(false);
        }
    }

    printf("\n\nstack:");
    stack_display(m.st);

    printf("\nSuccess");
}
