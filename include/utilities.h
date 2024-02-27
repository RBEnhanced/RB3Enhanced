/*
    RB3Enhanced - utilities.h
    Helper functions for managing function hooks.
*/

void HookFunction(unsigned int OriginalAddress, void *StubFunction, void *NewFunction);
void UnhookFunction(unsigned int *OriginalAddress, unsigned int *StubFunction);

#ifdef RB3E_PS3
// utilities for working with PS3
void PS3_Write32(uint32_t address, uint32_t value);
void PS3_WriteMemory(uint32_t address, void *data, size_t size);
void PS3_SetGameTOCBase(uint32_t toc);
void PS3_SetPluginTOCBase(uint32_t toc);
void PS3_PokeBranch(uint32_t address, uint32_t replacement, char linked);
#define POKE_PLUGIN_B(addr, dest) PS3_PokeBranch(addr, dest, 0)
#define POKE_PLUGIN_BL(addr, dest) PS3_PokeBranch(addr, dest, 1)
#undef POKE_32
#define POKE_32 PS3_Write32
#else
#define POKE_PLUGIN_B POKE_B
#define POKE_PLUGIN_BL POKE_BL
#endif

#define POKE_STUB(addr, dest) POKE_B(PLUGIN_PTR(addr), dest)
