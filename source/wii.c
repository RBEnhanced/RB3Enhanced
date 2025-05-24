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
#include "GlobalSymbols.h"
#include "config.h"
#include "rb3/WiiMemMgr.h"
#include "utilities.h"

char _has256MBMem2 = 0;
int RB3E_Launcher_ExpandedRAM = 0;

BSLUG_MODULE_GAME("SZB?");
BSLUG_MODULE_NAME("RB3Enhanced");
#ifndef RB3E_WII_BANK8
BSLUG_MODULE_VERSION(RB3E_BUILDTAG);
#else
BSLUG_MODULE_VERSION(RB3E_BUILDTAG "-BANK8");
#endif
BSLUG_MODULE_AUTHOR("github.com/RBEnhanced");
BSLUG_MODULE_LICENSE("GPLv2");

char ACURL[RB3E_MAX_DOMAIN + 16];
char PRURL[RB3E_MAX_DOMAIN + 16];

static int HasRunDetection = 0;
static int DetectionResult = 0;
static int DetectDolphin()
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

int RB3E_IsEmulator()
{
    if (HasRunDetection)
        return DetectionResult;
    DetectionResult = DetectDolphin();
    HasRunDetection = 1;
    return DetectionResult;
}

// i just made some limits up on the spot, sorry ~Emma
//    bad idea? maybe!
//    don't call more than 10 threads with more than 0x10000 bytes of stack
static int thread_stack[0x10000];
static void *thread_stack_ptr = thread_stack;
static OSThread_t threads[10];
static int made_threads = 0;
int RB3E_CreateThread(void *address, void *arg, int stack_size)
{
    OSThread_t *thread = &threads[made_threads++];
    thread_stack_ptr += stack_size;
    if (OSCreateThread(thread, address, arg, thread_stack_ptr, stack_size, 0x18, false))
        return (int)thread;
    return -1;
}

void RB3E_FlushCache(void * address, unsigned int size) {
    unsigned int alignedAddress = ((unsigned int)address & ~0x1F);
    unsigned int alignedSize = ((unsigned int)address & ~0x1F) + 0x20;
    DCFlushRange((void *)alignedAddress, alignedSize);
    ICInvalidateRange((void *)alignedAddress, alignedSize);
}

int RB3E_RelaunchGame()
{
    /*
        Adding this *properly* involves adding new BrainSlug function definitions,
        so I'm gonna hold off until those can be done properly, maybe.

        But the gist of what this function ought to do on Wii is:

        First, make a nice clean buffer, in MEM2 ideally.
        maybe we can use game allocator for this? i dont know. "pseudo"code follows:

        int file = RB3E_OpenFile("rb3/rb3e/launcher.dol", 0);
        int filesize = RB3E_FileSize(file);
        RB3E_ReadFile(file, 0, buffer, filesize);
        RB3E_CloseFile(file);
        FAT_partition_destructor(&sd_partition);
        __io_wiisd.shutdown();
        OSDisableInterrupts();
        uint32_t entrypoint = LoadDOL(buffer);
        ((void)(*)entrypoint)();
    */
    return -1;
}

void HeapInit(Heap *heap, const char *name, int num, int *mem, int sizeWords, int isHandle, int strategy, int debug_level, int allow_temp);
void HeapInitHook(Heap *heap, const char *name, int num, int *mem, int sizeWords, int isHandle, int strategy, int debug_level, int allow_temp)
{
    if (strcmp(name, "main") == 0)
    {
        // TODO(Emma): somehow reclaim original "mem" buffer here, as right now it's a very large (~38MiB) memory leak
        //             ideally we would add it as a free block in the resulting heap
        mem = (int *)(0x94000000);
        sizeWords = 0x3000000;
    }
    RB3E_DEBUG("Heap %i (\"%s\") = %p, words: 0x%x (size: 0x%x)", num, name, mem, sizeWords, sizeWords * 4);
    HeapInit(heap, name, num, mem, sizeWords, isHandle, strategy, debug_level, allow_temp);
}

#ifdef RB3E_WII_BANK8
void ResolvedModuleKeyboard(void *keyboardRso);
void ResolvedModuleKeyboardHook(void *keyboardRso)
{
    // The keyboard RSO in bank 8 has asserts that detect if the address is outside of 128MB worth of MEM2
    // so we try to patch out the asserts that crash as well as all the address checks.
    // Does this suck? Yes. Does it work? Debatable.
    uint32_t *rsoAsUint32 = (uint32_t *)keyboardRso;
    for (int i = 0; i < (0x100000 / 4); i++)
    {
        // check for (address & 0xF8000000) == 0x90000000
        if ((rsoAsUint32[i] & 0xF00FFFFF) == 0x50030008 &&     // rlwinm rD, rA, 0, 0, 0x4
            (rsoAsUint32[i + 1] & 0xFF00FFFF) == 0x3c007000 && // addis r0, rA, 0x7000
            rsoAsUint32[i + 2] == 0x28000000)                  // cmplwi r0, 0
        {
            RB3E_DEBUG("Patching keyboard address check at %p", &rsoAsUint32[i]);
            rsoAsUint32[i + 1] = LI(0, 0); // make the check always 0 (always succeed)
        }
        // check for assert function start
        if (rsoAsUint32[i] == 0x90010070 &&
            rsoAsUint32[i + 1] == 0x41820158 &&
            rsoAsUint32[i + 2] == 0x7f83e378)
        {
            RB3E_DEBUG("Patching assert function at %p", &rsoAsUint32[i]);
            rsoAsUint32[i] = BLR;
        }
    }
    ResolvedModuleKeyboard(keyboardRso);
}
#endif

extern void RB3E_InstallWiiExceptionHandler(); // wii_exceptions.c
static void CTHook(void *ThisApp, int argc, char **argv)
{
    // install an exception handler to show a graphical error when things go wrong
    RB3E_InstallWiiExceptionHandler();
    // print out if we're using a 256MB MEM2
    if (_has256MBMem2)
    {
        RB3E_MSG("Running with 256MB MEM2!", NULL);
    }
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
    // unmount sd card if legacy sd card mode is enabled and the drive is mounted
    if (config.LegacySDMode && RB3E_Mounted)
    {
        RB3E_DEBUG("Unmounting SD card...", NULL);
        FAT_partition_destructor(&sd_partition);
        __io_wiisd.shutdown();
        RB3E_Mounted = 0;
    }
}

BSLUG_MUST_REPLACE(AppConstructor, CTHook);

// HACK(Emma): because, there is a function that runs *before App::__ct() and main()*, we have to apply some hooks here...
// don't use HookFunction here if it's repeated in rb3enhanced.c
// TODO(Emma): move this to a platform-agnostic thing, since there *could* be stuff like this in 360
void _start();
static void _startHook()
{
    POKE_B(&SymbolConstruct, PORT_SYMBOL_CT);
    POKE_B(PORT_BIGSYMBOLFUNC_TAIL, InitGlobalSymbols);

    // Extrems's 480p fix
    POKE_B(PORT_VISETMODE_LI_28, PORT_VISETMODE_PATCH_CODE);
    POKE_32(PORT_VISETMODE_PATCH_CODE + 0, LI(3, 3));
    POKE_32(PORT_VISETMODE_PATCH_CODE + 4, LI(28, 1));
    POKE_B(PORT_VISETMODE_PATCH_CODE + 8, PORT_VISETMODE_LI_28 + 4);
    POKE_32(PORT_VISETMODE_STB_28, 0x98610021); // replace stb r28,0x21(r1) with stb r3,0x21(r1)

#if 0
    // TODO(Emma): can we somehow make this an optional toggle in the launcher?
    // Remove the deflicker filter
    POKE_32(PORT_GXSETCOPYFILTER_BEQ, 0x48000040); // replace beq with b
#endif

#ifdef RB3EDEBUG
    // for now we limit the ability to use more than intentional RAM to debug builds
    // just in case shit hits the fan
    // Cool Facts: on a devkit Wii, the retail build of RB3 will have a 128MB heap by default. so we don't want to handle anything extra!

    // detect 256MB being enabled in Dolphin
    if (*(uint32_t *)0x80003118 >= 0x10000000)
    {
        _has256MBMem2 = 1;
        // change OS1 globals to act like MEM2 is 64MB
        // this is because our Heap expansion code relies on the game thinking it's running on 64MB
        POKE_32(0x80003118, 0x04000000); // mem2 size physical
        POKE_32(0x8000311C, 0x04000000); // mem2 size simulated
        POKE_32(0x80003120, 0x93400000); // end of mem2
        POKE_32(0x80003128, 0x933E0000); // usable mem2 end
        POKE_32(0x80003130, 0x933E0000); // ipc buffer start
        POKE_32(0x80003134, 0x93400000); // ipc buffer end
    }
    // otherwise, detect the launcher enabling 256MB on vWii
    else if (RB3E_Launcher_ExpandedRAM == 0xCAFE1337)
    {
        _has256MBMem2 = 1;
    }

    if (_has256MBMem2)
    {
        // change the IBAT and DBAT initialisation functions

        // change ConfigMEM2_52MB to allocate 256MB DBAT
        POKE_32(PORT_CONFIGMEM2_52MB_INST + 0x0, ADDI(3, 3, 0x1FFF));  // 0x90000000 cached DBAT
        POKE_32(PORT_CONFIGMEM2_52MB_INST + 0x10, ADDI(5, 5, 0x1FFF)); // 0xd0000000 uncached DBAT
        // change ConfigMEM2_56MB to allocate 256MB DBAT
        POKE_32(PORT_CONFIGMEM2_56MB_INST + 0x0, ADDI(3, 3, 0x1FFF));  // 0x90000000 cached DBAT
        POKE_32(PORT_CONFIGMEM2_56MB_INST + 0x10, ADDI(5, 5, 0x1FFF)); // 0xd0000000 uncached DBAT
        // change ConfigMEM2_64MB to allocate 256MB DBAT
        POKE_32(PORT_CONFIGMEM2_64MB_INST + 0x0, ADDI(3, 3, 0x1FFF));  // 0x90000000 cached DBAT
        POKE_32(PORT_CONFIGMEM2_64MB_INST + 0x10, ADDI(5, 5, 0x1FFF)); // 0xd0000000 uncached DBAT
        // change EnableInstsOnMEM2Lo16MB to enable insts on all 256MB
        // RsoInit calls this in RealMode to allow RSOs to run from MEM2
        POKE_32(PORT_ENABLEINSTSONMEM2LO16MB_INST + 0x0, ADDI(6, 6, 0x1FFF)); // 0x90000000 cached IBAT
        // clear icache just in case
        ICInvalidateRange((void *)PORT_CONFIGMEM2_52MB_INST, 0x800);

        // hook Heap::Init to change size and location of main heap
        HookFunction(PORT_HEAPINIT, HeapInit, HeapInitHook);

#ifdef RB3E_WII_BANK8
        // RSO loader code gets real mad at you
        POKE_32(PORT_BANK8_MEM2_RSO_ASSERT1, LIS(0, 0xa000));
        POKE_32(PORT_BANK8_MEM2_RSO_ASSERT2, LIS(0, 0xa000));
        POKE_32(PORT_BANK8_MEM2_RSO_ASSERT3, LIS(0, 0xa000));
        POKE_32(PORT_BANK8_MEM2_RSO_ASSERT4, LIS(0, 0xa000));
        // hook the keyboard RSO function to accept pointers past 0x98000000
        HookFunction(PORT_BANK8_KEYBOARD_RESOLVED, ResolvedModuleKeyboard, ResolvedModuleKeyboardHook);
#endif
    }
#endif

    // launch the game
    _start();
}

BSLUG_MUST_REPLACE(_start, _startHook);

#endif
