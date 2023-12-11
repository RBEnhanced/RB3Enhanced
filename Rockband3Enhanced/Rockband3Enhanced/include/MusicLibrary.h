/*
    RB3Enhanced - MusicLibrary.h
    Functions for interacting with the music library. (Not to be confused with rb3/MusicLibrary.h)
*/

// selects any entry based on the full artist name or the game origin
void MusicLibrarySelectHeading(char *heading);
// selects any entry based on album name
void MusicLibrarySelectSubHeading(char *heading);
// selects a song based on its shortname
void MusicLibrarySelectSong(char *shortname);
