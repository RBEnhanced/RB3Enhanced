/*
    RB3Enhanced - SongParserHooks.c
    Hooks related to the parsing of song MIDIs.
*/

#include <string.h>
#include <stdio.h>
#include "SongParserHooks.h"
#include "rb3/ModifierManager.h"
#include "GlobalSymbols.h"

int SongParserPitchToSlotHook(SongParser *thisSongParser, int pitch, int *difficulty, int tick)
{
    Modifier *doubleBassModifier;
    int ret;

    ret = SongParserPitchToSlot(thisSongParser, pitch, difficulty, tick);
    doubleBassModifier = ModifierActive(*(int *)PORT_MODIFIERMGR_POINTER, globalSymbols.doubleBass, 0);

    // only read the notes when double bass pedal modifier is on
    if (doubleBassModifier->enabled)
    {
        // make sure the current track type is drums
        if (thisSongParser->mTrackType == kTrackDrum)
        {
            // check for MIDI note pitch 95 (which is the 2x bass pedal note)
            if (pitch == 95)
            {
                // the original function is expected to modify the difficulty value depending on the midi note pitch (which is why it is a ptr), so do that here
                // we don't need to worry about the other difficulties since this is purely expert only. although adding support for those in the future would be easy
                if (*difficulty == 4)
                {
                    *difficulty = 3;
                    return 0;
                }
            }
        }
    }

    return ret;
}