#ifndef __STACK_DEBUG_H__
#define __STACK_DEBUG_H__

#include "header.h"

STRUCT(bytes32_list)
{
    bytes32_t b;
    bytes32_list_p next;
};

#endif
