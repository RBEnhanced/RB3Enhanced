/*
    RB3Enhanced - xbox360_crypto.c
    Corona/devkit crypto fix.
*/

#ifdef RB3E_XBOX

#include <xtl.h>
#include "../include/xbox360.h"
#include "../include/ports.h"
#include "../include/ppcasm.h"

// Deobscured keyset
BYTE NewKeyset[64] = {0x01, 0x22, 0x00, 0x38, 0xd2, 0x01, 0x78, 0x8b, 0xdd, 0xcd, 0xd0, 0xf0, 0xfe, 0x3e, 0x24, 0x7f, 0x51, 0x73, 0xad, 0xe5, 0xb3, 0x99, 0xb8, 0x61, 0x58, 0x1a, 0xf9, 0xb8, 0x1e, 0xa7, 0xbe, 0xbf, 0xc6, 0x22, 0x94, 0x30, 0xd8, 0x3c, 0x84, 0x14, 0x08, 0x73, 0x7c, 0xf2, 0x23, 0xf6, 0xeb, 0x5a, 0x02, 0x1a, 0x83, 0xf3, 0x97, 0xe9, 0xd4, 0xb8, 0x06, 0x74, 0x14, 0x6b, 0x30, 0x4c, 0x00, 0x91};
// This is meant to be a struct, but we don't need to define it.
BYTE OurAesState[0x160] = {0};
BYTE Feed[0x10] = {0};

DWORD XeKeysSetKeyHook(WORD KeyId, DWORD KeyBuffer, WORD KeyLength)
{
    // Hacky way to get the key offset, subtract their base address and add ours
    KeyBuffer -= 0x82c76258;
    KeyBuffer += (DWORD)&NewKeyset;
    // Clean up our existing state.
    memset(&OurAesState, 0, 0x160);
    memset(Feed, 0, 0x10);
    // Redirect call to XeCrypt (which doesn't use XeKeys)
    XeCryptAesKey(OurAesState, (PBYTE)KeyBuffer);
    return 0;
}
DWORD XeKeysAesCbcHook(DWORD KeyId, const PBYTE InBuffer, DWORD InLength, PBYTE OutBuffer, PBYTE IV, BOOL Encrypt)
{
    // Redirect call to XeCrypt (which doesn't use XeKeys)
    XeCryptAesCbc(OurAesState, InBuffer, InLength, OutBuffer, Feed, Encrypt);
    return 0;
}

void InitCryptoHooks()
{
    POKE_B(PORT_XEKEYSSETKEY_STUB, &XeKeysSetKeyHook);
    POKE_B(PORT_XEKEYSAESCBC_STUB, &XeKeysAesCbcHook);
    RB3E_MSG("Initialised XeKeys->XeCrypt hooks!", NULL);
}

#endif