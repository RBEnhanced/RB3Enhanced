#ifndef _BANDUSER_H
#define _BANDUSER_H

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

typedef struct _BandUser
{
    float saveLevel;
    float timeToReturnFromBrink;
    float crowdLoss;
    float deployBeats;
    TrackType trackType;
    int controllerType; // TODO: - Figure out all the controller types
    float deployThreshold;
    float saveEnergy;
    int unknown; // perhaps this is individual score?
    float pointBonus;
} BandUser;

#endif // _BANDUSER_H