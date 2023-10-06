/*
    RB3Enhanced - xbox360.c
    Xbox 360 initialisation and helper functions.
*/

#ifdef RB3E_XBOX

#include <xtl.h>
#include "ppcasm.h"
#include "ports.h"
#include "rb3enhanced.h"
#include "xbox360.h"

static int HasRunDetection = 0;
static int DetectionResult = 0;
static int DetectXenia()
{
    // Detect if running in Xenia by checking two things:
    // - The memory address of XAM's first export, real hardware has this after ~0x81C50000 (17559)
    // - The first instruction of the first exports of both XAM and the kernel, Xenia has this as the "sc" instruction.
    HANDLE xamhandle = NULL;
    PDWORD xamimportaddress = NULL;
    HANDLE kernhandle = NULL;
    PDWORD kernimportaddress = NULL;
    HasRunDetection = 1;
    // Get the address of a XAM export.
    XexGetModuleHandle("xam.xex", &xamhandle);
    XexGetProcedureAddress(xamhandle, 1, &xamimportaddress);
    if (xamhandle)
        XCloseHandle(xamhandle);
    RB3E_DEBUG("Xenia detection (XAM): %p = %08x", xamimportaddress, xamimportaddress[0]);
    // If XAM is not in the typical memory address space, we're in an emulator.
    if ((DWORD)xamimportaddress >> 24 != 0x81)
        return 1;
    // Get the address of a Kernel export.
    XexGetModuleHandle("xboxkrnl.exe", &kernhandle);
    XexGetProcedureAddress(kernhandle, 1, &kernimportaddress);
    if (kernhandle)
        XCloseHandle(kernhandle);
    RB3E_DEBUG("Xenia detection (Kernel): %p = %08x", kernimportaddress, kernimportaddress[0]);
    // This *shouldn't* happen, but it could, so just in case.
    if (xamimportaddress == NULL || kernimportaddress == NULL)
        return 1;
    // Checks if the first instruction of either a kernel or XAM export is "sc".
    if (kernimportaddress[0] == 0x44000042 || xamimportaddress[0] == 0x44000042)
        return 1;
    return 0;
}

int RB3E_IsEmulator()
{
    if (HasRunDetection)
        return DetectionResult;
    DetectionResult = DetectXenia();
    return DetectionResult;
}

void RB3E_Sleep(int ms)
{
    Sleep(ms);
}

int RB3E_CreateThread(void *address, void *arg, int stack_size)
{
    unsigned int thread_id;
    if (CreateThread(NULL, stack_size, address, arg, 0, &thread_id) != NULL)
        return thread_id;
    return -1;
}

int RB3E_RelaunchGame()
{
    // no idea if this actually works
    XLaunchNewImage("default.xex", 0);
    return 0;
}

static void CTHook(void *ThisApp, int argc, char **argv)
{
    // initialise hooks for XeCrypt
    InitCryptoHooks();
    // initialise hooks for input
    InitInputHooks();
    // initialise hooks for liveless - this has to be done *after* systeminit
    POKE_BL(PORT_SYSTEMINIT_BLANK, &InitLivelessHooks);
    // launch game
    StartupHook(ThisApp, argc, argv);
}

BOOL APIENTRY DllMain(HANDLE hInstDLL, DWORD reason, LPVOID lpReserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        RB3E_DEBUG("DLL has been loaded");
        // Replace App::Run with App::RunWithoutDebugging
        POKE_BL(PORT_APP_RUN, PORT_APP_RUNNODEBUG);
        // Apply our hook
        POKE_BL(PORT_APP_CALL, &CTHook);
    }
    return TRUE;
}

#endif
