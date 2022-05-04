#ifndef _SONGSORTBYRECENTENTRY_H
#define _SONGSORTBYRECENTENTRY_H

#include "Symbol.h"

typedef struct _SongSortByRecentEntry
{
    int * vtablePtr;
    int gameOrigin;
    int * acquireTime; // ??? game seems to check if this is 0xFFFFFFFF and if so puts it in "previously acquired", so I'm guessing this is some sort of time since acquired thing
    Symbol songName; // probably actually a symbol, need to double-check
} SongSortByRecentEntry;

extern SongSortByRecentEntry * SongSort(int * songSortByRecent, int * unk, char * songName, char * gameOrigin, int * unk2);
extern Symbol * GetSymbolByGameOrigin(Symbol * sym, int gameOrigin);

#endif // _SONGSORTBYRECENTENTRY_H