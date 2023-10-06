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

BSLUG_MODULE_GAME("SZB?");
BSLUG_MODULE_NAME("RB3Enhanced");
BSLUG_MODULE_VERSION(RB3E_BUILDTAG);
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
    _start();
}

BSLUG_MUST_REPLACE(_start, _startHook);

#endif
