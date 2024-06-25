/*
    RB3Enhanced - SongParserHooks.h
    Hooks related to the parsing of song MIDIs.
*/

#include "rb3/SongParser.h"
#include "ports.h"

int SongParserPitchToSlotHook(SongParser *thisSongParser, int pitch, int *difficulty, int tick);