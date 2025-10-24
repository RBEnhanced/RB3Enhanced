#ifndef _BEATMATCHER_H
#define _BEATMATCHER_H

#include "rb3/SongData.h"
#include "rb3/TrackWatcher.h"

typedef struct _BeatMatcher
{
    char pad[0x28]; // 0x0
    SongData* mSongData; // 0x28
    char pad2[0x18]; // 0x2C
    TrackWatcher *mWatcher; // 0x44

    // TODO: the rest
} BeatMatcher;

#endif // _BEATMATCHER_H
