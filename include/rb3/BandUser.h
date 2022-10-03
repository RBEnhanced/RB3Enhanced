#ifndef _BANDUSER_H
#define _BANDUSER_H

typedef enum _Difficulty
{
    EASY = 0,
    MEDIUM = 1,
    HARD = 2,
    EXPERT = 3
} Difficulty;

typedef enum _TrackType
{
    BASS = 2,
    DRUMS = 0,
    GUITAR = 1,
    HARMONIES = 7,
    KEYS = 4,
    PRO_BASS = 8,
    PRO_GUITAR = 6,
    PRO_KEYS = 5,
    VOCALS = 3
} TrackType;

typedef enum _ControllerType
{
    DRUM_CONT = 0,
    GUITAR_CONT = 1,
    VOX_CONT = 2,
    KEYBOARD_CONT = 3,
    PRO_GUITAR_CONT = 4,
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