#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "debug.h"

#include "../uint/header.h"



#ifdef DEBUG

#include "../bytes32/debug.h"

number_p number_create_null();
number_p number_create_bytes32(bytes32_t b);

number_p number_create_bytes32_dbg(bytes32_t b, int i)
{
    if(i == 0) return number_create_bytes32(b);

    number_p n = number_create_null();
    n->next = number_create_bytes32_dbg(b, i-1);
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

number_p number_bytes32_bytes32_mul(bytes32_t b1, bytes32_t b2)
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

    int j;
    number_p n2_aux, n = NULL;
    for(int i=0; n1; i++, n1 = n1->next)
    for(j=0, n2_aux = n2; n2_aux; j++, n2_aux = n2_aux->next)
    {
        number_p n_aux = number_bytes32_mul(n1->b, n2_aux->b);
        if(n_aux == NULL) continue;

        n = number_add_bytes32(n, n_aux->b, i + j);
        if(n_aux->next)
            n = number_add_bytes32(n, n_aux->next->b, i + j + 1);

        number_free(n_aux);
    }
    return n;
}
