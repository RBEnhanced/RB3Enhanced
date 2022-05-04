/*
    RB3Enhanced - xbox360.c
    Xbox 360 initialisation and helper functions.
*/

#ifdef RB3E_XBOX

#include <xtl.h>
#include "ppcasm.h"
#include "ports.h"
#include "rb3enhanced.h"

void InitCryptoHooks();
void InitLivelessHooks();

static void CTHook(void *ThisApp, int argc, char **argv)
{
    // initialise hooks for XeCrypt
    InitCryptoHooks();
    // initialise hooks for liveless - this has to be done *after* systeminit
    POKE_BL(PORT_SYSTEMINIT_BLANK, &InitLivelessHooks);
    // launch game
    StartupHook(ThisApp, argc, argv);
}

BOOL APIENTRY DllMain(HANDLE hInstDLL, DWORD reason, LPVOID lpReserved)
{
    if (reason == DLL_PROCESS_ATTACH)
        POKE_BL(PORT_APP_CALL, CTHook);
    return TRUE;
}

#endif