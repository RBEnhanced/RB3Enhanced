/*
    RB3Enhanced - wii.c
    Wii initialisation and helper functions. (BrainSlug)
*/

#ifdef RB3E_WII

#include <bslug.h>
#include <stdio.h>
#include <rvl/cache.h>
#include <io/fat-sd.h>
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
BSLUG_MODULE_AUTHOR("ihatecompvir & contributors");
BSLUG_MODULE_LICENSE("???");

char ACURL[RB3E_MAX_DOMAIN + 16];
char PRURL[RB3E_MAX_DOMAIN + 16];

static void CTHook(void *ThisApp, int argc, char **argv)
{
    // launch game
    StartupHook(ThisApp, argc, argv);
    // apply wfc patches if applicable
    if (config.EnableGoCentral)
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
}

BSLUG_MUST_REPLACE(AppConstructor, CTHook);

#endif