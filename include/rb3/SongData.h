#ifndef _SONGDATA_H
#define _SONGDATA_H
#include "rb3/StdVector.h"

typedef struct _SongData
{
    char pad[0x84];
    std_vector gameGemDbs; // 0x84 std::vector<GameGemDB*>
} SongData;

typedef struct _PhraseList
{
    int placeholder;
} PhraseList;

extern PhraseList *GetPhraseList(int i, int phraseType);

#endif // _SONGDATA_H