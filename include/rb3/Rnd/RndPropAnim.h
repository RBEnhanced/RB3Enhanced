#ifndef _RNDPROPANIM_H
#define _RNDPROPANIM_H

#include "rb3/Object.h"

typedef struct _RndPropAnim
{
    int *vtable; // 0x0
    int unk;     // 0x04
    int unk2;    // 0x08
    int unk3;    // 0x0c
    int unk4;    // 0x10
    int unk5;    // 0x14
    int unk6;    // 0x18
    int unk7;    // 0x1c
#ifdef RB3E_XBOX
    int unk8;
#endif
    Object object; // 0x20

} RndPropAnim;

void PropAnimSetFrame(RndPropAnim *rndPropAnim, float frame, float time);

#endif // _RNDPROPANIM_H