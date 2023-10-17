/*
    RB3Enhanced - SongHooks.h
    Hooks related to loading songs into the game.
*/

#include "rb3/Data.h"

int MetadataSongIDHook(DataNode *song_id);
int GetSongIDHook(DataArray *song, DataArray *missing_data_maybe);
SongMetadata *InitSongMetadataHook(SongMetadata *data);
