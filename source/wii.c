/*
    RB3Enhanced - wii.c
    Wii initialisation and helper functions. (BrainSlug)
*/

#ifdef RB3E_WII

#include <bslug.h>
#include <stdio.h>
#include <rvl/cache.h>
#include <rvl/ipc.h>
#include <io/fat-sd.h>
#include <io/fat.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "emvolution/dvd.h"
#include "ports.h"
#include "ppcasm.h"
#include "version.h"
#include "rb3enhanced.h"
#include "config.h"

BSLUG_MODULE_GAME("SZB?");
BSLUG_MODULE_NAME("RB3Enhanced");
BSLUG_MODULE_VERSION(RB3E_BUILDTAG);
BSLUG_MODULE_AUTHOR("github.com/RBEnhanced");
BSLUG_MODULE_LICENSE("GPLv2");

char ACURL[RB3E_MAX_DOMAIN + 16];
char PRURL[RB3E_MAX_DOMAIN + 16];

int RB3E_IsEmulator()
{
    int dolphin_fd = IOS_Open("/dev/dolphin", IPC_OPEN_NONE);
    if (dolphin_fd)
    {
        // if we have a file descriptor for opening /dev/dolphin, we're under an emulator.
        IOS_Close(dolphin_fd);
        return 1;
    }
    else
    {
        // otherwise we aren't
        // TODO: Ye Olde Dolphin will report not emulator. Those versions probably wouldn't run RB3E at all tbh.
        return 0;
    }
}

static void CTHook(void *ThisApp, int argc, char **argv)
{
    // launch game
    StartupHook(ThisApp, argc, argv);
    // apply wfc url patches if gocentral is enabled and configured server isn't Ninty
    if (config.EnableGoCentral && strcmp(config.NASServer, "naswii.nintendowifi.net") != 0)
    {
        RB3E_DEBUG("Applying WFC patches...", NULL);
        // craft naswii URLs
        memset(ACURL, 0, sizeof(ACURL));
        memset(PRURL, 0, sizeof(PRURL));
        strcat(ACURL, "http://");
        strcat(ACURL, config.NASServer);
        strcpy(PRURL, ACURL);
        strcat(ACURL, "/ac");
        strcat(PRURL, "/pr");
        // write pointers
        POKE_32(PORT_NASWII_AC_URL, &ACURL);
        POKE_32(PORT_NASWII_PR_URL, &PRURL);
    }
    // unmount sd card if legacy sd card mode is enabled
    if (config.LegacySDMode)
    {
        RB3E_DEBUG("Unmounting SD card...", NULL);
        FAT_partition_destructor(&sd_partition);
        RB3E_Mounted = 0;
    }
}

BSLUG_MUST_REPLACE(AppConstructor, CTHook);

#endif