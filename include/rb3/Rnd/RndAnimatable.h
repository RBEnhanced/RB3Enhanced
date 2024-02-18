#ifndef _RNDANIMATABLE_H
#define _RNDANIMATABLE_H

#include "rb3/Object.h"

typedef struct _RndAnimatable
{
    void *vtable;
    void *objectVtable;
    float mFrame;
    int mRate;
} RndAnimatable;

#endif // _RNDANIMATABLE_H