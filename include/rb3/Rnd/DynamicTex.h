#ifndef _DYNAMIC_TEX_H
#define _DYNAMIC_TEX_H

#include "rb3/String.h"
#include "rb3/Rnd/RndMat.h"

typedef struct _DynamicTex
{
    int * vtable; // 0x0
    RndTex * mTex; // 0x4
    String mMatName; // 0x8
    RndMat* mMat; // 0x14
    void* mFileLoader; // 0x18
    char unk; // 0x1C
} DynamicTex;

void DynamicTexConstructor(DynamicTex* thisDynamicTex, const char * path, const char* matName, char createNewMat, char enableZBuffer);
void DynamicTexDestructor(DynamicTex* thisDynamicTex, int unk);



#endif // _DYNAMIC_TEX_H
