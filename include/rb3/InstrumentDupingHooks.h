#ifndef _INSTRUMENTDUPINGHOOKS_H
#define _INSTRUMENTDUPINGHOOKS_H

#include "rb3/GameGemDB.h"



extern void RecalcGemList(void* thisTrackWatcherImpl);


extern void PostLoad(void* thisGemPlayer, char b);
extern void GemPlayerDestructor(void* thisGemPlayer);

#endif // _INSTRUMENTDUPINGHOOKS_H
