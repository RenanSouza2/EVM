#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "debug.h"

#define TRY(CODE) if(!(CODE)) return false

#ifdef DEBUG

#include "../bytes32/debug.h"
#include "../stack/debug.h"

#endif

machine_t machine_init(char s[])
{
    machine_t m;
    m.code = bytes_create_string(s);
    m.pc = 0;
    m.st = stack_init();
    return m;
}

bool machine_push(machine_p m, uchar op)
{
    uchar size = op - 0x5f;
    bytes_t b = bytes_get_mult(m->code, m->pc, size);
    bytes32_t b32 = bytes32_bytes(b);
    TRY(stack_push(&m->st, b32));
    m->pc += size;
    return true;
}

bool machine_1_1(machine_p m, bytes32_1_1_f func)
{
    bytes32_t b32;
    TRY(stack_pop(&b32, &m->st));
    b32 = func(b32);
    TRY(stack_push(&m->st, b32));
    return true;
}

bool machine_2_1(machine_p m, bytes32_2_1_f func)
{
    bytes32_t b32, b32_0, b32_1;
    TRY(stack_pop(&b32_0, &m->st));
    TRY(stack_pop(&b32_1, &m->st));
    b32 = func(b32_0, b32_1);
    TRY(stack_push(&m->st, b32));
    return true;
}

bool machine_exec(machine_p m, char code[])
{
    *m = machine_init(code);

    while (true)
    {
        uchar op = bytes_get(m->code, m->pc++);
        switch (op)
        {
            case 0x00: return true;
            case 0x01: TRY(machine_2_1(m, bytes32_add)); break;
            case 0x02: TRY(machine_2_1(m, bytes32_mul)); break;
            case 0x03: TRY(machine_2_1(m, bytes32_sub)); break;
            case 0x04: TRY(machine_2_1(m, bytes32_div)); break;
            case 0x06: TRY(machine_2_1(m, bytes32_mod)); break;
            
            case 0x10: TRY(machine_2_1(m, bytes32_lt)); break;
            case 0x11: TRY(machine_2_1(m, bytes32_gt)); break;

            case 0x14: TRY(machine_2_1(m, bytes32_eq)); break;
            case 0x15: TRY(machine_1_1(m, bytes32_is_zero)); break;

            case 0x1b: TRY(machine_2_1(m, bytes32_shr)); break;
            case 0x1c: TRY(machine_2_1(m, bytes32_shl)); break;



            case 0x60 ... 0x7f: TRY(machine_push(m, op)); break;
        
            default: return false;
        }
    }
}
