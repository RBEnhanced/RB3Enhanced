/*
    RB3Enhanced - xbox360_input.c
    Xbox 360 controller detection/interaction functions.
*/

#ifdef RB3E_XBOX

#include <xtl.h>
#include "ports.h"
#include "utilities.h"
#include "rb3/Joypad.h"

// TODO(Emma): clean up this whole file/impl, this was just a quick test really

typedef struct _SantrollerRevision
{
    char device_type;
    char rhythm_type : 4;
    char console_type : 4;
} SantrollerRevision;

static BOOL controller_is_santroller[4] = {0};
static BOOL has_detected[4] = {0};

int JoypadGetCachedXInputCapsHook(int index, XINPUT_CAPABILITIES *out_caps, int force_update)
{
    int r = JoypadGetCachedXInputCaps(index, out_caps, force_update);
    if (r == TRUE && out_caps != NULL)
    {
        if (out_caps->Gamepad.sThumbLX == 0x1209 && out_caps->Gamepad.sThumbLY == 0x2882)
        {
            if (controller_is_santroller[index] == FALSE)
            {
                SantrollerRevision *revision = (SantrollerRevision *)&out_caps->Gamepad.sThumbRX;
                RB3E_DEBUG("Santroller detected at index %i, type %02x, rhythm type %01x, console type %01x", index, revision->console_type, revision->rhythm_type, revision->rhythm_type);
                controller_is_santroller[index] = TRUE;
                has_detected[index] = TRUE;
            }
        }
        else
        {
            if (!has_detected[index] || controller_is_santroller[index])
            {
                RB3E_DEBUG("%i is not santroller, type %02x", index, out_caps->SubType);
                controller_is_santroller[index] = FALSE;
                has_detected[index] = TRUE;
            }
        }
    }
    return r;
}

void InitInputHooks()
{
    HookFunction(PORT_JOYPADGETCACHEDXINPUTCAPS, &JoypadGetCachedXInputCaps, &JoypadGetCachedXInputCapsHook);
}

#endif
