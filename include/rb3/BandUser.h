#ifndef _BANDUSER_H
#define _BANDUSER_H

#include "rb3/String.h"
#include "rb3/Symbol.h"
#include "rb3/Track.h"

typedef enum _Difficulty
{
    kDifficultyEasy = 0,
    kDifficultyMedium = 1,
    kDifficultyHard = 2,
    kDifficultyExpert = 3
} Difficulty;

typedef enum _TrackType
{
    kTrackDrum = 0,
    kTrackGuitar = 1,
    kTrackBass = 2,
    kTrackVocals = 3,
    kTrackKeys = 4,
    kTrackRealKeys = 5,
    kTrackRealGuitar = 6,
    kTrackRealGuitar22Fret = 7,
    kTrackRealBass = 8,
    kTrackRealBass22Fret = 9,
    kTrackNone = 10,
    kTrackPending = 11,
    kTrackPendingVocals = 12
} TrackType;

typedef enum _ControllerType
{
    kControllerDrum = 0,
    kControllerGuitar = 1,
    kControllerVocals = 2,
    kControllerKeys = 3,
    kControllerRealGuitar = 4,
    kControllerNone = 5
} ControllerType;

typedef enum _PreferredScoreType
{
    kScoreDrum = 0,
    kScoreBass = 1,
    kScoreGuitar = 2,
    kScoreVocals = 3,
    kScoreHarmony = 4,
    kScoreKeys = 5,
    kScoreRealDrum = 6,
    kScoreRealGuitar = 7,
    kScoreRealBass = 8,
    kScoreRealKeys = 9,
    kScoreBand = 10
} PreferredScoreType;

typedef struct _BandUser
{
    // BandUser members
    int unknown_0x0;
    int unknown_0x4;
    Difficulty difficulty;
    char mUnknown;
    char pad[3];
    TrackType trackType;
    ControllerType controllerType;
    char mTrackSelected;
    char mUnknown2;
    char pad2[2];
    PreferredScoreType mPreferredScoreType;
    int mOvershellState;
    String mOvershellFocus;
    void *mChar;
#ifdef RB3E_XBOX
    char mGameplayOptions[0x48];
#else
    char mGameplayOptions[0x38];
#endif
    char mAutoplay;
    char pad3[3];
#ifdef RB3E_WII
    int mUnknownBool;
#endif
    Symbol mPreviousAward;
    float mLastHitFraction;
    Track *mTrack;
    void *mPlayer;
    char mParticipating;
#ifdef RB3E_XBOX
    char pad4[3];
    int mUnknown3;
#else
    char mIsWiiRemoteController;
    char mJustDisconnected;
    char pad4;
#endif

    // User members
    // TODO
} BandUser;

#endif // _BANDUSER_H