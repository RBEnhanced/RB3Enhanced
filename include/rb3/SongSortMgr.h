#ifndef _SONGSORTMGR_H
#define _SONGSORTMGR_H

#include "rb3/NodeSort.h"

typedef struct _SongSortMgr
{
#ifdef RB3E_XBOX
    char pad[0x4c];
#else
    char pad[0x44];
#endif
    NodeSort *mNodeSort[11]; // NodeSort *[11] in debug symbols, depends on the number of sorting types, retail might have more
} SongSortMgr;

NodeSort *SongSortMgrGetSort(SongSortMgr *thisSongSortMgr, int sortType);

#endif // _SONGSORTMGR_H
