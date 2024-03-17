/*
    RB3Enhanced - utilities.c
    Helper functions for managing function hooks.
*/

#ifndef RB3E_PS3

#include "ports.h"
#include "ppcasm.h"

// Hooks a function, given the original function address, address of an 8-byte stub, and address of the new function.
void HookFunction(unsigned int OriginalAddress, void *StubFunction, void *NewFunction)
{
    unsigned int *orig = (unsigned int *)OriginalAddress;
    unsigned int *stub = (unsigned int *)StubFunction;
    stub[0] = orig[0];
    stub[1] = B(&orig[1], &stub[1]);
    orig[0] = B((unsigned int)NewFunction, OriginalAddress);
}

// Un-hooks a function, given the original function address and the address of the stub function used.
void UnhookFunction(unsigned int OriginalAddress, unsigned int *StubFunction)
{
    unsigned int *orig = (unsigned int *)OriginalAddress;
    unsigned int *stub = (unsigned int *)StubFunction;
    orig[0] = stub[0];
}

#endif // RB3E_PS3
