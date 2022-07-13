/*
    RB3Enhanced - rb3enhanced.c
    Initialisation functions and basic patches for the game.
*/

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "ports.h"
#include "ppcasm.h"
#include "config.h"
#include "utilities.h"
#include "version.h"
#include "LocaleHooks.h"
#include "GemHooks.h"
#include "GlobalSymbols.h"
#include "rb3enhanced.h"
#include "DTAFunctions.h"
#include "SetlistHooks.h"
#include "SpeedHooks.h"
#include "gocentral.h"
#include "STUN.h"
#include "wii_usbhid.h"
#include "rb3/File.h"
#include "rb3/MetaPerformer.h"
#include "rb3/BandLabel.h"
#include "rb3/Data.h"
#include "rb3/Symbol.h"
#include "rb3/ModifierManager.h"
#include "rb3/RockCentralGateway.h"
#include "rb3/Locale.h"
#include "rb3/App.h"
#include "rb3/Game.h"
#include "rb3/GameGem.h"
#include "rb3/Random.h"
#include "rb3/SortNode.h"
#include "rb3/TrackPanelDirBase.h"
#include "rb3/InetAddress.h"
#include "rb3/UsbWii.h"

void SetVenueHook(int *thisMetaPerformer, Symbol venue)
{
    Symbol blackBackground;
    Modifier *blackBackgroundModifier;

    RB3E_MSG("Original venue: %s", venue.sym);
    SymbolConstruct(&blackBackground, "mod_black_background");

    blackBackgroundModifier = ModifierActive(*(int *)PORT_MODIFIERMGR_POINTER, blackBackground, 0);
    if (blackBackgroundModifier->enabled)
    {
        venue.sym = "none";
        RB3E_MSG("New venue: %s", venue.sym);
    }

    SetVenue(thisMetaPerformer, venue);
}

// New file hook, for ARKless file loading
void *NewFileHook(char *fileName, int flags)
{
    static char newFile[256] = {0};
#ifdef RB3E_XBOX
    unsigned int i = 0;
    for (i = 0; i < strlen(fileName); i++)
    {
        if (fileName[i] == ':')
            goto LOAD_ORIGINAL; // drive letter, we can't do this
    }
    memcpy(newFile, "GAME:/", 6);
    strcpy(newFile + 6, fileName);
    for (i = 0; i < strlen(newFile); i++)
    {
        if (newFile[i] == '/')
            newFile[i] = '\\'; // fix backticks
    }
#elif RB3E_WII
    strcpy(newFile, "sd:/rb3/");
    strcpy(newFile + 8, fileName);
#endif
    if (config.LogFileAccess)
        RB3E_MSG("%s", fileName);
    // if the file isn't already being loaded from disk, isn't in ".." and exists, use the new file
    if (fileName[0] != '.' && RB3E_FileExists(newFile))
    {
#ifdef RB3E_WII
        fileName = newFile; // the emvolution DVD hooks use this path directly
#endif
        flags = 0x10002; // tell the game to load from GAME/SD
    }
    else
        goto LOAD_ORIGINAL;
LOAD_ORIGINAL:
    return NewFile(fileName, flags);
}

void *ModifierManagerConstructorHook(int thisModifierManager, int unk)
{
    // DO NOT USE use_save_value ON THE MODIFIERS HERE OR IT WILL MAKE VANILLA WII PROFILES UNLOADABLE
    RB3E_MSG("Adding custom modifiers...", NULL);
    ExecuteDTA(PORT_ROCKCENTRALGATEWAY, "{do{push_back {find $syscfg modifiers modifiers} (mod_black_background)}}");
    ExecuteDTA(PORT_ROCKCENTRALGATEWAY, "{do{push_back {find $syscfg modifiers modifiers} (mod_force_hopos)}}");
    ExecuteDTA(PORT_ROCKCENTRALGATEWAY, "{do{push_back {find $syscfg modifiers modifiers} (mod_mirror_mode)}}");
    ExecuteDTA(PORT_ROCKCENTRALGATEWAY, "{do{push_back {find $syscfg modifiers modifiers} (mod_color_shuffle)}}");
    return ModifierManagerConstructor(thisModifierManager, unk);
}

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
#endif
    RB3E_MSG("Patches applied!", NULL);
}

// apply patches conditionally based on config settings
void ApplyConfigurablePatches()
{
    if (config.UnlockClothing == 1)
    {
        // Unlocks all clothing, tattoos, face paint, and video venues
        // TODO: Figure out what marks items as locked in the UI and patch that as well, right now it still shows them as locked
        POKE_32(PORT_CHARACTER_CLOTHES_CHECK, NOP);
        POKE_32(PORT_TATTOO_CHECK, LI(3, 1));
        POKE_32(PORT_FACE_PAINT_CHECK, LI(3, 1));
        POKE_32(PORT_VIDEO_VENUE_CHECK, LI(3, 1));
    }
}

void InitialiseFunctions()
{
#ifndef RB3E_WII // AppConstructor is handled by the BrainSlug engine
    POKE_B(&AppConstructor, PORT_APP_CT);
#endif
    POKE_B(&ExecuteDTA, PORT_EXECUTEDTA);
    POKE_B(&BandLabelSetDisplayText, PORT_BANDLABELSETDISPLAYTEXT);
    POKE_B(&SymbolConstruct, PORT_SYMBOL_CT);
    POKE_B(&ModifierActive, PORT_MODIFIERMGR_ACTIVE);
    POKE_B(&HmxFactoryFuncAt, PORT_HMXFACTORYFUNCAT);
    POKE_B(&RandomInt, PORT_RANDOMINT);
    POKE_B(&DataNodeEvaluate, PORT_DATANODEEVALUATE);
    POKE_B(&FileExists, PORT_FILE_EXISTS);
    RB3E_MSG("Functions initialized!", NULL);
}

void ApplyHooks()
{
    POKE_B(PORT_DATAINITFUNCS_TAIL, &AddDTAFunctions);
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
#ifdef RB3E_WII // wii exclusive hooks
    HookFunction(PORT_USBWIIGETTYPE, &UsbWiiGetType, &UsbWiiGetTypeHook);
#else // 360 exclusive hooks
#endif
    RB3E_MSG("Hooks applied!", NULL);
}

void StartupHook(void *ThisApp, int argc, char **argv)
{
#ifdef RB3E_XBOX
    char returnedIP[50];
#endif

    RB3E_MSG("Loaded! Version " RB3E_BUILDTAG " (" RB3E_BUILDCOMMIT ")", NULL);

    InitialiseFunctions();
    ApplyPatches();
    ApplyHooks();
    RB3E_MountFileSystems();
    InitConfig();
    LoadConfig();
    ApplyConfigurablePatches();

    RB3E_MSG("Starting Rock Band 3...", NULL);
    AppConstructor(ThisApp, argc, argv);

    InitGlobalSymbols(); // this has to be done after init

#ifdef RB3E_XBOX
    if (config.STUNServerIP != NULL)
    {
        if (config.STUNServerPort != 0)
        {
            if (getExternalIP(config.STUNServerIP, config.STUNServerPort, returnedIP) == 0)
            {
                strncpy(config.ExternalIP, returnedIP, RB3E_MAX_IP_LEN);
                RB3E_DEBUG("IP: %s", returnedIP);
            }
            else
            {
                RB3E_DEBUG("Could not get IP via STUN!", NULL);
            }
        }
        else
        {
            RB3E_DEBUG("STUN port not set!", NULL);
        }
    }
    else
    {
        RB3E_DEBUG("STUN server not set!", NULL);
    }
#endif
    return;
}