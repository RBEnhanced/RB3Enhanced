#ifndef _SONGSORTBYRECENTENTRY_H
#define _SONGSORTBYRECENTENTRY_H

#include "Symbol.h"

typedef struct _SongSortByRecentEntry
{
    int *vtablePtr;
    int gameOrigin;
    int *acquireTime; // ??? game seems to check if this is 0xFFFFFFFF and if so puts it in "previously acquired", so I'm guessing this is some sort of time since acquired thing
    Symbol songName;  // probably actually a symbol, need to double-check
} SongSortByRecentEntry;

extern SongSortByRecentEntry *SongSortByRecent(int *songSortByRecent, int *dateAcquired, char *songName, Symbol gameOrigin, int *unk2);
extern Symbol *GetSymbolByGameOrigin(Symbol *sym, int gameOrigin);
extern int GetGameOriginBySymbol(Symbol sym);

#endif // _SONGSORTBYRECENTENTRY_H
