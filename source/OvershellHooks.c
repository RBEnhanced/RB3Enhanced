/*
    RB3Enhanced - OvershellHooks.c
    Hooks to the "overshell" and related functionality.
*/

#include "rb3/BandUser.h"
#include "rb3/Symbol.h"
#include "GlobalSymbols.h"
#include "OvershellHooks.h"
#include "ports.h"

// MSVC inlined this function in the original game so redefine it
void OvershellPartSelectProviderAddPart(OvershellSlot *thisOvershellSlot, Symbol *instrumentToAdd, TrackType trackType, char *icon)
{
    OvershellListEntry entry;
    entry.icon = icon[0];
    entry.trackType = trackType;
    entry.sym = instrumentToAdd->sym;
    // RB3E_DEBUG("Adding instrument %s (icon: %c, type: %i) to overshell.", entry.sym, entry.icon, entry.trackType);
    SomeVectorPushBackMaybe(&thisOvershellSlot->list_vector_maybe, &entry);
}

// Monkey patching the function will be hard, so make a realistic, dark and gritty reboot.
void OvershellPartSelectProviderReload(OvershellSlot *thisOvershellSlot, ControllerType controllerType, BandUser *bandUser)
{
    PrepareSomeVectorMaybe(&thisOvershellSlot->list_vector_maybe, thisOvershellSlot->list_vector_maybe, thisOvershellSlot->unk_var_2);
    thisOvershellSlot->controllerType = controllerType;
    thisOvershellSlot->bandUser = bandUser;
    // RB3E_DEBUG("Building overshell selection for controller type %i.", controllerType);
    switch (controllerType)
    {
    case kControllerVocals:
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_vocal_solo, kTrackVocals, "V");
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_vocal_harmony, kTrackVocals, "2");
        // allow pad to play as guitar, bass, keys and drums :D
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_guitar, kTrackGuitar, "G");
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_bass, kTrackBass, "B");
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_keys, kTrackKeys, "K");
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_drums, kTrackDrum, "D");
        break;
    case kControllerGuitar:
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_guitar, kTrackGuitar, "G");
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_bass, kTrackBass, "B");
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_keys, kTrackKeys, "K");
        break;
    case kControllerDrum:
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_drums, kTrackDrum, "D");
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_drums_pro, kTrackDrum, "d");
        break;
    case kControllerKeys:
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_keys, kTrackKeys, "K");
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_real_keys, kTrackRealKeys, "k");
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_guitar, kTrackGuitar, "G");
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_bass, kTrackBass, "B");
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_drums, kTrackDrum, "D");
        break;
    case kControllerRealGuitar:
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_real_guitar, kTrackRealGuitar, "g");
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_real_bass, kTrackRealBass, "b");
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_guitar, kTrackGuitar, "G");
        OvershellPartSelectProviderAddPart(thisOvershellSlot, &globalSymbols.overshell_bass, kTrackBass, "B");
        break;
    }
}
