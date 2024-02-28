/*
    RB3Enhanced - ps3_crypto.c
    Loading unencrypted files on RB3 PS3.
*/

#ifdef RB3E_PS3

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "ports.h"
#include "ppcasm.h"
#include "utilities.h"

int ctr_decrypt(char *input, char *output, int size, void *ctr_state);
int ctr_decrypt_hook(char *input, char *output, int size, void *ctr_state)
{
    // if ctr_state is NULL then the mogg is unencrypted, just read input into output
    if (ctr_state == NULL)
    {
        memcpy(output, input, size);
        return 0;
    }
    // do a proper decryption
    return ctr_decrypt(input, output, size, ctr_state);
}

void InitCryptoHooks()
{
    POKE_32(PORT_PS3_VORBISREADER_CTR_CHECK, NOP);
    HookFunction(PORT_CTR_DECRYPT, PLUGIN_PTR(ctr_decrypt), PLUGIN_PTR(ctr_decrypt_hook));

    // fallthrough after sceNpDrmIsAvailable doesn't return CELL_OK when reading .mid.edat files
    // would normally call SetErrorCode(?), we change it to just run cellFsOpen normally
    POKE_32(PORT_NPDRM_UNAVAILABLE_FALLTHROUGH + 0, LI(6, 0));              // set arg to NULL, disables npdrm opening
    POKE_32(PORT_NPDRM_UNAVAILABLE_FALLTHROUGH + 4, LI(7, 0));              // set arg size to 0, disables npdrm opening
    POKE_B(PORT_NPDRM_UNAVAILABLE_FALLTHROUGH + 8, PORT_NPDRM_FSOPEN_JUMP); // jumps to cellFsOpen call
    // redirect to other SetErrorCode(?) after doing npdrm cellFsOpen
    POKE_B(PORT_NPDRM_FSOPEN_RETURN, PORT_NPDRM_SETERRORCODE_JUMP);

    RB3E_MSG("Initialised decrypted content hooks!", NULL);
}

#endif