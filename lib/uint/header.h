#ifndef __UINT_H__
#define __UINT_H__

typedef unsigned long int luint;

#define DECH(LUINT) ((uint)((LUINT) >> 32))
#define DECL(LUINT) ((uint)(LUINT))

uint  uint_not(uint u);
luint uint_add(uint u1, uint u2);
luint uint_mul(uint u1, uint u2);

#endif