/*
    RB3Enhanced - config.c
    Handles loading the rb3.ini configuration file to set plugin settings.
*/

#include <stdlib.h>
#include <string.h>
#include "config.h"
#include "rb3enhanced.h"
#include "ports.h"
#include "inih.h"

RB3E_Config config;

#ifdef RB3E_XBOX
const char *paths[8] = {"GAME:\\rb3.ini", "RB3HDD:\\rb3.ini", "RB3USB0:\\rb3.ini", "RB3USB1:\\rb3.ini", "RB3USB2:\\rb3.ini", "RB3MUINT:\\rb3.ini", "RB3INTUSB:\\rb3.ini", "RB3INTMMC:\\rb3.ini"};
#elif RB3E_WII
const char *paths[1] = {"sd:/rb3/rb3.ini"};
#endif

void InitConfig()
{
    memset(&config, 0, sizeof(config));
#ifdef RB3E_WII
    // strcpy(config.NASServer, "naswii.wiimmfi.de");
#endif
    config.SongSpeedMultiplier = 1.0;
    config.TrackSpeedMultiplier = 1.0;
#ifdef RB3EDEBUG
    config.LogFileAccess = 1;
#endif
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
        if (strcmp(name, "ForcedVenue") == 0 && !RB3E_CONFIG_FALSE(value))
            strncpy(config.ForcedVenue, value, RB3E_MAX_CONFIG_LEN);
        if (strcmp(name, "GameOriginIcons") == 0)
            config.GameOriginIcons = RB3E_CONFIG_BOOL(value);
        if (strcmp(name, "LogFileAccess") == 0)
            config.LogFileAccess = RB3E_CONFIG_BOOL(value);
        if (strcmp(name, "UnlockClothing") == 0)
            config.UnlockClothing = RB3E_CONFIG_BOOL(value);
        if (strcmp(name, "LanguageOverride") == 0 && strlen(value) == RB3E_LANG_LEN)
            strncpy(config.LanguageOverride, value, RB3E_LANG_LEN);
    }
    if (strcmp(section, "GoCentral") == 0)
    {
        if (strcmp(name, "EnableGoCentral") == 0)
            config.EnableGoCentral = RB3E_CONFIG_BOOL(value);
        if (strcmp(name, "GoCentralAddress") == 0)
            strncpy(config.GoCentralAddress, value, RB3E_MAX_CONFIG_LEN);
    }
    if (strcmp(section, "Graphics") == 0)
    {
        if (strcmp(name, "RenderResX") == 0)
            config.RenderResX = atoi(value);
        if (strcmp(name, "RenderResY") == 0)
            config.RenderResY = atoi(value);
    }
#ifdef RB3E_WII
    if (strcmp(section, "Wii") == 0)
    {
        if (strcmp(name, "NASServer") == 0)
            strncpy(config.NASServer, value, RB3E_MAX_DOMAIN);
        if (strcmp(name, "LegacySDMode") == 0)
            config.LegacySDMode = RB3E_CONFIG_BOOL(value);
    }
#elif RB3E_XBOX
    if (strcmp(section, "Xbox360") == 0)
    {
        if (strcmp(name, "EnableLiveless") == 0)
            config.EnableLiveless = RB3E_CONFIG_BOOL(value);
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
    return 1;
}

void LoadConfig()
{
    char buf[0x2000] = {0};
    int file = -1;
    int i = 0;
    int read = 0;
    if (!RB3E_Mounted)
    {
        RB3E_MSG("No drives mounted, using default config.", NULL);
        return;
    }
    for (i = 0; i < (sizeof(paths) / sizeof(paths[0])); i++)
    {
        RB3E_DEBUG("Checking %s", paths[i]);
        if (!RB3E_FileExists(paths[i]))
            continue;
        file = RB3E_OpenFile(paths[i], 0);
        if (file == -1)
        {
            RB3E_MSG("%s found, but failed to load", paths[i]);
            continue;
        }
        else
        {
            RB3E_MSG("Loading config from %s", paths[i]);
            break;
        }
    }
    if (file == -1)
    {
        RB3E_MSG("No config file loaded, using default config.", NULL);
        return;
    }
    read = RB3E_ReadFile(file, 0, buf, sizeof(buf));
    RB3E_DEBUG("Read %i bytes from config file, parsing", read);
    if (ini_parse_string(buf, INIHandler, NULL) < 0)
    {
        RB3E_MSG("Failed to load config file, using default config", NULL);
        InitConfig();
    }
    else
    {
        RB3E_MSG("Successfully loaded config!", NULL);
    }
    RB3E_CloseFile(file);
}