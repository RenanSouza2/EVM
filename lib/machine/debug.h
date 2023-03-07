#ifndef __EXAMPLE_DEBUG_H__
#define __EXAMPLE_DEBUG_H__

#include <stdbool.h>

#include "header.h"
#include "../bytes/header.h"
#include "../stack/header.h"

STRUCT(env)
{
    bytes32_t address;
    bytes32_t origin;
    bytes32_t caller;
    bytes32_t callvalue;
    bytes_t calldata;
    bytes32_t gasprice;
};

STRUCT(machine)
{
    bytes_t code;
    int pc;
    stack_t st;
    env_t env;
    bytes_t returndata;
};

typedef bytes32_t (*bytes32_1_1_f)(bytes32_t);
typedef bytes32_t (*bytes32_2_1_f)(bytes32_t,bytes32_t);

machine_t machine_init(char s[]);

bool machine_push(machine_p m, uchar op);
bool machine_1_1(machine_p m, bytes32_1_1_f arith);
bool machine_2_1(machine_p m, bytes32_2_1_f arith);

bool machine_exec(machine_p m, char code[]);

#endif
