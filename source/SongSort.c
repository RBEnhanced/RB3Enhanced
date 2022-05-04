/*
    RB3Enhanced - SongSort.c
    Song sorting patches allowing for custom song sources.
*/

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "ports.h"
#include "rb3/Symbol.h"
#include "rb3/SongSortByRecentEntry.h"

#define RB3E_SONGSORT_START 20
const char *defaultOrigins[11] = {"rb1", "rb2", "rb3", "rb1_dlc", "rb2_dlc", "rb3_dlc", "greenday", "pearljam", "lego", "ugc", "ugc_plus"};
Symbol origins[256] = {0};
int newOrigins = 0;

SongSortByRecentEntry *SongSortHook(int *songSortByRecent, int *unk, char *songName, char *gameOrigin, int *unk2)
{
    int i = 0;
    SongSortByRecentEntry *entry = SongSort(songSortByRecent, unk, songName, gameOrigin, unk2);

    if (!gameOrigin) // if there is no origin, return the default
        return entry;
    for (i = 0; i < 11; i++)
    { // if it's an existing game origin, return the default
        if (strcmp(gameOrigin, defaultOrigins[i]) == 0)
            return entry;
    }

    // it is not an existing song origin, it is custom
    RB3E_DEBUG("Non-standard origin detected: %s", gameOrigin);
    for (i = 0; i < newOrigins; i++)
    {
        if (strcmp(origins[i].sym, gameOrigin) == 0)
        {
            // we've seen this origin before, return the default value
            entry->gameOrigin = i + RB3E_SONGSORT_START;
            return entry;
        }
    }
    // we haven't seen this origin before, construct the origin
    entry->gameOrigin = newOrigins + RB3E_SONGSORT_START;
    SymbolConstruct(&origins[newOrigins++], gameOrigin);
    return entry;
}

Symbol *GetSymbolByGameOriginHook(Symbol *sym, int gameOrigin)
{
    if (gameOrigin >= RB3E_SONGSORT_START)
    {
        // the original func is updating the symbol provided in r3 so do the same here
        sym->sym = origins[gameOrigin - RB3E_SONGSORT_START].sym;
        return sym;
    }
    return GetSymbolByGameOrigin(sym, gameOrigin);
}