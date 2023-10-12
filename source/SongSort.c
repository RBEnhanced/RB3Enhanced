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

#define RB3E_SONGSORT_START 10
static const char *defaultOrigins[9] = {"rb1", "rb2", "rb3", "greenday", "pearljam", "lego", "rb1_dlc", "rb3_dlc", "ugc_plus"};
static const int defaultOriginCount = 9;
static Symbol origins[512] = {0};
static int newOrigins = 0;

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

int GetGameOriginBySymbolHook(Symbol gameOrigin)
{
    int i, r = 0;

    // if we don't have a game origin or if its 0, return the default
    if (gameOrigin.sym == NULL || strlen(gameOrigin.sym) == 0)
        return GetGameOriginBySymbol(gameOrigin);

    for (i = 0; i < defaultOriginCount; i++)
    {
        // if it's an existing game origin, return the default
        if (strcmp(gameOrigin.sym, defaultOrigins[i]) == 0)
            return GetGameOriginBySymbol(gameOrigin);
    }
    for (i = 0; i < newOrigins; i++)
    {
        // if we've seen this origin before, return the existing value
        if (strcmp(origins[i].sym, gameOrigin.sym) == 0)
            return i + RB3E_SONGSORT_START;
    }
    // we haven't seen this origin before, construct the new origin
    r = newOrigins + RB3E_SONGSORT_START;
    SymbolConstruct(&origins[newOrigins++], gameOrigin.sym);
    return r;
}
