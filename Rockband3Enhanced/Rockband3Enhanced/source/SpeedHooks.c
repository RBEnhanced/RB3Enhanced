/*
    RB3Enhanced - SpeedHooks.c
    Hooks for track/song speed changes.
*/

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "../include/ports.h"
#include "../include/config.h"
#include "../include/rb3/TrackPanelDirBase.h"
#include "../include/rb3/Game.h"

void SetMusicSpeedHook(void *thisGame, float speed)
{
    // do not change song speed in practice
    if (speed == 1.0)
        speed = config.SongSpeedMultiplier;
    RB3E_DEBUG("Music speed: %.2f", speed);
    SetMusicSpeed(thisGame, speed);
}

void UpdateTrackSpeedHook(void *thisTrackPanelDirBase, float speed)
{
    speed = speed / config.TrackSpeedMultiplier;
    RB3E_DEBUG("Track speed: %.2f", speed);
    UpdateTrackSpeed(thisTrackPanelDirBase, speed);
}