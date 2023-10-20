#ifndef _UISCREEN_H
#define _UISCREEN_H

#include "../Symbol.h"

typedef struct _UIScreen
{
    int *vtable;        // 0x0
    char unk[0x14];     // 0x4 - prboably different on Wii
    Symbol screen_name; // 0x18
} UIScreen;

#endif // _UISCREEN_H
