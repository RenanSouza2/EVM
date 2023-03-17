#include <string.h>

#include "debug.h"

#ifdef DEBUG

#include <stdio.h>

void bytes_n_display(int scalar, const uint b[scalar])
{
    printf("0x");
    for(int i=scalar-1; i>=0; i--) 
        printf("%08x", b[i]);
}

#endif



int bytes_n_cmp_uint(int scalar, const uint b[scalar], uint u)
{
    for(int i=scalar-1; i>0; i--)
        if(b[i])
            return 1;

    if(b[0] > u) return  1;
    if(b[0] < u) return -1;
    return 0;
}

void bytes_n_add_uint(int scalar, uint b[scalar], uint u, int i)
{
    if(u == 0) return;
    if(i >= scalar) return;

    luint lu = uint_add(b[i], u);
    b[i] = DECL(lu);

    return bytes_n_add_uint(scalar, b, DECH(lu), i+1);
}

void bytes_n_shl_uint(int scalar, uint b[scalar], uint shift)
{
    if(shift >= scalar << 5)
    {
        BYTES_N_RESET(scalar, b);
        return;
    }

    if(shift == 0) return;
    
    int jmp, off;
    jmp = shift >> 5;
    off = shift & 31;
    
    uint b_out[scalar];
    BYTES_N_RESET(scalar, b_out);
    if(off == 0)
    {
        memcpy(&b_out[jmp], b, (scalar-jmp) << 2);
        memcpy(b, b_out, scalar << 2);
        return;
    }
    
    b_out[jmp] = b[0] << off;
    for(int i=1; i+jmp<scalar; i++)
        b_out[i+jmp] = (b[i-1] >> (32 - off)) | (b[i] << off);

    BYTES_N_SET(scalar, b, b_out);
    return;
}

void bytes_n_shr_uint(int scalar, uint b[scalar], uint shift)
{
    if(shift >= scalar << 5)
    {
        BYTES_N_RESET(scalar, b);
        return;
    }

    int jmp = shift >> 5;
    int off = shift & 31;

    uint b_out[scalar];
    BYTES_N_RESET(scalar, b_out);
    if(off == 0)
    {
        memcpy(b_out, &b[jmp], (scalar-jmp) << 2);
        memcpy(b, b_out, scalar << 2);
        return;
    }

    b_out[scalar-1-jmp] = b[scalar-1] >> off;
    for(int i=0; i+jmp+1<scalar; i++)
    {
        b_out[i] =  (b[jmp+i+1] << (32 - off)) | (b[jmp+i] >> off);
    }

    BYTES_N_SET(scalar, b, b_out);
    return;
}



bool bytes_n_is_zero(int scalar, const uint b[scalar])
{
    for(int i=0; i<scalar; i++)
        if(b[i])
            return false;

    return true;
}

int bytes_n_cmp(int scalar, const uint b1[scalar], const uint b2[scalar])
{
    for(int i=scalar-1; i>=0; i--) 
    {
        if(b1[i] > b2[i]) return  1; 
        if(b1[i] < b2[i]) return -1; 
    }
    return 0;
}



void bytes_n_not(int scalar, uint b[scalar])
{
    for(int i=0; i<scalar; i++)
        b[i] = ~b[i];
}

void bytes_n_minus(int scalar, uint b[scalar])
{
    bytes_n_not(scalar, b);
    bytes_n_add_uint(scalar, b, 1, 0);
}



void bytes_n_add(int scalar, uint b1[scalar], const uint b2[scalar])
{
    for(int i=0; i<scalar; i++)
        bytes_n_add_uint(scalar, b1, b2[i], i);
}

void bytes_n_sub(int scalar, uint b1[scalar], const uint b2[scalar])
{
    uint b2_aux[scalar];
    BYTES_N_SET(scalar, b2_aux, b2);
    bytes_n_minus(scalar, b2_aux);
    bytes_n_add(scalar, b1, b2_aux);
}

void bytes_n_mul(int scalar, uint b1[scalar], const uint b2[scalar])
{
    uint b[scalar];
    BYTES_N_RESET(scalar, b);

    for(int i=0; i<scalar; i++)
    {
        if(b1[i] == 0) continue;
        
        for(int j=0; j+i<scalar; j++)
        {
            if(b2[j] == 0) continue;

            luint lu = uint_mul(b1[i], b2[j]);
            bytes_n_add_uint(scalar, b, DECL(lu), i+j);
            bytes_n_add_uint(scalar, b, DECH(lu), i+j+1);
        }
    }
    
    BYTES_N_SET(scalar, b1, b);
}

void bytes_n_div_mod(int scalar, uint b1[scalar], uint b2[scalar])
{
    if(bytes_n_is_zero(scalar, b2))
    {
        BYTES_N_RESET(scalar, b1);
        BYTES_N_RESET(scalar, b2);
        return;
    }
    
    uint b1_aux[scalar];
    BYTES_N_SET(scalar, b1_aux, b1);
    bytes_n_shr_uint(scalar, b1_aux, 1);

    uint b_base[scalar];
    BYTES_N_SET_FIRST(scalar, b_base, 1);

    while(bytes_n_cmp(scalar, b1_aux, b2) >= 0)
    {
        bytes_n_shl_uint(scalar, b2, 1);
        bytes_n_shl_uint(scalar, b_base, 1);
    }
    
    uint b_out[scalar];
    BYTES_N_RESET(scalar, b_out);
    while(!bytes_n_is_zero(scalar, b_base))
    {
        if(bytes_n_cmp(scalar, b1, b2) >= 0)
        {
            bytes_n_sub(scalar, b1, b2);
            bytes_n_add(scalar, b_out, b_base);
        }
        
        bytes_n_shr_uint(scalar, b2, 1);
        bytes_n_shr_uint(scalar, b_base, 1);
    }
    
    BYTES_N_SET(scalar, b2, b_out);
    return;
}
