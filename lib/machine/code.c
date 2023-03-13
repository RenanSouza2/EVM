#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#include "debug.h"

#define TRY(CODE) if(!(CODE)) return false

#ifdef DEBUG

#include "../bytes32/debug.h"
#include "../stack/debug.h"
#include "../bytes/debug.h"

bool machine_memory()
{
    return stack_memory() && bytes_memory();
}

#endif

machine_t machine_init(char s[])
{
    machine_t m;
    m.code = bytes_create_string(s);
    m.pc = 0;
    m.st = stack_init();
    return m;
}

void machine_free(machine_t m)
{
    stack_free(m.st);
    bytes_free(m.code);
}

bool machine_push(machine_p m, uchar op)
{
    uchar size = op - 0x5f;
    bytes_t bs = bytes_get_mult(m->code, m->pc, size);
    bytes32_t b = bytes32_bytes(bs);
    TRY(stack_push(&m->st, b));
    m->pc += size;
    return true;
}

bool machine_1_1(machine_p m, bytes32_1_1_f func)
{
    bytes32_t b;
    TRY(stack_pop(&b, &m->st));
    b = func(b);
    TRY(stack_push(&m->st, b));
    return true;
}

bool machine_2_1(machine_p m, bytes32_2_1_f func)
{
    bytes32_t b, b0, b1;
    TRY(stack_pop(&b0, &m->st));
    TRY(stack_pop(&b1, &m->st));
    b = func(b0, b1);
    TRY(stack_push(&m->st, b));
    return true;
}

bool machine_3_1(machine_p m, bytes32_3_1_f func)
{
    bytes32_t b, b0, b1, b2;
    TRY(stack_pop(&b0, &m->st));
    TRY(stack_pop(&b1, &m->st));
    TRY(stack_pop(&b2, &m->st));
    b = func(b0, b1, b2);
    TRY(stack_push(&m->st, b));
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
            case 0x05: TRY(machine_2_1(m, bytes32_sdiv)); break;
            case 0x06: TRY(machine_2_1(m, bytes32_mod)); break;
            case 0x07: TRY(machine_2_1(m, bytes32_smod)); break;
            case 0x08: TRY(machine_3_1(m, bytes32_addmod)); break;
            case 0x09: TRY(machine_3_1(m, bytes32_mulmod)); break;
            case 0x0A: TRY(machine_2_1(m, bytes32_exp)); break;
            case 0x0B: TRY(machine_2_1(m, bytes32_sign_extend)); break;

            case 0x10: TRY(machine_2_1(m, bytes32_lt)); break;
            case 0x11: TRY(machine_2_1(m, bytes32_gt)); break;

            case 0x14: TRY(machine_2_1(m, bytes32_eq)); break;
            case 0x15: TRY(machine_1_1(m, bytes32_is_zero)); break;
            
            case 0x19: TRY(machine_1_1(m, bytes32_not)); break;

            case 0x1b: TRY(machine_2_1(m, bytes32_shl)); break;
            case 0x1c: TRY(machine_2_1(m, bytes32_shr)); break;

            case 0x60 ... 0x7f: TRY(machine_push(m, op)); break;
        
            default: return false;
        }
    }
}
