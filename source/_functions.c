/*
    RB3Enhanced - _functions.c
    Definitions used by the compiler to call/hook functions.
    This file should not be included by any other file, to avoid conflicts.
*/

#include "ports.h"
#include "ppcasm.h"
// get line number
#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)
// define small stub using line number to avoid optimisations
#ifdef RB3E_XBOX // for xbox, use MSVC notation
#define RB3E_STUB(x)           \
    __declspec(naked) void x() \
    {                          \
        __asm { li r3, __LINE__ }     \
    }
#else // other platforms should use GCC notation
#define RB3E_STUB(x) \
    void x() { asm("li 0, " LINE_STRING ";"); }
#endif

// function stub definitions
#ifndef RB3E_WII // AppConstructor is handled by the BrainSlug engine
RB3E_STUB(AppConstructor)
#endif
RB3E_STUB(ExecuteDTA)
RB3E_STUB(SymbolConstruct)
RB3E_STUB(ModifierActive)
RB3E_STUB(HmxFactoryFuncAt)
RB3E_STUB(BandLabelSetDisplayText)
RB3E_STUB(RandomInt)
RB3E_STUB(DataNodeEvaluate)
RB3E_STUB(FileExists)
RB3E_STUB(QueueMessage)
// hooked function stubs
RB3E_STUB(Localize)
RB3E_STUB(SetVenue)
RB3E_STUB(ModifierManagerConstructor)
RB3E_STUB(NewFile)
RB3E_STUB(SongSort)
RB3E_STUB(GetSymbolByGameOrigin)
RB3E_STUB(WillBeNoStrum)
RB3E_STUB(AddGameGem)
RB3E_STUB(SetSongAndArtistName)
RB3E_STUB(SetMusicSpeed)
RB3E_STUB(UpdateTrackSpeed)
RB3E_STUB(SetAddress)
RB3E_STUB(GetWidgetByName)
RB3E_STUB(GetSlotColor)
RB3E_STUB(UsbWiiGetType)
RB3E_STUB(SetSystemLanguage)
