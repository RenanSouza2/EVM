#ifndef __NUMBER_H__
#define __NUMBER_H__

#include "../../utils/struct.h"
#include "../bytes32/header.h"

PLACEHOLDER(number);

number_p number_create_bytes32(bytes32_t b);

number_p number_add(number_p n1, number_p n2);
number_p number_mul(number_p n1, number_p n2);

#endif
