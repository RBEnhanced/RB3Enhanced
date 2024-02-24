#ifndef _TRACKWATCHER_H
#define _TRACKWATCHER_H

#include "rb3/Data.h"
#include "rb3/Vector.h"

typedef struct _TrackWatcher
{
    void *mImpl;
    vector mSinks;
    int mTrack;
    char mIndependentSlots;
    char pad[3];
    int mUserGuid[4];
    void *mPlayerSlot;
    Symbol mControllerType;
    void *mSongData;
    void *mTrackWatcherParent;
    DataArray *mCfg;
} TrackWatcher;

#endif // _TRACKWATCHER_H