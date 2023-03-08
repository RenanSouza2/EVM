#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "debug.h"

#include "../uint/header.h"



#ifdef DEBUG

#include <stdarg.h>

#include "../bytes32/debug.h"

int number_created;
int number_freed;

#define NUMBER_INC() number_created++;
#define NUMBER_DEC()    \
    {   \
        assert(number_created > number_freed);  \
        number_freed++; \
    }

bool number_memory()
{
    if(number_created == number_freed) return true;

    printf("\nmissed %u number allocated\t\t", number_created - number_freed);
    return false;
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
    n_out = n = number_create_bytes32_force(value);

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

#else
#define NUMBER_INC()
#define NUMBER_DEC()
#endif



number_p number_create_null()
{
    number_p n = calloc(1, sizeof(number_t));
    assert(n);
    NUMBER_INC();
    return n;
}

number_p number_create_bytes32_force(bytes32_t b)
{
    number_p n = number_create_null();
    n->b = b;
    return n;
}

number_p number_create_bytes32(bytes32_t b)
{
    if(bytes32_is_zero_bool(b)) return NULL;
    number_p n = number_create_null();
    n->b = b;
    return n;
}

void number_free(number_p n)
{
    if(n == NULL) return;
    number_free(n->next);

    free(n);
    NUMBER_DEC();
}

number_p number_copy(number_p n)
{
    if(n == NULL) return NULL;

    number_p n_copy;
    n_copy = number_create_bytes32_force(n->b);
    n_copy->next = number_copy(n->next);
    return n_copy;
} 

number_p number_prune(number_p n)
{
    if(n == NULL) return n;
    if(n->next != NULL) return n;
    if(!bytes32_is_zero_bool(n->b)) return n;

    free(n);
    NUMBER_DEC();
    return NULL;
}



number_p number_add_bytes32(number_p n, bytes32_t b)
{
    if(bytes32_is_zero_bool(b)) return n;
    if(n == NULL) return number_create_bytes32(b);

    bytes32_dual_t bd = bytes32_full_add(n->b, b);
    n->b = bd.b[0];
    n->next = number_add_bytes32(n->next, bd.b[1]);
    return n;
}

number_p number_sub_bytes32(number_p n, bytes32_t b)
{
    if(bytes32_is_zero_bool(b)) return n;
    assert(n != NULL);

    if(bytes32_cmp(n->b, b) < 0) 
        n->next = number_sub_bytes32(n->next, b_one);
    n->b = bytes32_sub(n->b, b);
    return number_prune(n);
}

number_p number_mul_bytes32(number_p n, number_p n1, bytes32_t b)
{
    if(n1 == NULL) return n;

    bytes32_dual_t bd = bytes32_full_mul(n1->b, b);
    n = number_add_bytes32(n, bd.b[0]);
    if(n == NULL) n = number_create_null();
    
    n->next = number_add_bytes32(n->next, bd.b[1]);
    n->next = number_mul_bytes32(n->next, n1->next, b);
    return n;
}

int number_cmp(number_p n1, number_p n2)
{
    if(n1 == NULL) return -!!n2;
    if(n2 == NULL) return 1;

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
    
    number_p n_out = number_create_bytes32_force(b);
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
    
    number_p n_out = number_create_bytes32_force(b);
    n_out->next = number_shr_rec(n->next, u);
    return n_out;
}

number_p number_mul_rec(number_p n, number_p n1, number_p n2)
{
    if(n1 == NULL) return n;

    if(bytes32_is_zero_bool(n1->b))
    {
        if(n == NULL) n = number_create_null();
    }
    else n = number_mul_bytes32(n, n2, n1->b);

    n->next = number_mul_rec(n->next, n1->next, n2);
    return n;
}



number_p number_add(number_p n1, number_p n2)
{
    if(n1 == NULL) return number_copy(n2);
    if(n2 == NULL) return n1;

    n1 = number_add_bytes32(n1, n2->b);
    n1->next = number_add(n1->next, n2->next);
    return n1;
}

number_p number_sub(number_p n1, number_p n2)
{
    if(n2 == NULL) return n1;
    assert(n1 != NULL);

    n1 = number_sub_bytes32(n1, n2->b);
    if(n1 == NULL) return NULL;

    n1->next = number_sub(n1->next, n2->next);
    return number_prune(n1);
}

number_p number_mul(number_p n1, number_p n2)
{
    if(n2 == NULL)
    {
        number_free(n1);
        return NULL;
    }
    number_p n = number_mul_rec(NULL, n1, n2);
    number_free(n1);
    return n;
}

number_p number_shl(number_p n, uint u)
{
    assert(u < 256);
    if(n == NULL) return NULL;
    if(u == 0) return n;

    bytes32_t b = bytes32_shl_uint(n->b, u);
    number_p n_out = number_create_bytes32_force(b);
    n_out->next = number_shl_rec(n, u);
    number_free(n);

    return n_out;
}

number_p number_shr(number_p n, uint u)
{
    assert(u < 256);
    if(u == 0) return n;
    
    number_p n_out = number_shr_rec(n, u);
    number_free(n);
    return n_out;
}

number_p number_mod(number_p n1, number_p n2)
{
    if(n2 == NULL) return NULL;
    n1 = number_copy(n1);
    n2 = number_copy(n2);

    number_p n1_aux = number_shr(n1, 1);
    int loops;
    for(loops = 0; number_cmp(n2, n1_aux) >= 0; loops++)
    {
        number_p n2_aux = number_shl(n2, 1);
        number_free(n2);
        n2 = n2_aux;
    }
    number_free(n1_aux);

    for(int i=0; i<loops; i++)
    {
        if(number_cmp(n1, n2) >= 0)
        {
            n1_aux = number_sub(n1, n2);
            number_free(n1);
            n1 = n1_aux;
        }

        number_p n2_aux = number_shr(n2, 1);
        number_free(n2);
        n2 = n2_aux;
    }

    return n1;
}