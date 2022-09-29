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
#include "rb3/BandSongMgr.h"
#include "rb3/MetaPerformer.h"
#include "rb3enhanced.h"

void *GameConstructHook(void *theGame) // You just lost
{
    char in_game = 0x01;
    // TODO(Emma): Port addresses to Wii
#ifdef RB3E_XBOX
    Symbol song;
    int song_id;
    SongMetadata *metadata;
    GetSongShortname(&song, *(int *)PORT_THEMETAPERFORMER);
    if (song.sym != NULL)
    {
        RB3E_SendEvent(RB3E_EVENT_SONG_SHORTNAME, song.sym, strlen(song.sym));
        song_id = GetSongIDFromShortname((BandSongMgr *)PORT_THESONGMGR, song.sym, 1);
        RB3E_DEBUG("GetSongIDFromShortname(%s) = %i", song.sym, song_id);
        metadata = GetMetadata((BandSongMgr *)PORT_THESONGMGR, song_id);
        if (metadata != NULL)
        {
            RB3E_DEBUG("Started song: '%s' - %s (ID: %i, %s)", metadata->title.buf, metadata->artist.buf, metadata->song_id, metadata->shortname);
            RB3E_SendEvent(RB3E_EVENT_SONG_NAME, metadata->title.buf, metadata->title.length);
            RB3E_SendEvent(RB3E_EVENT_SONG_ARTIST, metadata->artist.buf, metadata->artist.length);
        }
    }
#endif
    RB3E_SendEvent(RB3E_EVENT_STATE, &in_game, sizeof(in_game));
    return GameConstruct(theGame);
}

void *GameDestructHook(void *theGame, int r4)
{
    char in_game = 0x00;
    RB3E_SendEvent(RB3E_EVENT_STATE, &in_game, sizeof(in_game));
    return GameDestruct(theGame, r4);
}
