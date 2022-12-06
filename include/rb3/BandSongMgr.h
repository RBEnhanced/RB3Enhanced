#ifndef _BANDSONGMGR_H
#define _BANDSONGMGR_H

#include "SortNode.h"

typedef struct _BandSongMgr
{
    int unk;
} BandSongMgr;

extern int GetSongIDFromShortname(BandSongMgr *thisSongMgr, char *shortName, int unknown2);
extern SongMetadata *GetMetadata(BandSongMgr *thisSongMgr, int songId);
extern int SongMgrGetRankedSongs(BandSongMgr *thisSongMgr, void *vector, char unk1, char unk2);

#endif // _BANDSONGMGR_H
