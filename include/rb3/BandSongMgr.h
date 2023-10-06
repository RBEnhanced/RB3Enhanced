#ifndef _BANDSONGMGR_H
#define _BANDSONGMGR_H

#include "SortNode.h"
#include "Symbol.h"

typedef struct _BandSongMgr
{
    int unk;
} BandSongMgr;

extern int GetSongIDFromShortname(BandSongMgr *thisSongMgr, Symbol shortName, int fail); // fail seems to be unused on retail builds, but it would simulate a failure on debug builds
extern SongMetadata *GetMetadata(BandSongMgr *thisSongMgr, int songId);
extern int SongMgrGetRankedSongs(BandSongMgr *thisSongMgr, void *vector, char demosIfGameModeAllows, char includeRestricted);

#endif // _BANDSONGMGR_H
