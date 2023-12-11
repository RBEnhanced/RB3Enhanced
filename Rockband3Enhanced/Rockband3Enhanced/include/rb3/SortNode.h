#ifndef _SORTNODE_H
#define _SORTNODE_H

#include "SongMetadata.h"

typedef struct _Unknown2
{
#ifdef RB3E_WII
    char unknown[0xfc];
#else
    char unknown[0x108];
#endif
    SongMetadata *metaData;
} Unknown2;

typedef struct _SortNode
{
#ifdef RB3E_WII
    char something[0x34];
#else
    char something[0x40];
#endif
    Unknown2 *somethingElse;
} SortNode;

#endif // _SORTNODE_H