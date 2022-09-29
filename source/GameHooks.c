/*
    RB3Enhanced - GameHooks.c
    Hooks for the Game state.
*/

#ifdef RB3E_XBOX // we only have ports for Xbox right now, unguard when Wii ports are added

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
    Symbol song;
    int song_id;
    SongMetadata *metadata;
    char in_game = 0x01;
    GetSongShortname(&song, *(int *)PORT_THEMETAPERFORMER);
    if (song.sym != NULL)
    {
        RB3E_SendEvent(RB3E_PACKET_SONG_SHORTNAME, song.sym, strlen(song.sym));
        song_id = GetSongIDFromShortname((BandSongMgr *)PORT_THESONGMGR, song.sym, 1);
        metadata = GetMetadata((BandSongMgr *)PORT_THESONGMGR, song_id);
        if (metadata != NULL)
        {
            RB3E_DEBUG("Started song: '%s' - %s (ID: %i, %s)", metadata->title, metadata->artist, metadata->song_id, metadata->shortname);
            RB3E_SendEvent(RB3E_PACKET_SONG_NAME, metadata->title, strlen(metadata->title));
            RB3E_SendEvent(RB3E_PACKET_SONG_ARTIST, metadata->artist, strlen(metadata->artist));
        }
    }
    RB3E_SendEvent(RB3E_PACKET_STATE, &in_game, sizeof(in_game));
    return GameConstruct(theGame);
}

void *GameDestructHook(void *theGame)
{
    char in_game = 0x00;
    RB3E_SendEvent(RB3E_PACKET_STATE, &in_game, sizeof(in_game));
    return GameDestruct(theGame);
}

#endif