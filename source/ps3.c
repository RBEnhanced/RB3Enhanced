#ifdef RB3E_PS3

#include <sys/prx.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/process.h>
#include <sys/ppu_thread.h>
#include <sysutil/sysutil_msgdialog.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "config.h"
#include "rb3/App.h"
#include "rb3enhanced.h"
#include "version.h"
#include "ports.h"
#include "ppcasm.h"
#include "utilities.h"

SYS_MODULE_INFO(RB3Enhanced, 0, 1, 0);
SYS_MODULE_START(_prx_start);
SYS_MODULE_STOP(_prx_stop);

// printing without printf for the prx entrypoint
static void sys_tty_write(uint32_t ch, const void *buf, uint32_t len, uint32_t *pwritelen)
{
    system_call_4(403, (uint64_t)ch, (uint64_t)buf, (uint64_t)len, (uint64_t)pwritelen);
}

// sys_tty_write only accepts single-byte on CFW so print intro string on here
static void putstring(char *str, int len)
{
    int written = 0;
    for (int i = 0; i < len; i++)
        sys_tty_write(0, &(str[i]), 1, &written);
}

// this function is written to in the prx entrypoint
// to act as a way to consistently reference the plugin's TOC base
// without needing to do a TOC lookup
uint64_t get_plugin_toc_base()
{
    asm("lis 3, 0xFFFF;"
        "ori 3, 3, 0xFFFF;"
        "blr;");
}

void set_stack_toc(uint64_t toc)
{
    asm("std 3, 0x28(1);");
}

uint64_t get_toc_base()
{
    asm("mr 3, 2;");
}

void set_toc_base(uint64_t toc)
{
    asm("mr 2, 3;");
}

static bool HasEmuDetectResult = false;
static bool EmuDetectResult = false;
int RB3E_IsEmulator()
{
    // Detecting RPCS3 is done by checking if the PID is a low number (always 1, this may change soon though)
    if (!HasEmuDetectResult)
        EmuDetectResult = sys_process_getpid() < 0x1000;
    return EmuDetectResult;
}

int RB3E_RelaunchGame()
{
    return -1;
}

static char *EUR_TitleIDs[] = {
    "BLES01611", // band3_patch
    "BLES00986", // band3
    "BLES00385", // rb2
    "BLES00228", // rb1
};
static int NumEURTitleIDs = 4;

static char *USA_TitleIDs[] = {
    "BLUS30463", // band3
    "BLUS30147", // rb2
    "BLUS30050", // rb1
};
static int NumUSATitleIDs = 3;

static char *primary_titleid = NULL;
static int primary_region = 0; // 0 = EUR, 1 = USA
static bool has_usa_titleids = false;
static bool has_eur_titleids = false;
static bool has_registered_ids = false;

int TitleIDRegister(char *titleid, uint32_t r4);
int TitleIDRegisterHook(char *titleid, uint32_t r4)
{
    RB3E_DEBUG("TitleIDRegister(titleid=%s, r4=0x%08x)", titleid, r4);
    // the first registered title ID is at startup after checking PARAM.SFO
    if (primary_titleid == NULL)
    {
        primary_titleid = titleid;
        if (strcmp(primary_titleid, USA_TitleIDs[0]) == 0)
            primary_region = 1;
        RB3E_MSG("Primary Title ID: %s", primary_titleid);
    }

    // register the title id
    int r = TitleIDRegister(titleid, r4);

    if (config.RegionFreeDLC)
    {
        // check if it's european (Yay!)
        if (!has_eur_titleids)
        {
            for (int i = 0; i < NumEURTitleIDs; i++)
            {
                if (strcmp(titleid, EUR_TitleIDs[i]) == 0)
                {
                    RB3E_DEBUG("Game has passed EUR title IDs", NULL);
                    has_eur_titleids = true;
                    break;
                }
            }
        }

        // check if it's american (Noo!)
        if (!has_usa_titleids)
        {
            for (int i = 0; i < NumUSATitleIDs; i++)
            {
                if (strcmp(titleid, USA_TitleIDs[i]) == 0)
                {
                    RB3E_DEBUG("Game has passed USA title IDs", NULL);
                    has_usa_titleids = true;
                    break;
                }
            }
        }

        // if we are european, on our last title id, add the american ones too
        if (primary_region == 0 && r4 == NumEURTitleIDs - 1 && !has_usa_titleids && !has_registered_ids)
            for (int i = 0; i < NumUSATitleIDs; i++)
            {
                RB3E_DEBUG("Registering USA title ID %s", USA_TitleIDs[i]);
                TitleIDRegister(USA_TitleIDs[i], r4 + 1 + i);
                has_registered_ids = true;
            }

        // if we are american, on our last title id, add the european ones too
        if (primary_region == 1 && r4 == NumUSATitleIDs - 1 && !has_eur_titleids && !has_registered_ids)
            for (int i = 0; i < NumEURTitleIDs; i++)
            {
                RB3E_DEBUG("Registering EUR title ID %s", EUR_TitleIDs[i]);
                TitleIDRegister(EUR_TitleIDs[i], r4 + 1 + i);
                has_registered_ids = true;
            }
    }

    return r;
}

static bool has_run_cthook = false;
extern void sys_timer_sleep(int time);
void RPCS3NotifyButton(int button, void *arg)
{
}
extern int PS3_CodeWrites;
extern written_memory_patch PS3_Code[];
void RPCS3NotifyThread(uint64_t arg)
{
    sys_timer_sleep(5);
    if (!has_run_cthook)
    {
        cellMsgDialogOpen2(CELL_MSGDIALOG_TYPE_SE_TYPE_ERROR | CELL_MSGDIALOG_TYPE_BUTTON_TYPE_OK,
                           "RB3Enhanced requires that you run RPCS3 with the PPU Decoder setting set to 'Interpreter' mode.\n\n"
                           "Please re-launch Rock Band 3 with that setting applied.",
                           RPCS3NotifyButton, NULL, NULL);
        return;
    }
    else
    {
        char filename[128];
        sprintf(filename, "/dev_hdd0/rb3/%s_patch.yml", primary_region == 0 ? EUR_TitleIDs[1] : USA_TitleIDs[0]);
        if (!RB3E_FileExists(filename))
        {
            int fd = RB3E_OpenFile(filename, 1);
            if (fd == -1)
            {
                cellMsgDialogOpen2(CELL_MSGDIALOG_TYPE_SE_TYPE_ERROR | CELL_MSGDIALOG_TYPE_BUTTON_TYPE_OK,
                                   "RB3Enhanced failed to write RPCS3 patch file.",
                                   RPCS3NotifyButton, NULL, NULL);
            }
            else
            {
                // sprintf(buf, patch_header_template, MODULE_HASH, MODULE_TYPE, TITLE_ID, VERSION);
                char *patch_header_template = "%s:\n"
                                              "  \"RB3Enhanced " RB3E_BUILDCOMMIT " (%s)\":\n"
                                              "    Games:\n"
                                              "      \"Rock Band 3\":\n"
                                              "        %s: [ 01.%02i ]\n"
                                              "    Author: \"https://rb3e.rbenhanced.rocks/\"\n"
                                              "    Notes: \"Autogenerated by RB3Enhanced " RB3E_BUILDTAG " (" RB3E_BUILDCOMMIT ").\"\n"
                                              "    Patch Version: 1.0\n"
                                              "    Patch:\n";
                char header_buffer[512];

                // write RPCS3 patch header
                int fileoff = RB3E_WriteFile(fd, 0, "Version: 1.2\n\n", strlen("Version: 1.2\n\n"));

                // write the PPU patches
                // TODO: is it somehow possible to read the module IDs at runtime?
                sprintf(header_buffer, patch_header_template,
                        "PPU-0000000000000000000000000000000000000000", "EBOOT",
                        primary_region == 0 ? EUR_TitleIDs[1] : USA_TitleIDs[0], primary_region == 0 ? 6 : 5);
                fileoff += RB3E_WriteFile(fd, fileoff, header_buffer, strlen(header_buffer));
                for (int i = 0; i < PS3_CodeWrites; i++)
                {
                    if (PS3_Code[i].address < 0x1000000)
                    {
                        char buffer[128];
                        sprintf(buffer, "      - [ be32, 0x%x, 0x%08x ]\n", PS3_Code[i].address, PS3_Code[i].value);
                        fileoff += RB3E_WriteFile(fd, fileoff, buffer, strlen(buffer));
                    }
                }

                // write the RB3E PRX patches
                sprintf(header_buffer, patch_header_template,
                        "PRX-aaaaaaaaaaaaaaaaaaaaaaaaaaaa-0", "PRX",
                        primary_region == 0 ? EUR_TitleIDs[1] : USA_TitleIDs[0], primary_region == 0 ? 6 : 5);
                fileoff += RB3E_WriteFile(fd, fileoff, header_buffer, strlen(header_buffer));
                // hack to get our base address
                uint32_t base_address = PLUGIN_PTR(AppConstructor);
                for (int i = 0; i < PS3_CodeWrites; i++)
                {
                    if (PS3_Code[i].address > 0x1000000)
                    {
                        char buffer[128];
                        sprintf(buffer, "      - [ be32, 0x%x, 0x%08x ]\n", PS3_Code[i].address - base_address, PS3_Code[i].value);
                        fileoff += RB3E_WriteFile(fd, fileoff, buffer, strlen(buffer));
                    }
                }

                RB3E_CloseFile(fd);
                char dialog[512];
                sprintf(dialog, "A patch file has been written to %s!\n\n"
                                "Copy this file into your RPCS3 \"patches\" folder, then enable the \"RB3Enhanced\" patches\n"
                                "in the RPCS3 Patch Manager.",
                        filename);
                cellMsgDialogOpen2(CELL_MSGDIALOG_TYPE_SE_TYPE_NORMAL | CELL_MSGDIALOG_TYPE_BUTTON_TYPE_OK,
                                   dialog,
                                   RPCS3NotifyButton, NULL, NULL);
            }
        }
    }
}

void InitCryptoHooks();
static void CTHook(void *app, int argc, char **argv)
{
    // we save what the game's TOC base is and switch to our own
    uint64_t game_toc_base = get_toc_base();
    uint64_t plugin_toc_base = get_plugin_toc_base();
    set_toc_base(plugin_toc_base);

    // keep track of what the game and plugin's respective TOCs are
    PS3_SetGameTOCBase(game_toc_base);
    PS3_SetPluginTOCBase(plugin_toc_base);

    has_run_cthook = true;

    // copy over a chunk of the game's TOC into the blank space we put in ours
    // this means we don't have to recover the game's TOC after our functions run, in most cases anyways
    // thank you chadderz for the guidance
    // i don't know if we copy enough, a todo would be to check
    uint64_t game_toc = game_toc_base - 0x8000;
    uint64_t plugin_toc = plugin_toc_base - 0x8000;
    RB3E_DEBUG("game TOC = 0x%08x (r2=%08x)", game_toc, game_toc_base);
    RB3E_DEBUG("plugin TOC = 0x%08x (r2=%08x)", plugin_toc, plugin_toc_base);
    RB3E_DEBUG("copying 0x8000 from game TOC into plugin TOC", NULL);
    PS3_WriteMemory(plugin_toc, (void *)game_toc, 0xA000);

    InitCryptoHooks();
    HookFunction(PORT_TITLEIDREGISTER, PLUGIN_PTR(TitleIDRegister), PLUGIN_PTR(TitleIDRegisterHook));

    // launch RB3Enhanced + RB3
    StartupHook(app, argc, argv);
}

typedef enum _debugcheck_result
{
    can_use_dbg,
    can_use_ps3mapi,
    cant_use_either
} debugcheck_result;

int _prx_start(unsigned int args, unsigned int *argp)
{
    RB3E_MSG("RB3Enhanced.prx entered!", NULL);

    // basic sanity check - has no significance
    // just ensures someone is using the 1.06 BLES executable
    if (*(uint32_t *)0x8be67c != LI(0, 0))
    {
        RB3E_MSG("Wrong executable! Bailing...", NULL);
        return SYS_PRX_NO_RESIDENT;
    }

    char write_strategy = PS3_MemoryWriteCheck();
    if (write_strategy == can_use_dbg)
        RB3E_DEBUG("Using DEX syscalls to write memory", NULL);
    else if (write_strategy == can_use_ps3mapi)
        RB3E_DEBUG("Using PS3MAPI to write memory", NULL);
    else
        RB3E_DEBUG("Couldn't read memory either way? Oops", NULL);

    // write out a function for us to get the plugin's TOC base at any point
    // there are better ways to do this. but this functions
    uint64_t plugin_toc_base_addr = get_toc_base();
    PS3_Write32(PLUGIN_PTR(get_plugin_toc_base), LIS(3, (plugin_toc_base_addr >> 16)));
    PS3_Write32(PLUGIN_PTR(get_plugin_toc_base) + 4, ORI(3, 3, (plugin_toc_base_addr & 0xFFFF)));

    // poke our CTHook into the App::__ct call
    PS3_Write32(PORT_APP_CALL, BL(PLUGIN_PTR(CTHook), PORT_APP_CALL));

    if (RB3E_IsEmulator())
    {
        RB3E_MSG("RPCS3 detected", NULL);
        sys_ppu_thread_t *rpcs3_thread;
        sys_ppu_thread_create(&rpcs3_thread, RPCS3NotifyThread, 0, 1024, 4 * 4 * 1024, 0, "RB3E_RPCS3");
    }

    // stay resident in memory
    return SYS_PRX_RESIDENT;
}

int _prx_stop()
{
    return CELL_OK;
}

#endif // RB3E_PS3
