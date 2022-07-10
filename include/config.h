/*
    RB3Enhanced - config.h
    Initialisation functions and basic patches for the game.
*/

#ifndef _CONFIG_H
#define _CONFIG_H

#define RB3E_MAX_DOMAIN 128
#define RB3E_MAX_IP_LEN 16
#define RB3E_MAX_CONFIG_LEN 30

#define RB3E_CONFIG_BOOL(x) (strcmp(x, "true") == 0 || strcmp(x, "TRUE") == 0 || strcmp(x, "True") == 0 || strcmp(x, "1") == 0) ? 1 : 0

typedef struct _RB3E_Config
{
    float TrackSpeedMultiplier;
    float SongSpeedMultiplier;
    char ForcedVenue[RB3E_MAX_CONFIG_LEN];
    char GameOriginIcons;
    char EnableGoCentral;
    char LogFileAccess;
    char GoCentralAddress[RB3E_MAX_DOMAIN];
    char UnlockClothing;
#ifdef RB3E_XBOX
    char EnableLiveless;
    char DirectConnectIP[RB3E_MAX_IP_LEN];
    char ExternalIP[RB3E_MAX_IP_LEN];
#elif RB3E_WII
    char NASServer[RB3E_MAX_DOMAIN];
#endif
} RB3E_Config;

extern RB3E_Config config;

void InitConfig();
void LoadConfig();

#endif // _CONFIG_H