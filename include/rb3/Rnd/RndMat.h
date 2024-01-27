#ifndef _RNDMAT_H
#define _RNDMAT_H

#include "rb3/Rnd/RndTex.h"
#include "rb3/Object.h"

typedef struct _Color
{
    float r;
    float g;
    float b;
    float a;
} Color;

typedef enum _BlendMode
{
    BlendDest = 0,
    BlendSrc = 1,
    BlendAdd = 2,
    BlendSrcAlpha = 3,
    BlendSubtract = 4,
    BlendMultiply = 5,
    PreMultiplyAlpha = 6
} BlendMode;

typedef struct _RndMat
{
    Object object;
    BlendMode blend;
    Color diffuseColor;
    int zMode;
    int stencilMode;
    int texGen;
    int texWrap;
    char pad2[0x40];
    ObjPtr_RndTex diffuseTex;
    char intensify;
    char useEnviron;
    char preLit;
    char alphaCut;
    char alphaWrite;
    char pad3[0x3];
    int alphaThreshold;
    ObjPtr_RndTex nextPass;
    float emissiveMultiplier;
    Color specularColor;
    Color specularColor2;
    ObjPtr_RndTex normalMap;
    ObjPtr_RndTex emissiveMap;
    ObjPtr_RndTex specularMap;
} RndMat;

RndMat *RndMatNewObject();

#endif // _RNDMAT_H