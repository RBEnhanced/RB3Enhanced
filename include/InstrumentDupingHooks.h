#include "rb3/GemPlayer.h"
#include "rb3/SongData.h"


extern void PostLoadHook(GemPlayer* thisGemPlayer, char b);
extern void GemPlayerDestructorHook(GemPlayer* thisGemPlayer, int unknown);
extern Object* GetFreeSmasherPlateHook(void *thisGemTrackResourceManager, int inst);

extern PhraseList *GetPhraseListHook(int i, int phraseType);

extern void ReleaseSmasherPlateHook(void *thisGemTrackResourceManager, Object* plate);
