/*
    RB3Enhanced - SetlistHooks.h
    Hooks for modifying the setlist/song selection screen.
*/

#include "rb3/MusicLibrary.h"
#include "rb3/BandLabel.h"
#include "rb3/SortNode.h"
#include "rb3/Rnd/RndMat.h"

void SetSongAndArtistNameHook(BandLabel *label, SortNode *sortNode);
void SetSongNameFromNodeHook(BandLabel *label, SortNode *sortNode);
RndMat *MusicLibraryMatHook(MusicLibrary *thisMusicLibrary, int unk, int unk2, int *listSlot);
SongMetadata *SongMetadataConstructorHook(SongMetadata *thisSongMetadata, DataArray *data, DataArray *backupData, char isOnDisc);
char SongMetadataLoadHook(SongMetadata *thisSongMetadata, BinStream *stream);
