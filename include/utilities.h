/*
    RB3Enhanced - utilities.h
    Helper functions for managing function hooks.
*/

void HookFunction(unsigned int OriginalAddress, void *StubFunction, void *NewFunction);
void UnhookFunction(unsigned int *OriginalAddress, unsigned int *StubFunction);