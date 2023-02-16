#ifndef __UINT_H__
#define __UINT_H__

#include <stdlib.h>

typedef unsigned long int luint;

#define DECH(LUINT) ((uint)(((luint)(LUINT)) >> 32))
#define DECL(LUINT) ((uint)(LUINT))

luint uint_add(uint u1, uint u2);
luint uint_mul(uint u1, uint u2);
uint  uint_mix(uint u1, uint u2, uint shift);

#endif