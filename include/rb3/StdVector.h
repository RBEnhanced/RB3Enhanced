#ifndef _STDVECTOR_H
#define _STDVECTOR_H

typedef struct _std_vector
{
    // structure of a vector differs on 360 vs Wii
#ifdef RB3E_XBOX
    void *begin;
    void *end;
    void *capacity;
#else
    int *data;
    short count;
    short capacity;
#endif
} std_vector;

#endif // _STDVECTOR_H
