/*
    RB3Enhanced - RndPropAnimHooks.c
    Hooks for the RndPropAnim class.
*/

#include "rb3e_include.h"
#include "RndPropAnimHooks.h"

// this hook exists to fix the bug where the pause menu will not appear when playing an instrument not normally supported by your controller type (drums on keys, etc.)
// see https://github.com/RBEnhanced/RB3Enhanced/issues/2
void PropAnimSetFrameHook(RndPropAnim *rndPropAnim, float frame, float time)
{
    if (strcmp(rndPropAnim->object.name, "slot_positions.anim") == 0)
    {
        // check that it is not setting the frame to 0 (which is the default slot positions for overshell)
        if (frame != 0.0f)
        {
            // set the frame to 2 so pause menus will appear when playing on the wrong instrument type
            // TODO: Get rid of this hook and figure out how to properly fix this
            PropAnimSetFrame(rndPropAnim, 2.0f, 1.0f);
            return;
        }
    }
    PropAnimSetFrame(rndPropAnim, frame, time);
}