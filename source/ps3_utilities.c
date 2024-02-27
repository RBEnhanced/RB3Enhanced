/*
    RB3Enhanced - ps3_utilities.c
    Helper functions for managing memory writes and function hooks on the PS3.
*/

#ifdef RB3E_PS3

#include <stdint.h>
#include <string.h>
#include <sys/process.h>
#include <sys/syscall.h>

#include "ports.h"
#include "ppcasm.h"

#define PLUGIN_PTR(x) (uint32_t)(*(uint32_t *)&x)

// we have a large stub of executable memory we can use for when we poke
// function tails and need to make sure r2 survives
static uint32_t AssignedStubFuncBytes = 0;
// man this architecture aint shit
static void BigAssStubFunction()
{
    asm("li 0, 0; nop; li 0, 2; blr; li 0, 1; nop; li 0, 4; nop;");
    asm("li 0, 0; nop; li 0, 2; blr; li 0, 1; nop; li 0, 4; nop;");
    asm("li 0, 0; nop; li 0, 2; blr; li 0, 1; nop; li 0, 4; nop;");
    asm("li 0, 0; nop; li 0, 2; blr; li 0, 1; nop; li 0, 4; nop;");
    asm("li 0, 0; nop; li 0, 2; blr; li 0, 1; nop; li 0, 4; nop;");
    asm("li 0, 0; nop; li 0, 2; blr; li 0, 1; nop; li 0, 4; nop;");
    asm("li 0, 0; nop; li 0, 2; blr; li 0, 1; nop; li 0, 4; nop;");
    asm("li 0, 0; nop; li 0, 2; blr; li 0, 1; nop; li 0, 4; nop;");
    asm("li 0, 0; nop; li 0, 2; blr; li 0, 1; nop; li 0, 4; nop;");
    asm("li 0, 0; nop; li 0, 2; blr; li 0, 1; nop; li 0, 4; nop;");
    asm("li 0, 0; nop; li 0, 2; blr; li 0, 1; nop; li 0, 4; nop;");
    asm("li 0, 0; nop; li 0, 2; blr; li 0, 1; nop; li 0, 4; nop;");
    asm("li 0, 0; nop; li 0, 2; blr; li 0, 1; nop; li 0, 4; nop;");
    asm("li 0, 0; nop; li 0, 2; blr; li 0, 1; nop; li 0, 4; nop;");
    asm("li 0, 0; nop; li 0, 2; blr; li 0, 1; nop; li 0, 4; nop;");
}

// TODO(Emma): detect if dbg syscall fails and use hen syscall after the fact
// TODO(Emma): rpcs3 patch generation for llvm

// gets process memory using PS3MAPI, for HEN

static sys_pid_t pid = 0;

static int ps3mapi_get_process_mem(uint64_t address, void *data, size_t size)
{
    system_call_6(8, 0x7777, 0x31, pid, address, (uint64_t)data, size);
    return_to_user_prog(int);
}
// sets process memory using PS3MAPI, for HEN
static int ps3mapi_set_process_mem(uint64_t address, void *data, size_t size)
{
    system_call_6(8, 0x7777, 0x32, pid, address, (uint64_t)data, size);
    return_to_user_prog(int);
}

// gets process memory using DEX, works on Evilnat CFW
static int sys_dbg_read_process_memory(uint64_t address, void *data, size_t size)
{
    system_call_4(904, pid, address, size, (uint64_t)data);
    return_to_user_prog(int);
}
// sets process memory using DEX, works on Evilnat CFW
static int sys_dbg_write_process_memory(uint64_t address, void *data, size_t size)
{
    system_call_4(905, pid, address, size, (uint64_t)data);
    return_to_user_prog(int);
}

// uncomment this line to disable using dbg syscalls for testing purposes
// #define RB3E_DISABLE_PS3DEX

static char should_use_mapi = 0;
typedef enum _debugcheck_result
{
    can_use_dbg,
    can_use_ps3mapi,
    cant_use_either
} debugcheck_result;

// checks to see which set of syscalls to use for memory read/writes
// DEX/PEX consoles and RPCS3 will use sys_dbg_write_process_memory
// CEX/HEN consoles will use ps3mapi_set_process_mem
char PS3_MemoryWriteCheck()
{
    uint8_t elf_header[4];
    static uint8_t expected[4] = {0x7F, 'E', 'L', 'F'};

    pid = sys_process_getpid();
    RB3E_DEBUG("pid=0x%08x", pid);

#ifndef RB3E_DISABLE_PS3DEX
    int dbg_r = sys_dbg_read_process_memory(0x10000, elf_header, 4);
    RB3E_DEBUG("sys_dbg_read_process_memory()=%i", dbg_r);
    if (dbg_r == CELL_OK && memcmp(elf_header, expected, 4) == 0)
        return can_use_dbg;
#endif

    int mapi_r = ps3mapi_get_process_mem(0x10000, elf_header, 4);
    RB3E_DEBUG("ps3mapi_get_process_mem()=%i", mapi_r);
    if (mapi_r == CELL_OK && memcmp(elf_header, expected, 4) == 0)
    {
        should_use_mapi = 1;
        return can_use_ps3mapi;
    }

    return cant_use_either;
}

void PS3_Write32(uint32_t address, uint32_t value)
{
    uint32_t value_stack = value;
    RB3E_DEBUG("Write: %08x = %08x", address, value);
    if (!should_use_mapi)
        sys_dbg_write_process_memory(address, &value_stack, 4);
    else
        ps3mapi_set_process_mem(address, &value_stack, 4);
}

void PS3_WriteMemory(uint32_t address, void *data, size_t size)
{
    if (!should_use_mapi)
        sys_dbg_write_process_memory(address, data, size);
    else
        ps3mapi_set_process_mem(address, data, size);
}

static uint32_t plugin_toc_base = 0;
void PS3_SetPluginTOCBase(uint32_t toc)
{
    RB3E_DEBUG("Plugin TOC base: %08x", toc);
    plugin_toc_base = toc;
}

static uint32_t game_toc_base = 0;
void PS3_SetGameTOCBase(uint32_t toc)
{
    RB3E_DEBUG("Game TOC base: %08x", toc);
    game_toc_base = toc;
}

// Using a large stub function area, poke a branch that sets our r2 before going to replacement
void PS3_PokeBranch(uint32_t address, uint32_t replacement, char linked)
{
    // get our big ass stub function and take off 3 instructions worth of space
    uint32_t stub_base = PLUGIN_PTR(BigAssStubFunction);
    uint32_t *our_start = (uint32_t *)(stub_base + AssignedStubFuncBytes);
    AssignedStubFuncBytes += (3 * 0x4);

    // a stub to set r2 to the plugin's before jumping to the new function
    PS3_Write32((uint32_t)&our_start[0], LIS(2, (plugin_toc_base >> 16) & 0xFFFF));
    PS3_Write32((uint32_t)&our_start[1], ORI(2, 2, plugin_toc_base & 0xFFFF));
    PS3_Write32((uint32_t)&our_start[2], B(replacement, &our_start[2]));

    // branch from the original address to the new one
    if (linked)
        PS3_Write32(address, BL((uint32_t)our_start, address));
    else
        PS3_Write32(address, B((uint32_t)our_start, address));
}

// Hooks a function, given the original function address, address of a 20-byte stub, and address of the new function.
void HookFunction(unsigned int OriginalAddress, void *StubFunction, void *NewFunction)
{
    unsigned int *orig = (unsigned int *)OriginalAddress;
    unsigned int *stub = (unsigned int *)StubFunction;
    unsigned int *stub_r2 = (unsigned int *)(&stub[2]);

    // a stub to run the original instruction then return
    unsigned int stub_return[2];
    PS3_Write32((uint32_t)&stub[0], orig[0]);
    PS3_Write32((uint32_t)&stub[1], B(&orig[1], &stub[1]));

    // a stub to set r2 to the plugin's before jumping to the new function
    PS3_Write32((uint32_t)&stub_r2[0], LIS(2, (plugin_toc_base >> 16) & 0xFFFF));
    PS3_Write32((uint32_t)&stub_r2[1], ORI(2, 2, plugin_toc_base & 0xFFFF));
    PS3_Write32((uint32_t)&stub_r2[2], B(NewFunction, &stub_r2[2]));

    // branch from the original address to the new one
    PS3_Write32(OriginalAddress, B((uint32_t)stub_r2, OriginalAddress));
}

#endif // RB3E_PS3
