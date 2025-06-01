#ifndef _METAPERFORMER_H
#define _METAPERFORMER_H

#include "Symbol.h"

// TODO: - Map this out as it might be useful for other things
typedef struct _MetaPerformer
{
    char unknown[0x3ac];
} MetaPerformer;

extern void SetVenue(int *thisMetaPerformer, Symbol venue);
#ifdef RB3E_XBOX
extern void GetSongShortname(Symbol *symOut, int metaPerformer);
#else
// this calling convention makes more sense? wtf microsoft
extern Symbol GetSongShortname(int metaPerformer);
#endif

#endif // _METAPERFORMER_H