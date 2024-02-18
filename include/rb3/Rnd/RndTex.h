#ifndef _RNDTEX_H
#define _RNDTEX_H

#include "rb3/Object.h"

typedef enum _BitmapType
{
    RGBA = 3,
    DXT1 = 8,
    DXT5 = 24,
    ATI2 = 32,
    TPL_CMP = 72,
    TPL_CMP_ALPHA = 328,
} BitmapType;

typedef struct _RndBitmap
{
    char pad[0x1c];

} RndBitmap;

typedef struct _RndTex
{
    Object object;
    RndBitmap bitmap;
    float mipMapK;
    int type;
    int width;
    int height;
    int bpp;
    String name;
    int mipLevels;
    int *fileLoader;
    int unk2;
} RndTex;

typedef struct _ObjPtr_RndTex
{
    int *vtable;
    Object *object;
    RndTex *tex;
} ObjPtr_RndTex;

RndTex *RndTexNewObject();
void RndTexSetBitmap(RndTex *thisRndTex, FilePath *path);
void RndTexSetBitmap2(RndTex *thisRndTex, int width, int height, int bpp, int type, int useMips, char *path);

void RndBitmapGenerateMips(RndBitmap *thisBitmap);
int RndBitmapNumMips(RndBitmap *thisBitmap);

#endif // _RNDTEX_H