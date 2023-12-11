/*
    RB3Enhanced - MusicLibrary.c
    Functions for interacting with the music library.
*/

#include "../include/rb3e_include.h"

// selects any entry based on the full artist name or the game origin
void MusicLibrarySelectHeading(char *heading)
{
    Symbol headingname;
    SymbolConstruct(&headingname, heading);
    MusicLibrarySelectMaybe(*(int *)PORT_THEMUSICLIBRARY, headingname, 2, 1);
}
// selects any entry based on album name
void MusicLibrarySelectSubHeading(char *heading)
{
    Symbol headingname;
    SymbolConstruct(&headingname, heading);
    MusicLibrarySelectMaybe(*(int *)PORT_THEMUSICLIBRARY, headingname, 3, 1);
}
// selects a song based on its shortname
void MusicLibrarySelectSong(char *shortname)
{
    Symbol songname;
    SymbolConstruct(&songname, shortname);
    MusicLibrarySelectMaybe(*(int *)PORT_THEMUSICLIBRARY, songname, 4, 1);
}
