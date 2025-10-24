#ifndef _GEMPLAYER_H
#define _GEMPLAYER_H

#include "rb3/BeatMatcher.h"

typedef struct _GemPlayer
{
    char pad[0x230]; // 0x0
    BandUser * mUser; // 0x230
    char pad1[0x14]; // 0x234
    int mTrackNum; // 0x248
    char pad2[0x128]; // 0x24C
    BeatMatcher *mMatcher; // 0x374

    // TODO: the rest
} GemPlayer;


#endif // _GEMPLAYER_H
