#ifndef _BANDUI_H
#define _BANDUI_H

// the main UI class for the engine
typedef struct _BandUI
{
    char unk[0x8];
    int field_0x8;

    // wii has one 4 less bytes here but otherwise the same
#ifdef RB3E_XBOX
    char unk2[0x94];
#elif RB3E_WII
    char unk2[0x90];
#endif

    int *overshellPanel;
    int *eventDialogPanel;
    int *contentLoadingPanel;
    int *passiveMessagesPanel;
    int *saveloadStatusPanel;
    char unk3[0xc];
    int *abstractWipePanel;
} BandUI;

#endif // _BANDUI_H