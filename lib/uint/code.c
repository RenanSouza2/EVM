#include "debug.h"

luint uint_add(uint u1, uint u2) 
{
    return (luint)u1 + (luint)u2;
}

luint uint_mul(uint u1, uint u2) 
{
    return (luint)u1 * (luint)u2;
}

uint uint_mix(uint u1, uint u2, uint shift)
{
    if(shift == 0) return u2;
    return (u1 >> (32 - shift)) + (u2 << shift);
}
