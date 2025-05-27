/*
    RB3Enhanced - config.h
    Initialisation functions and basic patches for the game.
*/

#ifndef _CONFIG_H
#define _CONFIG_H

#define RB3E_MAX_DOMAIN 128
#define RB3E_MAX_IP_LEN 16
#define RB3E_MAX_CONFIG_LEN 30
#define RB3E_LANG_LEN 3

#define RB3E_CONFIG_BOOL(x) ((strcmp(x, "true") == 0 || strcmp(x, "TRUE") == 0 || strcmp(x, "True") == 0 || strcmp(x, "1") == 0) ? 1 : 0)
#define RB3E_CONFIG_FALSE(x) ((strcmp(x, "false") == 0 || strcmp(x, "FALSE") == 0 || strcmp(x, "False") == 0 || strcmp(x, "0") == 0) ? 0 : 1)

typedef struct _RB3E_Config
{
    // [General]
    float TrackSpeedMultiplier;
    float SongSpeedMultiplier;
    char ForcedVenue[RB3E_MAX_CONFIG_LEN];
    char GameOriginIcons;
    char LogFileAccess;
    char UnlockClothing;
    char LanguageOverride[RB3E_LANG_LEN + 1];
    char RawfilesDir[RB3E_MAX_CONFIG_LEN];
    char DisableRawfiles;
    char QuazalLogging;
    // [Graphics]
    int RenderResX;
    int RenderResY;
    char DisablePostProcessing;
    // [Events]
    char BroadcastTarget[RB3E_MAX_DOMAIN];
    char EnableEvents;
    char SendStagekit;
    // [GoCentral]
    char EnableGoCentral;
    char GoCentralAddress[RB3E_MAX_DOMAIN];
    // [Network]
    char EnableNATPMP;
    char EnableUPnP;
    // [HTTP]
    char EnableHTTPServer;
    char AllowCORS;
    char AllowScripts;
#ifdef RB3E_XBOX
    // [Xbox360]
    char EnableLiveless;
    char LivelessAddress[RB3E_MAX_DOMAIN];
    char DirectConnectIP[RB3E_MAX_IP_LEN];
    char ExternalIP[RB3E_MAX_IP_LEN];
    char STUNServer[RB3E_MAX_DOMAIN];
    int STUNServerPort;
#elif RB3E_WII
    // [Wii]
    char NASServer[RB3E_MAX_DOMAIN];
    char LegacySDMode;
    char ModernSDMode;
#endif
#ifdef RB3EDEBUG
    // [Debug]
    char LogMemoryOverview;
#endif
} RB3E_Config;

extern RB3E_Config config;

void InitDefaultConfig();
int HasLauncherConfig();
void LoadConfig();

#endif // _CONFIG_H
