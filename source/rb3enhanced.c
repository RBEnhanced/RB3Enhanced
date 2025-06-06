/*
    RB3Enhanced - rb3enhanced.c
    Initialisation functions and basic patches for the game.
*/

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "version.h"
#include "rb3_include.h"
#include "rb3e_include.h"

static int DefinesStep = 0;
char *DefinesHook(char *string_define, int always_null)
{
    switch (DefinesStep++)
    {
    case 0:
        return "RB3E";
    case 1:
        // might not be working?
        if (RB3E_IsEmulator())
            return "RB3E_EMULATOR";
        return NULL;
    default:
        return NULL;
    }
}

void SetVenueHook(int *thisMetaPerformer, Symbol venue)
{
    Symbol blackBackground;
    Modifier *blackBackgroundModifier;

    SymbolConstruct(&blackBackground, "mod_black_background");
    blackBackgroundModifier = ModifierActive(*(int *)PORT_MODIFIERMGR_POINTER, blackBackground, 0);
    if (blackBackgroundModifier->enabled)
        venue.sym = "none";

    if (config.ForcedVenue[0] != '\0')
        venue.sym = config.ForcedVenue;

    RB3E_MSG("Setting venue: %s", venue.sym);

    RB3E_SendEvent(RB3E_EVENT_VENUE_NAME, venue.sym, strlen(venue.sym));

    SetVenue(thisMetaPerformer, venue);
}

void UpdatePresenceHook(void *thisPresenceMgr)
{
    // when the game updates presence, fire off the current screen name to the events socket
    BandUI *bandUI = (BandUI *)PORT_THEBANDUI;
    if (bandUI->currentScreen != NULL && bandUI->currentScreen->screen_name.sym != NULL)
        RB3E_SendEvent(RB3E_EVENT_SCREEN_NAME, bandUI->currentScreen->screen_name.sym, strlen(bandUI->currentScreen->screen_name.sym));
    UpdatePresence(thisPresenceMgr);
}

// New file hook, for ARKless file loading
void *NewFileHook(char *fileName, int flags)
{
    char *new_path = NULL;
    if (strlen(fileName) > 250)
        return NULL;
    if (config.DisableRawfiles)
        goto LOAD_ORIGINAL;
    // checks the platform-specific APIs for the file
    new_path = RB3E_GetRawfilePath(fileName, 0);
    if (new_path != NULL)
    {
        fileName = new_path;
        flags = 0x10002; // tell the game to load this file raw
    }
#ifdef RB3E_WII
    // load the FileSD
    if (RB3E_Mounted > 0 &&
        (memcmp(fileName, "sd:/", 3) == 0 || new_path != NULL)) {
        return FileSD_New(fileName, flags);
    }
#ifdef RB3EDEBUG
    // only on debug builds and only on Dolphin, we can read from NAND
    if (RB3E_IsEmulator() && memcmp(fileName, "nand/", 5) == 0) {
        return FileWiiNAND_New(fileName + 4);
    }
#endif
#endif
LOAD_ORIGINAL:
    if (config.LogFileAccess)
        RB3E_MSG("File: %s (%s)", fileName, (flags & 0x10000) ? "Raw" : "ARK");
    return NewFile(fileName, flags);
}

DataArray *DataReadFileHook(char *path, int dtb)
{
#ifdef RB3E_WII
    // strstr doesn't exist on Wii and I don't trust that it works.
    // TODO(Emma): check
    char *hasdtb = (char *)0x80000000;
#else
    char *hasdtb = strstr(path, ".dtb");
#endif
    char *rawpath = NULL;
    if (config.DisableRawfiles)
        goto LOAD_ORIGINAL;
    if (hasdtb == NULL && dtb == 1)
    {
        rawpath = RB3E_GetRawfilePath(path, 0);
        if (rawpath != NULL)
        {
            RB3E_DEBUG("DataReadFile: %s", rawpath);
            // This works on Xbox (and maybe Wii)? I don't know how but it does.
            // Changing r4 doesn't seem to have any effect, it's the path's work
            // This game fuckin sucks
            return DataReadFile(rawpath, 0);
        }
    }
LOAD_ORIGINAL:
    return DataReadFile(path, dtb);
}

void *ModifierManagerConstructorHook(int thisModifierManager, int unk)
{
    // DO NOT USE use_save_value ON THE MODIFIERS HERE OR IT WILL MAKE VANILLA WII PROFILES UNLOADABLE
    RB3E_MSG("Adding custom modifiers...", NULL);
    ExecuteDTA(PORT_ROCKCENTRALGATEWAY, "{do{push_back {find $syscfg modifiers modifiers} (mod_black_background)}}");
    ExecuteDTA(PORT_ROCKCENTRALGATEWAY, "{do{push_back {find $syscfg modifiers modifiers} (mod_force_hopos)}}");
    ExecuteDTA(PORT_ROCKCENTRALGATEWAY, "{do{push_back {find $syscfg modifiers modifiers} (mod_mirror_mode)}}");
    ExecuteDTA(PORT_ROCKCENTRALGATEWAY, "{do{push_back {find $syscfg modifiers modifiers} (mod_color_shuffle)}}");
    ExecuteDTA(PORT_ROCKCENTRALGATEWAY, "{do{push_back {find $syscfg modifiers modifiers} (mod_gem_shuffle)}}");
    ExecuteDTA(PORT_ROCKCENTRALGATEWAY, "{do{push_back {find $syscfg modifiers modifiers} (mod_double_bass)}}");
    return ModifierManagerConstructor(thisModifierManager, unk);
}

// a TextStream object that prints straight to RB3E_DEBUG
void DebugTextStreamDestructor(TextStream *thisTextStream)
{
    // not dynamically allocating anything, can't be a destructor
}
void DebugTextStreamPrint(TextStream *thisTextStream, char *text)
{
#ifdef RB3E_XBOX
    // HACK(Emma): retail xbdm doesn't like newlines.
    // so what do we do? split up the string by every newline
    char *text_ptr = text;
    char *newline = text;
    do
    {
        newline = strstr(text_ptr, "\n");
        if (newline != NULL)
        {
            newline[0] = 0;
            RB3E_DEBUG("%s", text_ptr);
            // skip over the newline character and keep going
            text_ptr += strlen(text_ptr) + 1;
        }
        else if (text_ptr[0] != 0)
        {
            RB3E_DEBUG("%s", text_ptr);
        }
    } while (newline != NULL);
#else
    RB3E_DEBUG("%s", text);
#endif
}
TextStream_vtable DebugTextStream_vt = {DebugTextStreamDestructor, DebugTextStreamPrint};
TextStream DebugTextStream = {&DebugTextStream_vt};

static unsigned int framecount = 0;
// This function runs every frame.
// DO NOT TAKE LONGER THAN A FEW MILLISECONDS, DO **NOT** DO ANYTHING BLOCKING HERE
// STUFF YOU DO HERE WILL **DIRECTLY** IMPACT THE GAME'S FRAMERATE!
void HTTP_Server_RunLoop();
void Liveless_Poll();
void RB3E_RunLoop()
{
    if (config.EnableNATPMP)
        NATPMP_Poll();
#ifdef RB3E_XBOX
    if (config.EnableHTTPServer)
        HTTP_Server_RunLoop();
    if (config.EnableLiveless)
        Liveless_Poll();
    if (config.EnableUPnP)
        UPNP_Poll();
#endif
#ifdef RB3EDEBUG
    // print out memory every 5 seconds
    if (config.LogMemoryOverview && framecount % 300 == 0)
        MemPrintOverview(-3, &DebugTextStream);
#endif
    framecount++;
}

#ifdef RB3E_XBOX
// broadcasts stagekit events via the local network. not worth making a header file for imo
void StagekitSetState(int state1, int state2);
void StagekitSetStateHook(int state1, int state2)
{
    RB3E_EventStagekit event;
    event.LeftChannel = state1;
    event.RightChannel = state2;
    RB3E_SendEvent(RB3E_EVENT_STAGEKIT, &event, sizeof(RB3E_EventStagekit));
    StagekitSetState(state1, state2);
}
#endif

void ApplyPatches()
{
    // Patch out PlatformMgr::SetDiskError - this effectively nullifies checksum checks on ARKs and MIDs.
    POKE_32(PORT_SETDISKERROR, BLR);
    // Replace max_song_count DataNode::_value call with a hardcoded response.
    POKE_32(PORT_SONGLIMIT, LI(3, 8000));
    // "Fast startup" - make the game not wait until Splash is done
    POKE_32(PORT_FASTSTART_CHECK, NOP);
    // Replace call to song blacklist check with li r3, 0
    POKE_32(PORT_SONGBLACKLIST, LI(3, 0));
    // Patch out the function for checking keys on guitar unlock status.
    POKE_32(PORT_KEYSONGUITAR, BLR);
#ifdef RB3E_WII
    // Patch out calls to CustomSplash in App::_ct
    POKE_32(PORT_STRAPSCREEN_1, NOP);
    POKE_32(PORT_STRAPSCREEN_2, NOP);
    // Patch out erroneous second host header
    POKE_32(PORT_NASWII_HOST, NOP);
    // always fire the UpdatePresence function. TODO(Emma): look into it, still not firing when screen is changed :/
    POKE_32(PORT_UPDATEPRESENCEBLOCK_B, NOP);
#ifdef RB3E_WII_BANK8
    // nop debug crash enumerating content with legacysdmode disabled
    POKE_32(0x80419158, NOP);
#endif
#ifndef RB3E_WII_BANK8
    // always take the branch to 0x8024a628 so vocals can be selected without a mic plugged in
    // bank 8 does not have the mic check
    POKE_32(PORT_MICCHECK, 0x42800140);
#endif
#elif RB3E_XBOX
    if (RB3E_IsEmulator())
        POKE_32(PORT_SONGMGR_ISDEMO_CHECK, NOP);

    // skips check for stagekit to allow for fog commands to be issued without a stagekit plugged in
    POKE_32(PORT_STAGEKIT_EXISTS, NOP);

    // just blr instead of actually triggering the breakpoint
    POKE_32(PORT_QUAZAL_BREAKPOINT, BLR);
#endif
    // fixes a crash in online multiplayer
#ifdef RB3E_WII
    POKE_B(PORT_MULTIPLAYER_CRASH, PORT_MULTIPLAYER_FIX);
#else
    POKE_BL(PORT_MULTIPLAYER_CRASH, PORT_MULTIPLAYER_FIX);
#endif
    RB3E_MSG("Patches applied!", NULL);
}

// apply patches conditionally based on config settings
void ApplyConfigurablePatches()
{
#ifdef RB3E_XBOX
    // currently this only works for Xbox 360
    if (config.RenderResX != 0 && config.RenderResY != 0)
    {
        if (config.RenderResX > 1280 || config.RenderResY > 720)
        {
            RB3E_MSG("Render resolution set above 1280x720! This will cause graphical issues.", NULL);
        }
        POKE_32(PORT_RENDER_RES_X_PATCH1, LI(7, config.RenderResX));
        POKE_32(PORT_RENDER_RES_X_PATCH2, LI(29, config.RenderResX));
        POKE_32(PORT_RENDER_RES_Y_PATCH1, LI(11, config.RenderResY));
    }
#endif

    if (config.UnlockClothing == 1)
    {
        // Unlocks all clothing, tattoos, face paint, and video venues
        POKE_32(PORT_CHARACTER_CLOTHES_CHECK, LI(3, 1));
        POKE_32(PORT_CHARACTER_CLOTHES_CHECK2, BLR);
        POKE_32(PORT_TATTOO_CHECK, LI(3, 1));
        POKE_32(PORT_FACE_PAINT_CHECK, LI(3, 1));
        POKE_32(PORT_VIDEO_VENUE_CHECK, LI(3, 1));
    }

    if (config.DisableMenuMusic == 1)
    {
        // Disables MetaMusic from loading, saves space on the heap
        POKE_32(PORT_METAMUSICLOAD, BLR);
        POKE_32(PORT_METAMUSICSTART, BLR);
        POKE_32(PORT_METAMUSICPOLL, BLR);
        // Always return 1 from MetaMusic::Loaded, in MetaPanel::IsLoaded
        POKE_32(PORT_METAMUSICISLOADED, LI(3, 1));
        POKE_32(PORT_METAMUSICISLOADED + 4, BLR);
    }

#ifdef RB3EDEBUG
    if (config.QuazalLogging == 1)
    {
#ifdef RB3E_WII
        POKE_B(&OperatorEqualsFmt, 0x800183e4);
        POKE_BL(0x80018718, &OperatorEqualsFmtHook);
        POKE_BL(0x8006a6a8, &OperatorEqualsFmtHook);
        POKE_BL(0x8006cb58, &OperatorEqualsFmtHook);
        POKE_BL(0x8006a82c, &OperatorEqualsFmtHook);

        // POKE_32(0x804735c4, NOP);
        POKE_32(0x8006c984, LI(6, 1));
        POKE_32(0x8006a170, LI(6, 1));
        POKE_32(0x8006c9d0, LI(6, 1));
        // POKE_32(0x8007b3d4, LI(6, 0x1b0));
#else
        POKE_B(&OperatorEqualsFmt, 0x82a86ff0);
        POKE_BL(0x82a87550, &OperatorEqualsFmtHook);
        POKE_BL(0x82a97920, &OperatorEqualsFmtHook);
        POKE_BL(0x82a94744, &OperatorEqualsFmtHook);
        POKE_BL(0x82a948e0, &OperatorEqualsFmtHook);

        // POKE_32(0x804735c4, NOP);
        POKE_32(0x82a976dc, LI(8, 1));
        POKE_32(0x82a97920, NOP);
#endif

        HookFunction(PORT_STEPSEQUENCEJOBSETSTEP, &StepSequenceJobSetStep, &StepSequenceJobSetStepHook);
    }
#endif
}

void SymbolPreInitHook(int stringTableSize, int hashTableSize)
{
    SymbolPreInit(stringTableSize, hashTableSize);
    InitGlobalSymbols();
}

#ifdef RB3E_WII
char FileIsLocalHook(char *filepath) {
    if (memcmp(filepath, "sd:/", 4) == 0)
        return 1;
    return FileIsDLC(filepath);
}
#endif

void InitialiseFunctions()
{
#ifndef RB3E_WII
    // AppConstructor is handled by the BrainSlug engine
    POKE_B(&AppConstructor, PORT_APP_CT);
    // the Wii has a different memory manager, these funcs don't exist
    POKE_B(&MemPrintOverview, PORT_MEMPRINTOVERVIEW);
    POKE_B(&MemPrint, PORT_MEMPRINT);
    POKE_B(&MemNumHeaps, PORT_MEMNUMHEAPS);
    // TODO: port these to Wii
    POKE_B(&DataFindArray, PORT_DATAARRAYFINDARRAY);
    POKE_B(&DataFindData, PORT_DATAARRAYFINDDATA);
#endif
    POKE_B(&SongMgrGetRankedSongs, PORT_SONGMGRGETRANKEDSONGS);
#ifndef RB3E_WII_BANK8
    POKE_B(&PrepareSomeVectorMaybe, PORT_PREPARESOMEVECTORMAYBE);
    POKE_B(&SomeVectorPushBackMaybe, PORT_SOMEVECTORPUSHBACKMAYBE);
#endif
    POKE_B(&ExecuteDTA, PORT_EXECUTEDTA);
    POKE_B(&BandLabelSetDisplayText, PORT_BANDLABELSETDISPLAYTEXT);
    POKE_B(&SymbolConstruct, PORT_SYMBOL_CT);
    POKE_B(&ModifierActive, PORT_MODIFIERMGR_ACTIVE);
    POKE_B(&GetBandUserFromSlot, PORT_GETBANDUSERFROMSLOT);
#ifndef RB3E_WII_BANK8
    POKE_B(&HmxFactoryFuncAt, PORT_HMXFACTORYFUNCAT);
    // TODO(Emma): port to bank8
    POKE_B(&ObjectFindUIPanel, PORT_OBJECTFINDUIPANEL);
#endif
    POKE_B(&RandomInt, PORT_RANDOMINT);
    POKE_B(&DataNodeEvaluate, PORT_DATANODEEVALUATE);
    POKE_B(&FileExists, PORT_FILE_EXISTS);
    POKE_B(&SetAddress, PORT_SETADDRESS);
    POKE_B(&QueueMessage, PORT_QUEUEMESSAGE);
    POKE_B(&MusicLibrarySelect, PORT_MUSICLIBRARYSELECTMAYBE);
    POKE_B(&GetSongShortname, PORT_GETSONGSHORTNAME);
    POKE_B(&GetMetadata, PORT_GETMETADATA);
    POKE_B(&GetSongIDFromShortname, PORT_GETSONGIDFROMSHORTNAME);
    POKE_B(&GetBandUsers, PORT_GETBANDUSERS);
    POKE_B(&FileStreamConstructor, PORT_FILESTREAM_CT);
    POKE_B(&ChunkStreamConstructor, PORT_CHUNKSTREAM_CT);
    POKE_B(&Dynamic_Cast, PORT_DYNAMICCAST);
    POKE_B(&GameGetActivePlayer, PORT_GAMEGETACTIVEPLAYER);
    POKE_B(&JoypadGetPadData, PORT_JOYPADGETPADDATA);
    POKE_B(&MemAlloc, PORT_MEMALLOC);
    POKE_B(&MemFree, PORT_MEMFREE);
    POKE_B(&BinstreamWrite, PORT_BINSTREAMWRITE);
    POKE_B(&BinstreamRead, PORT_BINSTREAMREAD);
    POKE_B(&BinstreamWriteEndian, PORT_BINSTREAMWRITEENDIAN);
    POKE_B(&BinstreamReadEndian, PORT_BINSTREAMREADENDIAN);
#ifndef RB3E_XBOX
    POKE_B(&DataRegisterFunc, PORT_DATAREGISTERFUNC);
#endif
#ifdef RB3E_WII
    POKE_B(&FileIsDLC, PORT_FILEISDLC);
#endif
    RB3E_FlushCache((void *)RB3EBase, (unsigned int)RB3EStubEnd - (unsigned int)RB3EBase);
    RB3E_MSG("Functions initialized!", NULL);
}

void ApplyHooks()
{
    POKE_B(PORT_DATAINITFUNCS_TAIL, &AddDTAFunctions);
    POKE_B(PORT_ISSUPPORTEDLANGUAGE, &IsSupportedLanguageHook);
#ifndef RB3E_WII_BANK8
    POKE_B(PORT_BUILDINSTRUMENTSELECTION, &BuildInstrumentSelectionList);
#endif
    POKE_BL(PORT_OPTIONSTR_DEFINE, &DefinesHook);
    POKE_BL(PORT_RUNLOOP_SPARE, &RB3E_RunLoop);
    HookFunction(PORT_LOCALIZE, &Localize, &LocalizeHook);
    HookFunction(PORT_WILLBENOSTRUM, &WillBeNoStrum, &WillBeNoStrumHook);
    HookFunction(PORT_ADDGAMEGEM, &AddGameGem, &AddGameGemHook);
    HookFunction(PORT_SETSONGANDARTISTNAME, &SetSongAndArtistName, SetSongAndArtistNameHook);
    HookFunction(PORT_SETVENUE, &SetVenue, &SetVenueHook);
    HookFunction(PORT_MODIFIERMGR_CT, &ModifierManagerConstructor, &ModifierManagerConstructorHook);
    HookFunction(PORT_NEWFILE, &NewFile, &NewFileHook);
    HookFunction(PORT_SETSONGSPEED, &SetMusicSpeed, &SetMusicSpeedHook);
    HookFunction(PORT_SETTRACKSPEED, &UpdateTrackSpeed, &UpdateTrackSpeedHook);
    HookFunction(PORT_SETADDRESS, &SetAddress, &SetAddressHook);
    HookFunction(PORT_GETWIDGETBYNAME, &GetWidgetByName, &GetWidgetByNameHook);
    HookFunction(PORT_GETSLOTCOLOR, &GetSlotColor, &GetSlotColorHook);
    HookFunction(PORT_SETSYSTEMLANGUAGE, &SetSystemLanguage, &SetSystemLanguageHook);
    HookFunction(PORT_DATAREADFILE, &DataReadFile, &DataReadFileHook);
    HookFunction(PORT_GAME_CT, &GameConstruct, &GameConstructHook);
    HookFunction(PORT_GAME_DT, &GameDestruct, &GameDestructHook);
    HookFunction(PORT_GETSYMBOLBYGAMEORIGIN, &GetSymbolByGameOrigin, &GetSymbolByGameOriginHook);
    HookFunction(PORT_GETGAMEORIGINBYSYMBOL, &GetGameOriginBySymbol, &GetGameOriginBySymbolHook);
    HookFunction(PORT_RNDPROPANIMSETFRAME, &PropAnimSetFrame, &PropAnimSetFrameHook);
    HookFunction(PORT_SYMBOLPREINIT, &SymbolPreInit, &SymbolPreInitHook);
    HookFunction(PORT_INITSONGMETADATA, &InitSongMetadata, &InitSongMetadataHook);
    HookFunction(PORT_UPDATEPRESENCE, &UpdatePresence, &UpdatePresenceHook);
    HookFunction(PORT_SONGPARSERPITCHTOSLOT, &SongParserPitchToSlot, &SongParserPitchToSlotHook);
    HookFunction(PORT_DATASET, &DataSet, &DataSetHook);
    HookFunction(PORT_DATASETELEM, &DataSetElem, &DataSetElemHook);
    HookFunction(PORT_DATAONELEM, &DataOnElem, &DataOnElemHook);

#ifdef RB3E_WII // wii exclusive hooks
    // HookFunction(PORT_USBWIIGETTYPE, &UsbWiiGetType, &UsbWiiGetTypeHook);
    HookFunction(PORT_WIINETINIT_DNSLOOKUP, &StartDNSLookup, &StartDNSLookupHook);
    POKE_B(PORT_FILEISLOCAL, &FileIsLocalHook);
#elif RB3E_XBOX // 360 exclusive hooks
    HookFunction(PORT_STAGEKIT_SET_STATE, &StagekitSetState, &StagekitSetStateHook);
    HookFunction(PORT_SETSONGNAMEFROMNODE, &SetSongNameFromNode, &SetSongNameFromNodeHook);
    // TODO: port these to Wii
    HookFunction(PORT_DATANODEGETOBJ, &DataNodeGetObj, &DataNodeGetObjHook);
    POKE_B(PORT_GETSONGID, &GetSongIDHook);
    POKE_BL(PORT_SONG_ID_EVALUATE, &MetadataSongIDHook);
    POKE_BL(PORT_LOADOBJS_BCTRL, &LoadObj);
    POKE_BL(PORT_VERTEX_READ_1, &VertexReadHook);
    POKE_BL(PORT_VERTEX_READ_2, &VertexReadHook);
#endif
    RB3E_MSG("Hooks applied!", NULL);
}

void InitCNTHooks();
void TryToLoadPRNGKeyFromFile();

void StartupHook(void *ThisApp, int argc, char **argv)
{
    RB3E_MSG("Loaded! Version " RB3E_BUILDTAG " (" RB3E_BUILDCOMMIT ")", NULL);
    // apply code patches and hooks
    InitialiseFunctions();
    ApplyPatches();
    ApplyHooks();
    // initialise the default config state
    InitDefaultConfig();
    // if the launcher's passed a config, try to load it
    if (HasLauncherConfig())
        LoadConfig();
    // mount filesystems, then load a config file
    RB3E_MountFileSystems();
    LoadConfig();
    // apply any patches that are only added after config loads
    ApplyConfigurablePatches();

#ifdef RB3E_WII
    if (RB3E_Mounted && config.LegacySDMode == false && config.ModernSDMode == true)
    {
        TryToLoadPRNGKeyFromFile();
        InitCNTHooks();
    }
#endif

    // start the game by calling the proper app constructor
    RB3E_MSG("Starting Rock Band 3...", NULL);
    AppConstructor(ThisApp, argc, argv);
    // anything after here is post-splash
    return;
}
