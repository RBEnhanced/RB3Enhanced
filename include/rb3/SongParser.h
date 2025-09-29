#ifndef _SONGPARSER_H
#define _SONGPARSER_H

#include "Symbol.h"
#include "BandUser.h"

// TODO: fill out the rest of this, there is a lot of useful stuff in this class that could be useful for controlling how songs load
typedef struct _SongParser
{
#ifdef RB3E_WII
    char pad[0xdc];
#else
    char pad[0xf8];
#endif
    TrackType mTrackType;
} SongParser;

extern int SongParserPitchToSlot(SongParser *thisSongParser, int pitch, int *difficulty, int tick);

#endif // _SONGPARSER_H
