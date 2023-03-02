#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "debug.h"

#include "../uint/header.h"



#ifdef DEBUG

#include <stdarg.h>

#include "../bytes32/debug.h"

number_p number_create_null();
number_p number_create_bytes32(bytes32_t b);

number_p number_create_bytes32_off(bytes32_t b, int i)
{    
    if(i == 0) return number_create_bytes32(b);

    number_p n = number_create_null();
    n->next = number_create_bytes32_off(b, i-1);
    return n;
}

void number_display(number_p n)
{
    if(n == NULL) printf("\nempty number");

    for(int i=0; n; i++, n = n->next)
    {
        printf("\nnum[%d]: ", i);
        bytes32_display(n->b);
    }
}

number_p number_create_uint_mult(int count, ...)
{
    va_list args;
    va_start(args, count);

    number_p n, n_out;
    uint value = va_arg(args, uint);
    n_out = n = NUMBER_UINT(value);

    for(int i=1; i<count; i++)
    {
        value = va_arg(args, uint);
        n = n->next = NUMBER_UINT(value);
    }

    return n_out;
}

number_p number_create_bytes32_mult(int count, ...)
{
    va_list args;
    va_start(args, count);

    number_p n, n_out;
    bytes32_t value = va_arg(args, bytes32_t);
    n_out = n = number_create_bytes32(value);

    for(int i=1; i<count; i++)
    {
        value = va_arg(args, bytes32_t);
        n = n->next = number_create_bytes32(value);
    }

    return n_out;
}

bool number_uint_mult(number_p n, int count, ...)
{
    va_list args;
    va_start(args, count);

    for(int i=0; i<count; i++, n = n->next)
    {
        if(n == NULL)
        {
            printf("\nfailling at iteraction %d | reason 1", i);
            printf("\nexpected a not null pointer\t\t");
            return false;
        }
        
        uint value = va_arg(args, uint);
        bytes32_t b = BYTES32_UINT(value);
        if(bytes32_cmp(n->b, b) != 0)
        {
            printf("\nfailling at iteraction %d | reason 2", i);
            printf("\nexpected value: "); bytes32_display(b);
            printf("\nreturned value: "); bytes32_display(n->b);
            printf("\n\t\t");
            return false;
        }
    }
    return true;
}

bool number_bytes32_mult(number_p n, int count, ...)
{
    va_list args;
    va_start(args, count);

    for(int i=0; i<count; i++, n = n->next)
    {
        if(n == NULL)
        {
            printf("\nfailling at iteraction %d | reason 1", i);
            printf("\nexpected a not null pointer\t\t");
            return false;
        }
        
        bytes32_t b = va_arg(args, bytes32_t);
        if(bytes32_cmp(n->b, b) != 0)
        {
            printf("\nfailling at iteraction %d | reason 2", i);
            printf("\nexpected value: "); bytes32_display(b);
            printf("\nreturned value: "); bytes32_display(n->b);
            printf("\n\t\t");
            return false;
        }
    }
    return true;
}

#endif



number_p number_create_null()
{
    number_p n = calloc(1, sizeof(number_t));
    assert(n);
    return n;
}

number_p number_create_null_list(int i, number_p next)
{
    if(i == 0) return next;

    number_p n = number_create_null();
    n->next = number_create_null_list(i-1, next);
    return n;
}

number_p number_create_bytes32(bytes32_t b)
{
    number_p n = number_create_null();
    n->b = b;
    return n;
}

void number_free(number_p n)
{
    while(n != NULL)
    {
        number_p n_aux = n->next;
        free(n);
        n = n_aux;
    }
}

number_p number_copy(number_p n)
{
    if(n == NULL) return NULL;

    number_p n_copy;
    n_copy = number_create_bytes32(n->b);

    n = n->next;
    for(number_p n_aux = n_copy; n != NULL; n = n->next, n_aux = n_aux->next)
        n_aux->next = number_create_bytes32(n->b);

    return n_copy;
} 


number_p number_add_bytes32_rec2(number_p n, bytes32_t b)
{
    if(n == NULL) return number_create_bytes32(b);

    bytes32_dual_t bd = bytes32_full_add(n->b, b);
    n->b = bd.b[0];
    n->next = number_add_bytes32_rec2(n->next, bd.b[1]);
    return n;
}

number_p number_add_bytes32_rec1(number_p n, bytes32_t b, int i)
{
    if(i == 0) return number_add_bytes32_rec2(n, b);

    if(n == NULL) n = number_create_null();
    n->next = number_add_bytes32(n->next, b, i-1);
    return n;
}

number_p number_add_bytes32(number_p n, bytes32_t b, int i)
{
    if(bytes32_is_zero_bool(b)) return n;
    return number_add_bytes32_rec1(n, b, i);
}


bool number_is_zero(number_p n)
{
    if(n == NULL) return true;
    if(!bytes32_is_zero_bool(n->b)) return false;
    return number_is_zero(n->next);
}

int number_cmp(number_p n1, number_p n2)
{
    if(n1 == NULL) return -!number_is_zero(n2);
    if(n2 == NULL) return  !number_is_zero(n1);

    int res = number_cmp(n1->next, n2->next);
    if(res != 0) return res;

    return bytes32_cmp(n1->b, n2->b);
}


number_p number_shl_rec(number_p n, uint u)
{
    if(n->next == NULL)
    {
        bytes32_t b = bytes32_shr_uint(n->b, 256 - u);
        return number_create_bytes32(b);
    }

    bytes32_t b, b1, b2;
    b1 = bytes32_shr_uint(n->b, 256 - u);
    b2 = bytes32_shl_uint(n->next->b, u);
    b = bytes32_add(b1, b2);
    
    number_p n_out = number_create_bytes32(b);
    n_out->next = number_shl_rec(n->next, u);
    return n_out;
}

number_p number_shr_rec(number_p n, uint u)
{
    if(n->next == NULL)
    {
        bytes32_t b;
        b = bytes32_shr_uint(n->b, u);
        return number_create_bytes32(b);
    }

    bytes32_t b, b1, b2;
    b1 = bytes32_shr_uint(n->b, u);
    b2 = bytes32_shl_uint(n->next->b, 256-u);
    b = bytes32_add(b1, b2);
    
    number_p n_out = number_create_bytes32(b);
    n_out->next = number_shr_rec(n->next, u);
    return n_out;
}



number_p number_add(number_p n1, number_p n2)
{
    n1 = number_copy(n1);
    for(int i=0; n2; i++, n2 = n2->next)
        n1 = number_add_bytes32(n1, n2->b, i);

    return n1;
}

number_p number_mul(number_p n1, number_p n2)
{
    if(n1 == NULL) return NULL;
    if(n2 == NULL) return NULL;

    
    number_p n, n2_0;
    int j;
    n = NULL;
    n2_0 = n2;
    for(int i=0; n1; i++, n1 = n1->next)
    for(j=0, n2 = n2_0; n2; j++, n2 = n2->next)
    {
        bytes32_dual_t bd = bytes32_full_mul(n1->b, n2->b);
        n = number_add_bytes32(n, bd.b[0], i + j);
        n = number_add_bytes32(n, bd.b[1], i + j + 1);
    }
    return n;
}

number_p number_shl(number_p n, uint u)
{
    if(n == NULL) return NULL;

    int jmp = u >> 8;
    int off = u & 255;

    number_p n_out;
    if(off == 0) n_out = number_copy(n);
    else
    {
        bytes32_t b = bytes32_shl_uint(n->b, off);
        n_out = number_create_bytes32(b);
        n_out->next = number_shl_rec(n, off);
    }

    return number_create_null_list(jmp, n_out);
}

number_p number_shr(number_p n, uint u)
{
    for(; u > 256; u -= 256, n = n->next)
        if(n == NULL)
            return NULL;

    if(u == 0) return number_copy(n);

    return number_shr_rec(n, u);
}