#ifndef _BANDCHARDESC_H
#define _BANDCHARDESC_H

#include "Symbol.h"
#include "String.h"

// 0x58 in size
typedef struct _Head
{
    void * vtable; // 0x00  
    int pad;       // 0x04
    char hideHead; // 0x08
    char pad2[0x4F]; // 0x09
} Head;

typedef struct _OutfitPiece
{
    void * vtable;
    int pad;
    Symbol mName;
    int unk1;
    int unk2;
    int unk3;
} OutfitPiece;

typedef struct _Outfit
{
    void * vtable;
    unsigned int pad;
    OutfitPiece parts[12];
} Outfit;

typedef struct _BandCharDesc
{
    void * vtable;
    char pad_0004[0x8];
    Symbol prefab;
    Symbol gender;
    unsigned int skinColorIdx;
    Head head;
    Outfit outfit;
} BandCharDesc;

extern void MakeOutfitPath(BandCharDesc *thisBandCharDesc, Symbol part, String *filePath);
extern BandCharDesc *BandCharDescNewObject();
extern void ReloadPrefabs();

#endif // _BANDCHARDESC_H
