#ifndef _GAMEGEM_H
#define _GAMEGEM_H

#include "Symbol.h"

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

    char gemSeen : 1;     // Updated in real-time once the player has seen this gem and either hit or missed it
    char isHopo : 1;      // Does nothing on the first note of the song even if set
    char normalNote1 : 1; // Unsure what these both represent but sustains have them set to 0 and setting them to 0 forces a sustain
    char normalNote2 : 1;
    char unk5 : 1;
    char unk6 : 1;
    char unk7 : 1;
    char unk8 : 1;

    // Unsure about these two
    unsigned char unknown;
    int unknown2;

    // There is more after this that I am too lazy to map. Probably pro instrument stuff
} GameGem;

typedef enum _NoStrumState
{
    kStrumForceOn = 0,
    kStrumForceOff = 1,
    kStrumDefault = 2
} NoStrumState;

extern int WillBeNoStrum(int *gameGemListPtr, int *multiGemInfoPtr);
extern int AddGameGem(int *gameGemList, GameGem *gem, NoStrumState gemType);
extern int *GetWidgetByName(int *gemManager, Symbol sym);
extern Symbol GetSlotColor(int *bandUser);

#endif // _GAMEGEM_H