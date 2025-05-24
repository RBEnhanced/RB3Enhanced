/*
    RB3Enhanced - DTAFunctions.c
    Custom DTA functions, usable by scripts in-game.
*/

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "ports.h"
#include "GlobalSymbols.h"
#include "config.h"
#include "net_events.h"
#include "rb3/Data.h"
#include "rb3/SongMetadata.h"
#include "rb3/BandSongMgr.h"
#include "MusicLibrary.h"
#include "rb3enhanced.h"
#include "rb3/RockCentralGateway.h"

typedef struct _song_list_vector {
    int *start;
    int *end;
    int unk1;
} song_list_vector;

static song_list_vector dta_song_list = { 0 };
extern char *strtok(char *s, const char *delim);
extern char *strstr(const char *haystack, const char *needle);

DataNode *PrintToDebugger(DataNode *node, DataArray *args)
{
    DataNode *firstArg = DataNodeEvaluate(&args->mNodes->n[1]);
    switch (firstArg->type)
    {
    case FLOAT_VALUE:
        RB3E_MSG("print_debug: %f", firstArg->value.floatVal);
        break;
    case INT_VALUE:
        RB3E_MSG("print_debug: %i", firstArg->value.intVal);
        break;
    case STRING_VALUE:
        RB3E_MSG("print_debug: %s", (char *)*firstArg->value.object);
        break;
    default:
        RB3E_MSG("print_debug: %p", firstArg->value.object);
        break;
    }
    node->type = INT_VALUE;
    node->value.intVal = 1;
    return node;
}

// Get configuration values
DataNode *GetMusicSpeed(DataNode *node, DataArray *args)
{
    node->type = FLOAT_VALUE;
    node->value.floatVal = config.SongSpeedMultiplier;
    return node;
}
DataNode *GetTrackSpeed(DataNode *node, DataArray *args)
{
    node->type = FLOAT_VALUE;
    node->value.floatVal = config.TrackSpeedMultiplier;
    return node;
}
// Set configuration values
DataNode *ChangeMusicSpeed(DataNode *node, DataArray *args)
{
    DataNode *firstArg = DataNodeEvaluate(&args->mNodes->n[1]);
    switch (firstArg->type)
    {
    case FLOAT_VALUE:
        config.SongSpeedMultiplier = firstArg->value.floatVal;
        break;
    case INT_VALUE:
        config.SongSpeedMultiplier = (float)(firstArg->value.intVal);
        break;
    default:
        RB3E_MSG("Invalid type %i for rb3e_change_music_speed", firstArg->type);
        config.SongSpeedMultiplier = 1.0;
        break;
    }
    RB3E_DEBUG("Setting music speed to %.2f", config.SongSpeedMultiplier);
    node->type = INT_VALUE;
    node->value.intVal = 1;
    return node;
}
DataNode *ChangeTrackSpeed(DataNode *node, DataArray *args)
{
    DataNode *firstArg = DataNodeEvaluate(&args->mNodes->n[1]);
    switch (firstArg->type)
    {
    case FLOAT_VALUE:
        config.TrackSpeedMultiplier = firstArg->value.floatVal;
        break;
    case INT_VALUE:
        config.TrackSpeedMultiplier = (float)(firstArg->value.intVal);
        break;
    default:
        RB3E_MSG("Invalid type %i for rb3e_change_track_speed", firstArg->type);
        config.TrackSpeedMultiplier = 1.0;
        break;
    }
    RB3E_DEBUG("Setting track speed to %.2f", config.TrackSpeedMultiplier);
    node->type = INT_VALUE;
    node->value.intVal = 1;
    return node;
}
DataNode *DTASetVenue(DataNode *node, DataArray *args)
{
    DataNode *firstArg = DataNodeEvaluate(&args->mNodes->n[1]);
    char *venue;
    switch (firstArg->type)
    {
    case STRING_VALUE:
        venue = (char *)*firstArg->value.object;
        if (venue == NULL || strlen(venue) > 29)
        {
            RB3E_MSG("Invalid venue string value for rb3e_set_venue", NULL);
        }
        else
        {
            strncpy(config.ForcedVenue, venue, RB3E_MAX_CONFIG_LEN);
        }
        break;
    default:
        RB3E_MSG("Invalid type %i for rb3e_set_venue", firstArg->type);
        break;
    }
    node->type = INT_VALUE;
    node->value.intVal = 1;
    return node;
}

DataNode *DTAIsEmulator(DataNode *node, DataArray *args)
{
    node->type = INT_VALUE;
    node->value.intVal = RB3E_IsEmulator();
    return node;
}

DataNode *DTARelaunchGame(DataNode *node, DataArray *args)
{
    node->type = INT_VALUE;
    node->value.intVal = RB3E_RelaunchGame();
    return node;
}

DataNode *DTAGetSongCount(DataNode *node, DataArray *args)
{
    node->type = INT_VALUE;
    node->value.intVal = RB3E_LoadedSongCount;
    return node;
}

DataNode *DTASendModData(DataNode *node, DataArray *args)
{
    DataNode *firstArg = DataNodeEvaluate(&args->mNodes->n[1]);
    DataNode *secondArg = DataNodeEvaluate(&args->mNodes->n[2]);
    node->type = INT_VALUE;
    node->value.intVal = 0;
    // TODO(Emma): allow sending integer/float/etc second arguments? other things as pointers?
    if (firstArg->type != STRING_VALUE || secondArg->type != STRING_VALUE)
        RB3E_MSG("Invalid types for arguments to rb3e_send_mod_data! 1st: %i, 2nd: %i", firstArg->type, secondArg->type);
    else
    {
        RB3E_EventModData mod_event = {0};
        char *id_tag = (char *)*firstArg->value.object;
        char *data = (char *)*secondArg->value.object;
        if (id_tag == NULL || strlen(id_tag) > sizeof(mod_event.IdentifyValue))
            RB3E_MSG("Invalid identify string value for rb3e_send_mod_data! %p", id_tag);
        else if (data == NULL || strlen(data) > sizeof(mod_event.String))
            RB3E_MSG("Invalid data string value for rb3e_send_mod_data! %p", data);
        else // assume it's fine
        {
            strncpy(mod_event.IdentifyValue, id_tag, sizeof(mod_event.IdentifyValue));
            strncpy(mod_event.String, data, sizeof(mod_event.String));
            RB3E_SendEvent(RB3E_EVENT_DX_DATA, &mod_event, sizeof(mod_event));
            node->value.intVal = 1;
        }
    }
    return node;
}

DataNode *DTAGetSongName(DataNode *node, DataArray *args)
{
    // return "Darude Sandstorm";
    DataNode *firstArg = DataNodeEvaluate(&args->mNodes->n[1]);
    Symbol rb3e_no_song_name;
    SymbolConstruct(&rb3e_no_song_name, "rb3e_no_song_name");
    node->type = SYMBOL;
    node->value.string = rb3e_no_song_name.sym;
    if (firstArg->type != INT_VALUE)
        RB3E_MSG("Invalid types for arguments to rb3e_get_song_name! 1st: %i", firstArg->type);
    else
    {
        SongMetadata *songmet = GetMetadata((BandSongMgr *)PORT_THESONGMGR, firstArg->value.intVal);
        RB3E_DEBUG("rb3e_get_song_name %i", firstArg->value.intVal);
        if (songmet == NULL) {
            RB3E_MSG("!! FAILED TO GET SONG METADATA FOR %i !!", firstArg->value.intVal);
        } else {
            Symbol titleSym;
            SymbolConstruct(&titleSym, songmet->title.buf);
            node->value.string = titleSym.sym;
        }
    }
    return node;
}

DataNode *DTAGetArtist(DataNode *node, DataArray *args)
{
    DataNode *firstArg = DataNodeEvaluate(&args->mNodes->n[1]);
    Symbol rb3e_no_artist;
    SymbolConstruct(&rb3e_no_artist, "rb3e_no_artist");
    node->type = SYMBOL;
    node->value.string = rb3e_no_artist.sym;
    if (firstArg->type != INT_VALUE)
        RB3E_MSG("Invalid types for arguments to rb3e_get_artist! 1st: %i", firstArg->type);
    else
    {
        SongMetadata *songmet = GetMetadata((BandSongMgr *)PORT_THESONGMGR, firstArg->value.intVal);
        RB3E_DEBUG("rb3e_get_artist %i", firstArg->value.intVal);
        if (songmet == NULL) {
            RB3E_MSG("!! FAILED TO GET SONG METADATA FOR %i !!", firstArg->value.intVal);
        } else {
            Symbol artistSym;
            SymbolConstruct(&artistSym, songmet->artist.buf);
            node->value.string = artistSym.sym;
        }
    }
    return node;
}

DataNode *DTAGetAlbum(DataNode *node, DataArray *args)
{
    DataNode *firstArg = DataNodeEvaluate(&args->mNodes->n[1]);
    Symbol rb3e_no_album;
    SymbolConstruct(&rb3e_no_album, "rb3e_no_album");
    node->type = SYMBOL;
    node->value.string = rb3e_no_album.sym;
    if (firstArg->type != INT_VALUE)
        RB3E_MSG("Invalid types for arguments to rb3e_get_album! 1st: %i", firstArg->type);
    else
    {
        SongMetadata *songmet = GetMetadata((BandSongMgr *)PORT_THESONGMGR, firstArg->value.intVal);
        RB3E_DEBUG("rb3e_get_album %i", firstArg->value.intVal);
        if (songmet == NULL) {
            RB3E_MSG("!! FAILED TO GET SONG METADATA FOR %i !!", firstArg->value.intVal);
        } else {
            Symbol albumSym;
            SymbolConstruct(&albumSym, songmet->album.buf);
            node->value.string = albumSym.sym;
        }
    }
    return node;
}

DataNode *DTAGetGenre(DataNode *node, DataArray *args)
{
    DataNode *firstArg = DataNodeEvaluate(&args->mNodes->n[1]);
    Symbol rb3e_no_genre;
    SymbolConstruct(&rb3e_no_genre, "rb3e_no_genre");
    node->type = SYMBOL;
    node->value.string = rb3e_no_genre.sym;
    if (firstArg->type != INT_VALUE)
        RB3E_MSG("Invalid types for arguments to rb3e_no_genre! 1st: %i", firstArg->type);
    else
    {
        SongMetadata *songmet = GetMetadata((BandSongMgr *)PORT_THESONGMGR, firstArg->value.intVal);
        RB3E_DEBUG("rb3e_get_genre %i", firstArg->value.intVal);
        if (songmet == NULL) {
            RB3E_MSG("!! FAILED TO GET SONG METADATA FOR %i !!", firstArg->value.intVal);
        } else {
            node->value.string = songmet->genre.sym;
        }
    }
    return node;
}

DataNode *DTAGetOrigin(DataNode *node, DataArray *args)
{
    DataNode *firstArg = DataNodeEvaluate(&args->mNodes->n[1]);
    Symbol rb3e_no_origin;
    SymbolConstruct(&rb3e_no_origin, "rb3e_no_origin");
    node->type = SYMBOL;
    node->value.string = rb3e_no_origin.sym;
    if (firstArg->type != INT_VALUE)
        RB3E_MSG("Invalid types for arguments to rb3e_no_origin! 1st: %i", firstArg->type);
    else
    {
        SongMetadata *songmet = GetMetadata((BandSongMgr *)PORT_THESONGMGR, firstArg->value.intVal);
        RB3E_DEBUG("rb3e_no_origin %i", firstArg->value.intVal);
        if (songmet == NULL) {
            RB3E_MSG("!! FAILED TO GET SONG METADATA FOR %i !!", firstArg->value.intVal);
        } else {
            node->value.string = songmet->gameOrigin.sym;
        }
    }
    return node;
}

static void sanitize_string(const char *input, char *output, size_t out_size)
{
    size_t j = 0;
    size_t i;
    for (i = 0; input[i] && j + 1 < out_size; ++i) {
        unsigned char c = input[i];
        if (isalnum(c) || isspace(c)) {
            output[j++] = (char)tolower(c);
        }
    }
    while (j > 0 && isspace((unsigned char)output[j - 1])) --j;
    output[j] = '\0';
}

DataNode *DTA_SearchSongName(DataNode *node, DataArray *args) {
    DataNode *firstArg;
    Symbol sym_failed;
    const char *needle;
    int *list;
    int count;
    int i, j;
    int matchedByTitle = 0, allFound = 0;
    SongMetadata *md;
    char cleanNeedle[64], cleanTitle[256], cleanArtist[256], tempBuf[64];
    char *tokens[16], *tok;
    int tokenCount;

    SymbolConstruct(&sym_failed, "rb3e_search_failed");
    node->type = SYMBOL; node->value.string = sym_failed.sym;

    firstArg = DataNodeEvaluate(&args->mNodes->n[1]);
    if (firstArg->type != STRING_VALUE) return node;
    needle = *((const char **)firstArg->value.object);

    sanitize_string(needle, cleanNeedle, sizeof(cleanNeedle));

    if (!dta_song_list.start)
        SongMgrGetRankedSongs((BandSongMgr *)PORT_THESONGMGR, &dta_song_list, 0, 0);
    list = dta_song_list.start;
    count = (int)(dta_song_list.end - list);

    tokenCount = 0;
    strncpy(tempBuf, cleanNeedle, sizeof(tempBuf)); tempBuf[sizeof(tempBuf)-1] = '\0';
    tok = strtok(tempBuf, " \t");
    while (tok && tokenCount < 16) { tokens[tokenCount++] = tok; tok = strtok(NULL, " \t"); }

    for (i = 0; i < count; ++i) {
        md = GetMetadata((BandSongMgr *)PORT_THESONGMGR, list[i]);
        if (!md) continue;

        sanitize_string(md->title.buf, cleanTitle, sizeof(cleanTitle));
        sanitize_string(md->artist.buf, cleanArtist, sizeof(cleanArtist));

        if (strcmp(cleanTitle, cleanNeedle) == 0) matchedByTitle = 0;
        else if (strcmp(cleanArtist, cleanNeedle) == 0) matchedByTitle = 1;
        else {
            allFound = 1;
            for (j = 0; j < tokenCount; ++j) {
                if (strstr(cleanTitle, tokens[j])) { if (j==0) matchedByTitle = 0; }
                else if (strstr(cleanArtist, tokens[j])) { if (j==0) matchedByTitle = 1; }
                else { allFound = 0; break; }
            }
            if (!allFound) continue;
        }

        ExecuteDTA(PORT_ROCKCENTRALGATEWAY, "{{music_library get view_settings_provider} refresh_all_settings}");
        ExecuteDTA(PORT_ROCKCENTRALGATEWAY, "{song_select_filter_panel filter_enter}");
        ExecuteDTA(PORT_ROCKCENTRALGATEWAY, "{{music_library get view_settings_provider} select_setting 1}");
        {
            char buf[64];
            sprintf(buf, "{{music_library get view_settings_provider} select_setting_option %d}", matchedByTitle);
            ExecuteDTA(PORT_ROCKCENTRALGATEWAY, buf);
        }
        ExecuteDTA(PORT_ROCKCENTRALGATEWAY, "{music_library report_sort_and_filters}");
        ExecuteDTA(PORT_ROCKCENTRALGATEWAY, "{song_select_filter_panel filter_exit}");

        if (matchedByTitle) {
            MusicLibrarySelectHeading(md->artist.buf);
        } else {
            MusicLibrarySelectSong(md->shortname.sym);
        }
        node->value.string = md->shortname.sym;
        return node;
    }

    return node;
}

#ifdef RB3E_XBOX
// this function is inlined on the Xbox version, so we re-create it
void DataRegisterFunc(Symbol name, DTAFunction_t func)
{
    *(DTAFunction_t *)HmxFactoryFuncAt((int *)PORT_GDATAFUNCS, &name) = func;
}
#endif

void AddDTAFunctions()
{
    DataRegisterFunc(globalSymbols.print_debug, PrintToDebugger);
    DataRegisterFunc(globalSymbols.rb3e_change_music_speed, ChangeMusicSpeed);
    DataRegisterFunc(globalSymbols.rb3e_change_track_speed, ChangeTrackSpeed);
    DataRegisterFunc(globalSymbols.rb3e_get_music_speed, GetMusicSpeed);
    DataRegisterFunc(globalSymbols.rb3e_get_track_speed, GetTrackSpeed);
    DataRegisterFunc(globalSymbols.rb3e_set_venue, DTASetVenue);
    DataRegisterFunc(globalSymbols.rb3e_is_emulator, DTAIsEmulator);
    DataRegisterFunc(globalSymbols.rb3e_relaunch_game, DTARelaunchGame);
    DataRegisterFunc(globalSymbols.rb3e_get_song_count, DTAGetSongCount);
    DataRegisterFunc(globalSymbols.rb3e_send_event_string, DTASendModData);
    DataRegisterFunc(globalSymbols.rb3e_get_song_name, DTAGetSongName);
    DataRegisterFunc(globalSymbols.rb3e_get_artist, DTAGetArtist);
    DataRegisterFunc(globalSymbols.rb3e_get_album, DTAGetAlbum);
    DataRegisterFunc(globalSymbols.rb3e_get_origin, DTAGetOrigin);
    DataRegisterFunc(globalSymbols.rb3e_get_genre, DTAGetGenre);
    DataRegisterFunc(globalSymbols.rb3e_search_song_name, DTA_SearchSongName);
    RB3E_MSG("Added DTA functions!", NULL);
}
