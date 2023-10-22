#ifndef _UISCREEN_H
#define _UISCREEN_H

#include "../Symbol.h"

// Hmx::Object?
typedef struct _UIScreen
{
    int *vtable;
#ifdef RB3E_XBOX
    char unk[0x14];
#elif RB3E_WII
    char unk[0x8];
#endif
    Symbol screen_name; // referenced at the start of Hmx::Object::FindPathName?
} UIScreen;

#endif // _UISCREEN_H
