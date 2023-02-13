#ifndef __BYTES32_H__
#define __BYTES32_H__

#include <stdlib.h>

#include "../uint/header.h"

#define SCALAR 8
typedef uint bytes32[SCALAR];

void bytes32_display(bytes32 value);

#define BYTES32_RESET(PTR) memset(PTR, 0, 32)
#define BYTES32_ASSIGN(PTR1, PTR2) memcpy(PTR1, PTR2, 32);

void bytes32_convert(bytes32 res, luint value_int);
void bytes32_add_uint(bytes32 res, uint value, int i);
void bytes32_add(bytes32 res, bytes32 value1, bytes32 value2);
void bytes32_mul(bytes32 res, bytes32 value1, bytes32 value2);

#endif