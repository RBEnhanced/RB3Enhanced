#ifndef _JOYPAD_H
#define _JOYPAD_H

typedef struct _PadData
{
#ifdef RB3E_XBOX
    unsigned char unk[0xd4];
#elif RB3E_WII
    unsigned char unk[0x9c];
#endif
} PadData;
extern PadData *JoypadGetPadData(int index);

#ifdef RB3E_XBOX
#include <xtl.h>
extern int JoypadGetCachedXInputCaps(int index, XINPUT_CAPABILITIES *out_caps, int force_update);
#endif

#endif // _JOYPAD_H
