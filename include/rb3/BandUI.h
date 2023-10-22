#ifndef _BANDUI_H
#define _BANDUI_H

#include "UI/UIScreen.h"

// the main UI class for the engine
typedef struct _BandUI
{
    int *vtable;
    char unk[0x4];
    int field_0x8;

    // wii has one 4 less bytes here but otherwise the same
#ifdef RB3E_XBOX
    char unk2[0x20];
    UIScreen *currentScreen;
    char unk3[0x70];
#elif RB3E_WII
    char unk2[0x14];
    UIScreen *currentScreen;
    char unk3[0x78];
#endif

    int *overshellPanel;
    int *eventDialogPanel;
    int *contentLoadingPanel;
    int *passiveMessagesPanel;
    int *saveloadStatusPanel;
    char unk4[0xc];
    int *abstractWipePanel;
} BandUI;

#endif // _BANDUI_H
