#ifndef _UIPANEL_H
#define _UIPANEL_H

typedef struct _UIPanel UIPanel;

struct _UIPanel
{
    int *vtable; // 0x0
    int unk1;    // 0x4
    int unk2;    // 0x8
    int unk3;    // 0xc
    int unk4;    // 0x10
    int unk5;    // 0x14
    int unk6;    // 0x18
#ifdef RB3E_WII
    int is_up; // 0x1c
#else
    int unk7;  // 0x1c
    int is_up; // 0x20
#endif
};

#endif // _UIPANEL_H