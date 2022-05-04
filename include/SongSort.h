/*
    RB3Enhanced - SongSort.h
    Song sorting patches allowing for custom song sources.
*/

#include "rb3/Symbol.h"
#include "rb3/SongSortByRecentEntry.h"

SongSortByRecentEntry * SongSortHook(int * songSortByRecent, int * unk, char * songName, char * gameOrigin, int * unk2);
Symbol * GetSymbolByGameOriginHook(Symbol * sym, int gameOrigin);