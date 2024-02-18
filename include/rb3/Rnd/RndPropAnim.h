#ifndef _RNDPROPANIM_H
#define _RNDPROPANIM_H

#include "rb3/List.h"
#include "rb3/Object.h"
#include "rb3/Rnd/RndAnimatable.h"

typedef struct _RndPropAnim
{
    RndAnimatable mAnimatable;
    list mPropKeys;
    float mLastFrame;
    bool mInSetFrame;
    bool unk;
    char pad[2];
#ifdef RB3E_XBOX
    int unk2;
#endif
    Object mObject;
} RndPropAnim;

void PropAnimSetFrame(RndPropAnim *rndPropAnim, float frame, float time);

#endif // _RNDPROPANIM_H