/*
    RB3Enhanced - _functions.c
    Definitions used by the compiler to call/hook functions.
    This file should not be included by any other file, to avoid conflicts.
*/

#ifdef RB3E_PS3

// we have a big block of toc, to offset ours and paste in the original game's
// see BeforeStartupHook in ps3.c
char filler_toc[0xA000] __attribute__((section(".toc")));

#endif // RB3E_PS3
