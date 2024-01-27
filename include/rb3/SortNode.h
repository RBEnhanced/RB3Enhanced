#ifndef _SORTNODE_H
#define _SORTNODE_H

#include "Object.h"
#include "Symbol.h"
#include "SongMetadata.h"

typedef struct _SortNode SortNode;

typedef int (*ReturnsZero_t)();
typedef void (*OnlyReturns_t)();
typedef int (*GetNodeType_t)();
typedef Symbol (*GetToken_t)();
typedef int (*GetSongCount_t)(SortNode *thisSortNode);
typedef Symbol (*OnSelect_t)(SortNode *thisSortNode);
typedef Symbol (*Select_t)(SortNode *thisSortNode);

typedef struct _SortNodeVtable
{
    Object_vtable objectVtable;
    GetNodeType_t getNodeType;
    GetToken_t getToken;
    GetSongCount_t getSongCount;
    ReturnsZero_t returnsZero1;
    OnlyReturns_t onlyReturns1;
    OnSelect_t onSelect;
    Select_t select;
    OnlyReturns_t onlyReturns2;
    // there is more here but not really relevant atm

} SortNodeVtable;

typedef struct _SongRecord
{
    int *vtable;
#ifdef RB3E_WII
    char unknown[0xfc];
#else
    char unknown[0x104];
#endif
    SongMetadata *metaData;
} SongRecord;

struct _SortNode
{
    SortNodeVtable *vtable;
#ifdef RB3E_WII
    char something[0x30];
#else
    char something[0x3c];
#endif
    SongRecord *record;
};

#endif // _SORTNODE_H