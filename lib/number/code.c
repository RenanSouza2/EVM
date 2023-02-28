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
        if(n == NULL) return false;
        
        bytes32_t b = va_arg(args, bytes32_t);
        if(bytes32_cmp(n->b, b) != 0) return false;
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



number_p number_add_uint(number_p n, uint u, int i)
{
    if(u == 0) return n;

    if(n == NULL) n = number_create_null();
    if(i >= SCALAR)
    {
        n->next = number_add_uint(n->next, u, i-8);
        return n;
    }

    luint lu = uint_add(n->b.v[i], u);
    n->b.v[i] = DECL(lu);

    return number_add_uint(n, DECH(lu), i+1);
}

number_p number_add_bytes32(number_p n, bytes32_t b, int i)
{
    if(bytes32_is_zero_bool(b)) return n;

    if(i > 0)
    {
        if(n == NULL) n = number_create_null();
        n->next = number_add_bytes32(n->next, b, i-1);
        return n;
    }

    for(int i=0; i<SCALAR; i++)
        n = number_add_uint(n, b.v[i], i);

    return n;
}

number_p number_add_off(number_p n1, number_p n2, int i)
{
    number_p n2_aux = n2;
    for(; n2; i++, n2 = n2->next)
        n1 = number_add_bytes32(n1, n2->b, i);
    number_free(n2_aux);
    return n1;
}

number_p number_bytes32_mul(bytes32_t b1, bytes32_t b2)
{
    number_p n = NULL;
    for(int i=0; i<SCALAR; i++)
    for(int j=0; j<SCALAR; j++)
    {
        luint lu = uint_mul(b1.v[i], b2.v[j]);
        n = number_add_uint(n, DECL(lu), i + j);
        n = number_add_uint(n, DECH(lu), i + j + 1);
    }
    return n;
}

number_p number_mul_bytes32(number_p n, bytes32_t b)
{
    if(bytes32_is_zero_bool(b)) return NULL;

    number_p n_out = NULL;
    for(int i=0; n; i++, n = n->next)
    {
        number_p n_aux = number_bytes32_mul(n->b, b);
        n_out = number_add_off(n_out, n_aux, i);
    }
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

    
    number_p n = NULL;
    for(int i=0; n1; i++, n1 = n1->next)
    {
        number_p n_aux = number_mul_bytes32(n2, n1->b);
        n = number_add_off(n, n_aux, i);
    }
    return n;
}
