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
        __asm { li r3, __LINE__ }      \
    }
#else // other platforms should use GCC notation
#define RB3E_STUB(x)                   \
    void x()                           \
    {                                  \
        asm("li 0, " LINE_STRING ";"); \
    }
#endif

// function stub definitions
#ifndef RB3E_WII
RB3E_STUB(AppConstructor) // AppConstructor is handled by the BrainSlug engine
#endif
RB3E_STUB(ExecuteDTA)
RB3E_STUB(SymbolConstruct)
RB3E_STUB(ModifierActive)
RB3E_STUB(HmxFactoryFuncAt)
RB3E_STUB(BandLabelSetDisplayText)
RB3E_STUB(RandomInt)
RB3E_STUB(DataNodeEvaluate)
RB3E_STUB(DataFindArray)
RB3E_STUB(DataFindData)
RB3E_STUB(FileExists)
RB3E_STUB(QueueMessage)
RB3E_STUB(GetMetadata)
RB3E_STUB(GetSongIDFromShortname)
RB3E_STUB(GetSongShortname)
RB3E_STUB(GetBandUsers)
RB3E_STUB(GetBandUserFromSlot)
RB3E_STUB(ChunkStreamConstructor)
RB3E_STUB(FileStreamConstructor)
RB3E_STUB(Dynamic_Cast)
RB3E_STUB(GameGetActivePlayer)
RB3E_STUB(PropAnimSetFrame)
RB3E_STUB(ObjectFindUIPanel)
RB3E_STUB(JoypadGetPadData)
RB3E_STUB(SongMgrGetRankedSongs)
RB3E_STUB(InitSongMetadata)
RB3E_STUB(MemPrintOverview)
RB3E_STUB(MemPrint)
RB3E_STUB(MemNumHeaps)
RB3E_STUB(MemAlloc)
RB3E_STUB(MemFree)
// hooked function stubs
RB3E_STUB(Localize)
RB3E_STUB(SetVenue)
RB3E_STUB(ModifierManagerConstructor)
RB3E_STUB(NewFile)
RB3E_STUB(WillBeNoStrum)
RB3E_STUB(AddGameGem)
RB3E_STUB(SetSongAndArtistName)
RB3E_STUB(SetMusicSpeed)
RB3E_STUB(UpdateTrackSpeed)
RB3E_STUB(SetAddress)
RB3E_STUB(GetWidgetByName)
RB3E_STUB(GetSlotColor)
RB3E_STUB(UsbWiiGetType)
RB3E_STUB(StartDNSLookup)
RB3E_STUB(SetSystemLanguage)
RB3E_STUB(DataReadFile)
RB3E_STUB(StagekitSetState)
RB3E_STUB(GameConstruct)
RB3E_STUB(GameDestruct)
RB3E_STUB(SetSongNameFromNode)
RB3E_STUB(PrepareSomeVectorMaybe)
RB3E_STUB(SomeVectorPushBackMaybe)
RB3E_STUB(MusicLibrarySelect)
RB3E_STUB(SongSortByRecent)
RB3E_STUB(GetSymbolByGameOrigin)
RB3E_STUB(GetGameOriginBySymbol)
RB3E_STUB(JoypadGetCachedXInputCaps)
RB3E_STUB(SymbolPreInit)
RB3E_STUB(QueuingSocketBind)
RB3E_STUB(OperatorEqualsFmt)
RB3E_STUB(UpdatePresence)
RB3E_STUB(StepSequenceJobSetStep)
RB3E_STUB(BinstreamWrite)
RB3E_STUB(BinstreamRead)
RB3E_STUB(BinstreamReadEndian)
RB3E_STUB(BinstreamWriteEndian)
RB3E_STUB(SongParserPitchToSlot)