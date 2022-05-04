#ifndef _SORTNODE_H
#define _SORTNODE_H

// could not figure out how to do the pointer juggling to make this work from an initial pointer so had to make some structures for it
typedef struct _SongMetadata
{
#ifdef RBE3_WII
    // char unknown[0x20]; song id
    char unknown[0x2c];
#else
    char unknown[0x38];
#endif
    char *gameOrigin;
} SongMetadata;

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