#ifndef __BYTES64_H__
#define __BYTES64_H__

#include "../../utils/struct.h"
#include "../bytes32/header.h"

#define SCALAR_2 (2*SCALAR)

STRUCT(bytes64)
{
    uint v[SCALAR_2];
};

#endif
