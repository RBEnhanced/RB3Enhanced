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

#define CONFIG_FILENAME "rb3.ini"

void InitConfig()
{
    memset(&config, 0, sizeof(config));
    strcpy(config.RawfilesDir, "rb3");
#ifdef RB3E_WII
    // uncomment when GoCentral has a default instance that uses naswii auth
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
        if (strcmp(name, "RawfilesDir") == 0 && !RB3E_CONFIG_FALSE(value))
            strncpy(config.RawfilesDir, value, RB3E_MAX_CONFIG_LEN);
        if (strcmp(name, "DisableRawfiles") == 0)
            config.DisableRawfiles = RB3E_CONFIG_BOOL(value);
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
            strncpy(config.GoCentralAddress, value, RB3E_MAX_CONFIG_LEN);
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
    if (strcmp(section, "Graphics") == 0)
    {
        if (strcmp(name, "RenderResX") == 0)
            config.RenderResX = atoi(value);
        if (strcmp(name, "RenderResY") == 0)
            config.RenderResY = atoi(value);
    }
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
    int read = 0;
    char *filepath;
    if (!RB3E_Mounted)
    {
        RB3E_MSG("No drives mounted, using default config.", NULL);
        return;
    }
    filepath = RB3E_GetRawfilePath(CONFIG_FILENAME, 1);
    if (filepath == NULL)
    {
        RB3E_MSG("No config file found, using default config.", NULL);
        return;
    }
    file = RB3E_OpenFile(filepath, 0);
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