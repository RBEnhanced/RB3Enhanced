#ifndef _RNDMAT_H
#define _RNDMAT_H

#include "rb3/Rnd/RndTex.h"
#include "rb3/Rnd/Transform.h"
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
    kDest = 0,
    kSrc = 1,
    kAdd = 2,
    kSrcAlpha = 3,
    kSrcAlphaAdd = 4,
    kSubtract = 5,
    kMultiply = 6,
    kPreMultAlpha = 7
} BlendMode;

typedef enum _ZMode
{
    kDisable = 0,
    kNormal = 1,
    kTransparent = 2,
    kForce = 3,
    kDecal = 4
} ZMode;

typedef enum _StencilMode
{
    kStencilIgnore = 0,
    kStencilWrite = 1,
    kStencilTest = 2
} StencilMode;

typedef enum _TexGen
{
    kNone = 0,
    kXfm = 1,
    kSphere = 2,
    kProjected = 3,
    kXfmOrigin = 4,
    kEnviron = 5
} TexGen;

typedef enum _TexWrap
{
    kClamp = 0,
    kRepear = 1,
    kBorderBlack = 2,
    kBorderWhite = 3,
    kMirror = 4
} TexWrap;

typedef struct _RndMat
{
    Object object;
    BlendMode blend;
    Color diffuseColor;
    ZMode zMode;
    StencilMode stencilMode;
    TexGen texGen;
    TexWrap texWrap;
    Transform texXfm;
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
RndMat* RndMatSetDiffuseTex(RndMat *thisRndMat, RndTex *tex);

#endif // _RNDMAT_H