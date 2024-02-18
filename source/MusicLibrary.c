/*
    RB3Enhanced - MusicLibrary.c
    Functions for interacting with the music library.
*/

#include "rb3e_include.h"

void CheckForPanelAndJump(Symbol entryName, int sortType)
{
    // check if the song select panel is "up" (displayed on screen) before attempting a jump
    UIPanel *songSelectPanel = ObjectFindUIPanel(*(int *)PORT_OBJECTDIRMAINDIR, "song_select_panel", 1);
    if (songSelectPanel != NULL && songSelectPanel->mState == kPanelUp)
    {
        MusicLibrarySelect(*(int *)PORT_THEMUSICLIBRARY, entryName, sortType, 1);
    }
}

// selects any entry based on the full artist name or the game origin
void MusicLibrarySelectHeading(char *heading)
{
    Symbol headingname;
    SymbolConstruct(&headingname, heading);
    CheckForPanelAndJump(headingname, 2);
}
// selects any entry based on album name
void MusicLibrarySelectSubHeading(char *heading)
{
    Symbol headingname;
    SymbolConstruct(&headingname, heading);
    CheckForPanelAndJump(headingname, 3);
}
// selects a song based on its shortname
void MusicLibrarySelectSong(char *shortname)
{
    Symbol songname;
    SymbolConstruct(&songname, shortname);
    CheckForPanelAndJump(songname, 4);
}
