/*
    RB3Enhanced - SetlistHooks.h
    Hooks for modifying the setlist/song selection screen.
*/

#include "rb3/BandLabel.h"
#include "rb3/SortNode.h"

void SetSongAndArtistNameHook(BandLabel *label, SortNode *sortNode);
void SetSongNameFromNodeHook(BandLabel *label, SortNode *sortNode);
