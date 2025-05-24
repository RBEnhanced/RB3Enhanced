#ifndef _SORTNODE_H
#define _SORTNODE_H

#include "Object.h"
#include "Symbol.h"
#include "SongMetadata.h"

typedef enum _SongNodeType
{
    kNodeNone = 0,
    kNodeShortcut = 1,
    kNodeHeader = 2,
    kNodeSubheader = 3,
    kNodeSong = 4,
    kNodeFunction = 5,
    kNodeSetlist = 6,
    kNodeStoreSong = 7
} SongNodeType;

typedef struct _Unknown2
{
#ifdef RB3E_WII
    char unknown[0xfc];
#else
    char unknown[0x108];
#endif
    SongMetadata *metaData;
} Unknown2;

typedef int (*ReturnsZero_t)();
typedef void (*OnlyReturns_t)();
typedef SongNodeType (*GetNodeType_t)();
typedef Symbol (*GetToken_t)();
typedef int (*LocalizeToken_t)(SortNode *thisSortNode);
typedef Symbol (*OnSelect_t)(SortNode *thisSortNode);
typedef Symbol (*Select_t)(SortNode *thisSortNode);
typedef int *(*GetDateTime_t)(SortNode *thisSortNode);

typedef struct _ShortcutNodeVtable
{
    Object_vtable objectVtable;
    GetNodeType_t getNodeType;
    GetToken_t getToken;
    LocalizeToken_t localizeToken;
    GetDateTime_t getDateTime;
    OnlyReturns_t deleteAll;
    OnSelect_t onSelect;
    Select_t select;
    OnlyReturns_t onlyReturns2;
    // there is more here but not really relevant atm

} ShortcutNodeVtable;

typedef struct _SongRecord
{
    int *vtable;
#ifdef RB3E_WII
    char unknown[0xf8];
#else
    char unknown[0x104];
#endif
    SongMetadata *metaData;
} SongRecord;

struct _SortNode
{
    ShortcutNodeVtable *vtable;
#ifdef RB3E_WII
    char something[0x30];
#else
    char something[0x3c];
#endif
    SongRecord *record;
};

#endif // _SORTNODE_H