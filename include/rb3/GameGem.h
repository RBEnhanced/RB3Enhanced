#ifndef _GAMEGEM_H
#define _GAMEGEM_H

#include "rb3/Symbol.h"
#include "rb3/Vector.h"

typedef struct _GameGem
{
    float milliSeconds;
    int tick;
    short durationMs;
    short durationTicks;

    unsigned char unkBitfield1;
    unsigned char unkBitfield2;
    unsigned char unkBitfield3;

    char unk1 : 1;
    char unk2 : 1;
    char unk3 : 1;

    // There is likely a lot more to this for Pro Keys/Guitar/Bass/Drums in the previous bitfields, but this works fine for 5-lane instruments
    char orange : 1;
    char blue : 1;
    char yellow : 1;
    char red : 1;
    char green : 1;

    char isPlayed : 1; // Updated in real-time once the player has seen this gem and either hit or missed it
    char isHopo : 1;   // Does nothing on the first note of the song even if set
    char ignoreDuration : 1;
    char isCymbal : 1;
    char showChordNames : 1;
    char showSlashes : 1;
    char useAltDuration : 1;
    char autoPlayable : 1;
    unsigned char playableBy;
    char isRealGuitar;

    // There is more after this that I am too lazy to map. Probably pro instrument stuff
} GameGem;

typedef enum _NoStrumState
{
    kStrumForceOn = 0,
    kStrumForceOff = 1,
    kStrumDefault = 2
} NoStrumState;

typedef struct _GameGemList
{
    int mHopoThreshold;
    vector mGems;
} GameGemList;

extern int WillBeNoStrum(GameGemList *thisGameGemList, int *multiGemInfoPtr);
extern int AddGameGem(GameGemList *gameGemList, GameGem *gem, NoStrumState gemType);
extern int *GetWidgetByName(int *gemManager, Symbol sym);
extern Symbol GetSlotColor(int *bandUser, int slot);

#endif // _GAMEGEM_H