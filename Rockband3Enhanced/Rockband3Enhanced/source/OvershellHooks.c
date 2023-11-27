/*
    RB3Enhanced - OvershellHooks.c
    Hooks to the "overshell" and related functionality.
*/

#include "../include/rb3/BandUser.h"
#include "../include/rb3/Symbol.h"
#include "./includeGlobalSymbols.h"
#include "./include/OvershellHooks.h"
#include "../inlude/ports.h"

// MSVC inlined this function in the original game so redefine it
void AddInstrumentToList(OvershellSlot *thisOvershellSlot, Symbol *instrumentToAdd, TrackType trackType, char *icon)
{
    OvershellListEntry entry;
    entry.icon = icon[0];
    entry.trackType = trackType;
    entry.sym = instrumentToAdd->sym;
    // RB3E_DEBUG("Adding instrument %s (icon: %c, type: %i) to overshell.", entry.sym, entry.icon, entry.trackType);
    SomeVectorPushBackMaybe(&thisOvershellSlot->list_vector_maybe, &entry);
}

// Monkey patching the function will be hard, so make a realistic, dark and gritty reboot.
void BuildInstrumentSelectionList(OvershellSlot *thisOvershellSlot, ControllerType controllerType, BandUser *bandUser)
{
    PrepareSomeVectorMaybe(&thisOvershellSlot->list_vector_maybe, thisOvershellSlot->list_vector_maybe, thisOvershellSlot->unk_var_2);
    thisOvershellSlot->controllerType = controllerType;
    thisOvershellSlot->bandUser = bandUser;
    // RB3E_DEBUG("Building overshell selection for controller type %i.", controllerType);
    switch (controllerType)
    {
    case VOX_CONT:
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_vocal_solo, VOCALS, "V");
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_vocal_harmony, HARMONIES, "2");
        // allow pad to play as guitar, bass, keys and drums :D
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_guitar, GUITAR, "G");
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_bass, BASS, "B");
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_keys, KEYS, "K");
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_drums, DRUMS, "D");
        break;
    case GUITAR_CONT:
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_guitar, GUITAR, "G");
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_bass, BASS, "B");
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_keys, KEYS, "K");
        break;
    case DRUM_CONT:
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_drums, DRUMS, "D");
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_drums_pro, DRUMS, "d");
        break;
    case KEYBOARD_CONT:
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_keys, KEYS, "K");
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_real_keys, PRO_KEYS, "k");
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_guitar, GUITAR, "G");
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_bass, BASS, "B");
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_drums, DRUMS, "D");
        break;
    case PRO_GUITAR_CONT:
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_real_guitar, PRO_GUITAR, "g");
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_real_bass, PRO_BASS, "b");
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_guitar, GUITAR, "G");
        AddInstrumentToList(thisOvershellSlot, &globalSymbols.overshell_bass, BASS, "B");
        break;
    }
}
