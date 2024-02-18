#ifndef _BANDUSER_H
#define _BANDUSER_H

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

typedef struct _BandUser
{
    int unknown_0x0;
    int unknown_0x4;
    Difficulty difficulty;
    int unknown_0xc;
    TrackType trackType;
    ControllerType controllerType;
    int unknown_0x14;
    int unknown_0x18;
    int unknown_0x1c;
    int unknown_0x20;
} BandUser;

#endif // _BANDUSER_H