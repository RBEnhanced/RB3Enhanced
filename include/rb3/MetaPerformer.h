#ifndef _METAPERFORMER_H
#define _METAPERFORMER_H

#include "Symbol.h"

// TODO: - Map this out as it might be useful for other things
typedef struct _MetaPerformer
{
    char unknown[0x3ac];
} MetaPerformer;

extern void SetVenue(int *thisMetaPerformer, Symbol venue);
extern void GetSongShortname(Symbol *outSym, int metaPerformer);

#endif // _METAPERFORMER_H