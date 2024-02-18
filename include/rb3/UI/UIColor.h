#ifndef _UICOLOR_H
#define _UICOLOR_H

#include "rb3/Object.h"

typedef struct _UIColor
{
    Object Object;
    float mRed;
    float mGreen;
    float mBlue;
    float mAlpha;
} UIColor;

typedef struct _ObjPtr_UIColor
{
    int *vtable;
    Object *object;
    UIColor *color;
} ObjPtr_UIColor;

#endif