#ifndef _GAMEGEMDB_H
#define _GAMEGEMDB_H

#include "rb3/StdVector.h"

typedef struct _GameGemDB
{
    std_vector mGameGemLists;
    int mHopoThreshold;
} GameGemDB;

extern GameGemDB* Duplicate(GameGemDB* thisGameGemDB);
extern void* GetDiffGemList(GameGemDB* thisGameGemDB, int diff);

#endif // _GAMEGEMDB_H