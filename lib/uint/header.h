#ifndef __UINT_H__
#define __UINT_H__

typedef unsigned long int luint;

#define DECH(LUINT) ((uint)((LUINT) >> 32))
#define DECL(LUINT) ((uint)(LUINT))

luint uint_add(uint a, uint b);
luint uint_add_3(uint a, uint b, uint c);
luint uint_mul(uint a, uint b);

#endif