/*
    RB3Enhanced - ports.h
    Defines port addresses and platform-specific function definitions.
*/

#include "ports_xbox360.h"
#include "ports_wii.h"
#include "ports_ps3.h"
#include "ports_wii_bank8.h"

#define RB3E_MSG(x, ...) RB3E_PRINT("[RB3E:MSG] " x "\n", __VA_ARGS__)
#ifdef RB3EDEBUG
#define RB3E_DEBUG(x, ...) RB3E_PRINT("[RB3E:DBG] " x "\n", __VA_ARGS__)
#else
#define RB3E_DEBUG(x, ...)
#endif // RB3EDEBUG
