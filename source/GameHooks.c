/*
    RB3Enhanced - GameHooks.c
    Hooks for the Game state.
*/

#include <string.h>
#include <stdio.h>
#include "ports.h"
#include "net_events.h"
#include "rb3/Game.h"
#include "rb3/Symbol.h"
#include "rb3/SongMetadata.h"
#include "rb3/BandUser.h"
#include "rb3/BandUserMgr.h"
#include "rb3/BandSongMgr.h"
#include "rb3/MetaPerformer.h"
#include "rb3enhanced.h"

void *GameConstructHook(void *theGame) // You just lost
{
    char in_game = 0x01;
    int song_id;
    int i;
    SongMetadata *metadata;
    BandUser *bandUser;
    RB3E_EventBandInfo bandevent = {0};
#ifdef RB3E_XBOX
    Symbol song;
    GetSongSymbol(&song, *(int *)PORT_THEMETAPERFORMER);
#else
    Symbol song = GetSongSymbol(*(int *)PORT_THEMETAPERFORMER);
#endif
    if (song.sym != NULL)
    {
        RB3E_SendEvent(RB3E_EVENT_SONG_SHORTNAME, song.sym, strlen(song.sym));
        song_id = GetSongIDFromShortname((BandSongMgr *)PORT_THESONGMGR, song, 1);
        RB3E_DEBUG("GetSongIDFromShortname(%s) = %i", song.sym, song_id);
        metadata = GetMetadata((BandSongMgr *)PORT_THESONGMGR, song_id);
        if (metadata != NULL)
        {
            RB3E_DEBUG("Metadata: %p", metadata);
            RB3E_DEBUG("Started song: '%s' - %s (ID: %i, %s)", metadata->title.buf, metadata->artist.buf, metadata->mSongID, metadata->mShortName);
            RB3E_SendEvent(RB3E_EVENT_SONG_NAME, metadata->title.buf, metadata->title.length);
            RB3E_SendEvent(RB3E_EVENT_SONG_ARTIST, metadata->artist.buf, metadata->artist.length);
        }
    }
    for (i = 0; i < 4; i++)
    {
        bandUser = GetBandUserFromSlot(*(int *)PORT_THEBANDUSERMGR, i);
        if (bandUser != NULL)
        {
            RB3E_DEBUG("BandUser %i: %p - Track: %i, Controller: %i, Difficulty: %i", i, bandUser, bandUser->mTrackType, bandUser->mControllerType, bandUser->mDifficulty);
            bandevent.MemberExists[i] = 1;
            bandevent.Difficulty[i] = bandUser->mDifficulty;
            bandevent.TrackType[i] = bandUser->mTrackType;
        }
        else
        {
            bandevent.MemberExists[i] = 0;
        }
    }
    RB3E_SendEvent(RB3E_EVENT_BAND_INFO, &bandevent, sizeof(bandevent));
    RB3E_SendEvent(RB3E_EVENT_STATE, &in_game, sizeof(in_game));
    return GameConstruct(theGame);
}

void *GameDestructHook(void *theGame, int r4)
{
    char in_game = 0x00;
    RB3E_SendEvent(RB3E_EVENT_STATE, &in_game, sizeof(in_game));
    
#ifdef RB3E_XBOX
    // When not in-game, turn off the stage-kit lights.
    // Fixes the RB3 bug(?) that leaves 2 red leds on when exiting the score view screen.
    StagekitSetStateHook(0x00, 0xFF);    
#endif

    return GameDestruct(theGame, r4);
}
