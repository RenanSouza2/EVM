#ifndef __BYTES32_H__
#define __BYTES32_H__

#include <stdlib.h>

#include "../../submodule/c-library/struct.h"
#include "../uint/header.h"

#define SCALAR 8
STRUCT(bytes32)
{
    uint v[SCALAR];
};

void bytes32_display(bytes32_t value);

#define BYTES32_RESET(VALUE) memset(VALUE.v, 0, 32)
#define BYTES32_ASSIGN(VALUE1, VALUE2) memcpy(VALUE1, VALUE2, 32);

bytes32_t bytes32_add(bytes32_t b1, bytes32_t b2);
bytes32_t bytes32_mul(bytes32_t b1, bytes32_t b2);

#endif