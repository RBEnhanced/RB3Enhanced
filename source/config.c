/*
    RB3Enhanced - config.c
    Handles loading the rb3.ini configuration file to set plugin settings.
*/

#include <stdlib.h>
#include <string.h>
#ifdef RB3E_WII
#include <bslug.h>
#endif
#include "config.h"
#include "rb3enhanced.h"
#include "ports.h"
#include "inih.h"

RB3E_Config config;

// config given to the game by the loader
#define LAUNCHER_CONFIG_MAGIC 0x53443A44
int RB3E_Launcher_HasConfig;
char RB3E_Launcher_Config[0x1000];

#define CONFIG_FILENAME "rb3.ini"

void InitDefaultConfig()
{
    memset(&config, 0, sizeof(config));
    strcpy(config.RawfilesDir, "rb3");
#ifdef RB3E_WII
    strcpy(config.NASServer, "naswii.rbenhanced.rocks");
    config.ModernSDMode = 1;
#endif
    config.SongSpeedMultiplier = 1.0;
    config.TrackSpeedMultiplier = 1.0;
#ifdef RB3EDEBUG
    config.LogFileAccess = 1;
#endif
}

int HasLauncherConfig()
{
    return RB3E_Launcher_HasConfig == LAUNCHER_CONFIG_MAGIC;
}

static int INIHandler(void *user, const char *section, const char *name, const char *value)
{
    RB3E_DEBUG("%s - %s : %s", section == NULL ? "none" : section, name, value);
    if (strcmp(section, "General") == 0)
    {
        if (strcmp(name, "SongSpeedMultiplier") == 0)
            config.SongSpeedMultiplier = (float)atof(value);
        if (strcmp(name, "TrackSpeedMultiplier") == 0)
            config.TrackSpeedMultiplier = (float)atof(value);
        if (strcmp(name, "ForcedVenue") == 0 && strcmp(value, "false") != 0)
            strncpy(config.ForcedVenue, value, RB3E_MAX_CONFIG_LEN);
        if (strcmp(name, "GameOriginIcons") == 0)
            config.GameOriginIcons = RB3E_CONFIG_BOOL(value);
        if (strcmp(name, "LogFileAccess") == 0)
            config.LogFileAccess = RB3E_CONFIG_BOOL(value);
        if (strcmp(name, "UnlockClothing") == 0)
            config.UnlockClothing = RB3E_CONFIG_BOOL(value);
        if (strcmp(name, "AllowGoldOnAllDifficulties") == 0)
            config.AllowGoldOnAllDifficulties = RB3E_CONFIG_BOOL(value);
        if (strcmp(name, "DisableMenuMusic") == 0)
            config.DisableMenuMusic = RB3E_CONFIG_BOOL(value);
        if (strcmp(name, "LanguageOverride") == 0 && strlen(value) == RB3E_LANG_LEN)
            strncpy(config.LanguageOverride, value, RB3E_LANG_LEN);
        if (strcmp(name, "RawfilesDir") == 0 && !RB3E_CONFIG_FALSE(value))
            strncpy(config.RawfilesDir, value, RB3E_MAX_CONFIG_LEN);
        if (strcmp(name, "DisableRawfiles") == 0)
            config.DisableRawfiles = RB3E_CONFIG_BOOL(value);
        if (strcmp(name, "QuazalLogging") == 0)
            config.QuazalLogging = RB3E_CONFIG_BOOL(value);
        if (strcmp(name, "ContentLogging") == 0)
            config.ContentLogging = RB3E_CONFIG_BOOL(value);
    }
    if (strcmp(section, "Events") == 0)
    {
        if (strcmp(name, "EnableEvents") == 0)
            config.EnableEvents = RB3E_CONFIG_BOOL(value);
        if (strcmp(name, "BroadcastTarget") == 0)
            strncpy(config.BroadcastTarget, value, RB3E_MAX_DOMAIN);
    }
    if (strcmp(section, "GoCentral") == 0)
    {
        if (strcmp(name, "EnableGoCentral") == 0)
            config.EnableGoCentral = RB3E_CONFIG_BOOL(value);
        if (strcmp(name, "GoCentralAddress") == 0)
            strncpy(config.GoCentralAddress, value, RB3E_MAX_DOMAIN);
    }
    if (strcmp(section, "HTTP") == 0)
    {
        if (strcmp(name, "EnableHTTPServer") == 0)
            config.EnableHTTPServer = RB3E_CONFIG_BOOL(value);
        if (strcmp(name, "AllowCORS") == 0)
            config.AllowCORS = RB3E_CONFIG_BOOL(value);
        if (strcmp(name, "AllowScripts") == 0)
            config.AllowScripts = RB3E_CONFIG_BOOL(value);
    }
    if (strcmp(section, "Network") == 0)
    {
        if (strcmp(name, "EnableNATPMP") == 0)
            config.EnableNATPMP = RB3E_CONFIG_BOOL(value);
        if (strcmp(name, "EnableUPnP") == 0)
            config.EnableUPnP = RB3E_CONFIG_BOOL(value);
    }
#ifdef RB3E_WII
    if (strcmp(section, "Wii") == 0)
    {
        if (strcmp(name, "NASServer") == 0)
            strncpy(config.NASServer, value, RB3E_MAX_DOMAIN);
        if (strcmp(name, "LegacySDMode") == 0)
            config.LegacySDMode = RB3E_CONFIG_BOOL(value);
        if (strcmp(name, "ModernSDMode") == 0)
            config.ModernSDMode = RB3E_CONFIG_BOOL(value);
    }
#elif RB3E_XBOX
    if (strcmp(section, "Xbox360") == 0)
    {
        if (strcmp(name, "EnableLiveless") == 0)
            config.EnableLiveless = RB3E_CONFIG_BOOL(value);
        if (strcmp(name, "LivelessAddress") == 0)
            strncpy(config.LivelessAddress, value, RB3E_MAX_DOMAIN);
        if (strcmp(name, "DirectConnectIP") == 0)
            strncpy(config.DirectConnectIP, value, RB3E_MAX_IP_LEN);
        if (strcmp(name, "ExternalIP") == 0)
            strncpy(config.ExternalIP, value, RB3E_MAX_IP_LEN);
        if (strcmp(name, "STUNServer") == 0)
            strncpy(config.STUNServer, value, RB3E_MAX_DOMAIN);
        if (strcmp(name, "STUNServerPort") == 0)
            config.STUNServerPort = atoi(value);
    }
#endif
    if (strcmp(section, "Graphics") == 0)
    {
#ifdef RB3E_XBOX
        if (strcmp(name, "RenderResX") == 0)
            config.RenderResX = atoi(value);
        if (strcmp(name, "RenderResY") == 0)
            config.RenderResY = atoi(value);
#endif
        if (strcmp(name, "DisablePostProcessing") == 0)
            config.DisablePostProcessing = RB3E_CONFIG_BOOL(value);
    }
#ifdef RB3EDEBUG
    if (strcmp(section, "Debug") == 0)
    {
        if (strcmp(name, "LogMemoryOverview") == 0)
            config.LogMemoryOverview = RB3E_CONFIG_BOOL(value);
    }
#endif
    return 1;
}

void LoadConfig()
{
    char buf[0x2000] = {0};
    int file = -1;
    int read = 0;
    char *filepath;
    // check if the launcher has provided a valid INI to the module
    if (HasLauncherConfig())
    {
        RB3E_MSG("Loading config from launcher...", NULL);
        RB3E_DEBUG("%i byte config provided by launcher at %p, parsing", strlen(RB3E_Launcher_Config), RB3E_Launcher_Config);
        strncpy(buf, RB3E_Launcher_Config, sizeof(RB3E_Launcher_Config));
    }
    // load from mounted drives like usual
    else
    {
        if (!RB3E_Mounted)
        {
            RB3E_MSG("No drives mounted, using default config.", NULL);
            return;
        }
        // get the file path of the config file
        filepath = RB3E_GetRawfilePath(CONFIG_FILENAME, 1);
        if (filepath == NULL)
        {
            RB3E_MSG("No config file found, using default config.", NULL);
            return;
        }
        // open the file, read all the contents, then close
        RB3E_MSG("Loading config from %s...", filepath);
        file = RB3E_OpenFile(filepath, 0);
        read = RB3E_ReadFile(file, 0, buf, sizeof(buf));
        RB3E_CloseFile(file);
        RB3E_DEBUG("Read %i bytes from config file, parsing", read);
    }
    // parse the INI file
    if (ini_parse_string(buf, INIHandler, NULL) < 0)
    {
        RB3E_MSG("Failed to load config file, using default config", NULL);
        // re-initialise default config
        // because some values might've been parsed previously
        InitDefaultConfig();
    }
    else
    {
        RB3E_MSG("Successfully loaded config!", NULL);
    }
}

#ifdef RB3E_WII
// export these two functions to the brainslug loader
// TODO: exporting these on 360?
BSLUG_EXPORT(RB3E_Launcher_HasConfig);
BSLUG_EXPORT(RB3E_Launcher_Config);
#endif
