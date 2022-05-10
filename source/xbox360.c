/*
    RB3Enhanced - xbox360.c
    Xbox 360 initialisation and helper functions.
*/

#ifdef RB3E_XBOX

#include <xtl.h>
#include "ppcasm.h"
#include "ports.h"
#include "rb3enhanced.h"
#ifdef RB3E_XENIA
#include "version.h"
// Xenia doesn't allow direct writes, use XBDM export (similar to memcpy) to handle writes
int DmSetMemory(void *dest, int count, void *buf, void *unk);
void Write32(void *address, int integer)
{
    int intOnStack = integer; // write the integer as a location in memory
    DmSetMemory(address, 4, &intOnStack, NULL);
}
#endif

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
    {
        // Replace App::Run with App::RunWithoutDebugging
        POKE_BL(PORT_APP_RUN, PORT_APP_RUNNODEBUG);
        // Apply our hook
        POKE_BL(PORT_APP_CALL, &CTHook);
    }
    return TRUE;
}

#endif