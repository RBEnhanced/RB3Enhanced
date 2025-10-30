#ifndef _INSTRUMENTDUPINGHOOKS_H
#define _INSTRUMENTDUPINGHOOKS_H

#include "rb3/GameGemDB.h"



extern void RecalcGemList(void* thisTrackWatcherImpl);


extern void PostLoad(void* thisGemPlayer, char b);
extern void GemPlayerDestructor(void* thisGemPlayer);

extern Object* GetFreeSmasherPlate(void *thisGemTrackResourceManager, int inst);

extern void ReleaseSmasherPlate(void *thisGemTrackResourceManager, Object* plate);

#endif // _INSTRUMENTDUPINGHOOKS_H
