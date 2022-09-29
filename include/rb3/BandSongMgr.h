#ifndef _BANDSONGMGR_H
#define _BANDSONGMGR_H

#include "SortNode.h"

typedef struct _BandSongMgr
{
    int unk;
} BandSongMgr;

extern int GetSongIDFromShortname(BandSongMgr *thisSongMgr, char *shortName, int unknown2);
extern SongMetadata *GetMetadata(BandSongMgr *thisSongMgr, int songId);

#endif // _BANDSONGMGR_H