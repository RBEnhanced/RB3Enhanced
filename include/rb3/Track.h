#ifndef _TRACK_H
#define _TRACK_H

#include "rb3/BandUser.h"
#include "rb3/Object.h"
#include "rb3/Symbol.h"
#include "rb3/Vector.h"

typedef struct _TrackConfig
{
    int *mUser;
    char kDualPerspective;
    char pad[3];
    int mTrackNum;
    int mMaxSlots;
    int mNumSlots;
    Symbol mName;
    char mLefty;
    char pad2[3];
    unsigned int mGameCymbalLanes;
    char mDisableHopos;
    char pad3[3];
    vector mSlotSpacing;
} TrackConfig;

typedef struct _Track
{
    Object mTrackInterface;
    TrackConfig mConfig;
    float mLastRating;
    int mLastRatingState;
    char mUnk; // this is new to this structure, not sure what it does
    char pad[3];
    int mLastStreakCount;
    int mSlotIndex;
    void *mGameplayOptions;
    char mIntroPlaying;
    char pad2[3];
    float mIntroEndMs;
} Track;

#endif // _TRACK_H
