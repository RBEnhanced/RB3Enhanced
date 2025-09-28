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

RB3E_STUB(RB3EBase); // this will always be the start of .text

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
RB3E_STUB(FileIsDLC)
RB3E_STUB(DxRndSuspend)
#ifndef RB3E_XBOX
RB3E_STUB(DataRegisterFunc) // DataRegisterFunc is inlined on 360
#endif
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
RB3E_STUB(RndTexNewObject)
RB3E_STUB(RndMatNewObject)
RB3E_STUB(RndTexSetBitmap)
RB3E_STUB(RndTexSetBitmap2)
RB3E_STUB(FilePathConstructor)
RB3E_STUB(MusicLibraryMat)
RB3E_STUB(MusicLibraryOnEnter)
RB3E_STUB(MusicLibraryOnUnload)
RB3E_STUB(RndCreateDefaultTexture)
RB3E_STUB(MusicLibraryConstructor)
RB3E_STUB(MusicLibraryGetNodeByIndex)
RB3E_STUB(StringConstructor)
RB3E_STUB(AddSmasherPlateToVector)
RB3E_STUB(vector_push_back)
RB3E_STUB(GameGemDBConstructor)
RB3E_STUB(AddMultiGem)
RB3E_STUB(GetGameGemList)
RB3E_STUB(SongMetadataConstructor)
RB3E_STUB(SongMetadataLoad)
RB3E_STUB(SongSortMgrGetSort)
RB3E_STUB(NodeSortGetNode)
RB3E_STUB(DynamicTexConstructor)
RB3E_STUB(DynamicTexDestructor)
RB3E_STUB(RndMatSetDiffuseTex)
RB3E_STUB(RndTexSetBitmap3)
RB3E_STUB(BinstreamWrite)
RB3E_STUB(BinstreamRead)
RB3E_STUB(BinstreamReadEndian)
RB3E_STUB(BinstreamWriteEndian)
RB3E_STUB(SongParserPitchToSlot)
RB3E_STUB(DataSet)
RB3E_STUB(DataSetElem)
RB3E_STUB(DataOnElem)
RB3E_STUB(DataNodeGetObj)
RB3E_STUB(HeapInit)
RB3E_STUB(ResolvedModuleKeyboard)
RB3E_STUB(XboxContentConstruct)
RB3E_STUB(CacheMgrXbox_MountAsync)
RB3E_STUB(DataArrayExecute)

#ifdef RB3E_WII
// Wii-specific functions
// FUTURE(Emma): these really ought to be provided by the BrainSlug loader
RB3E_STUB(OSFatal)
RB3E_STUB(OSSetErrorHandler)
RB3E_STUB(PPCHalt)
RB3E_STUB(OSReturnToMenu)
RB3E_STUB(ARCInitHandle)
RB3E_STUB(contentInitHandleTitleNAND)
RB3E_STUB(CNTReleaseHandle)
RB3E_STUB(CNTRead)
RB3E_STUB(EC_GetContentInfos)
RB3E_STUB(CNTOpen)
#endif

RB3E_STUB(RB3EStubEnd);
