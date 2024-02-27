/*
    RB3Enhanced - DTAFunctions.c
    Custom DTA functions, usable by scripts in-game.
*/

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "ports.h"
#include "GlobalSymbols.h"
#include "config.h"
#include "net_events.h"
#include "rb3/Data.h"
#include "rb3enhanced.h"

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
DataNode *GetMusicSpeed(DataNode *node, int *args)
{
    node->type = FLOAT_VALUE;
    node->value.floatVal = config.SongSpeedMultiplier;
    return node;
}
DataNode *GetTrackSpeed(DataNode *node, int *args)
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

DataNode *DTAIsEmulator(DataNode *node, int *args)
{
    node->type = INT_VALUE;
    node->value.intVal = RB3E_IsEmulator();
    return node;
}

DataNode *DTARelaunchGame(DataNode *node, int *args)
{
    node->type = INT_VALUE;
    node->value.intVal = RB3E_RelaunchGame();
    return node;
}

DataNode *DTAGetSongCount(DataNode *node, int *args)
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

void AddDTAFunctions()
{
#ifndef RB3E_PS3
    *(int *)HmxFactoryFuncAt((int *)PORT_GDATAFUNCS, &globalSymbols.print_debug) = (int)PrintToDebugger;
    *(int *)HmxFactoryFuncAt((int *)PORT_GDATAFUNCS, &globalSymbols.rb3e_change_music_speed) = (int)ChangeMusicSpeed;
    *(int *)HmxFactoryFuncAt((int *)PORT_GDATAFUNCS, &globalSymbols.rb3e_change_track_speed) = (int)ChangeTrackSpeed;
    *(int *)HmxFactoryFuncAt((int *)PORT_GDATAFUNCS, &globalSymbols.rb3e_get_music_speed) = (int)GetMusicSpeed;
    *(int *)HmxFactoryFuncAt((int *)PORT_GDATAFUNCS, &globalSymbols.rb3e_get_track_speed) = (int)GetTrackSpeed;
    *(int *)HmxFactoryFuncAt((int *)PORT_GDATAFUNCS, &globalSymbols.rb3e_set_venue) = (int)DTASetVenue;
    *(int *)HmxFactoryFuncAt((int *)PORT_GDATAFUNCS, &globalSymbols.rb3e_is_emulator) = (int)DTAIsEmulator;
    *(int *)HmxFactoryFuncAt((int *)PORT_GDATAFUNCS, &globalSymbols.rb3e_relaunch_game) = (int)DTARelaunchGame;
    *(int *)HmxFactoryFuncAt((int *)PORT_GDATAFUNCS, &globalSymbols.rb3e_get_song_count) = (int)DTAGetSongCount;
    *(int *)HmxFactoryFuncAt((int *)PORT_GDATAFUNCS, &globalSymbols.rb3e_send_event_string) = (int)DTASendModData;
#else // on PS3 it's completely different
    // TODO(Emma): check if these actually work
    AddDTAFunctionPS3(&globalSymbols.print_debug, &PrintToDebugger);
    AddDTAFunctionPS3(&globalSymbols.rb3e_change_music_speed, &ChangeMusicSpeed);
    AddDTAFunctionPS3(&globalSymbols.rb3e_change_track_speed, &ChangeTrackSpeed);
    AddDTAFunctionPS3(&globalSymbols.rb3e_get_music_speed, &GetMusicSpeed);
    AddDTAFunctionPS3(&globalSymbols.rb3e_get_track_speed, &GetTrackSpeed);
    AddDTAFunctionPS3(&globalSymbols.rb3e_set_venue, &DTASetVenue);
    AddDTAFunctionPS3(&globalSymbols.rb3e_is_emulator, &DTAIsEmulator);
    AddDTAFunctionPS3(&globalSymbols.rb3e_relaunch_game, &DTARelaunchGame);
    AddDTAFunctionPS3(&globalSymbols.rb3e_get_song_count, &DTAGetSongCount);
    AddDTAFunctionPS3(&globalSymbols.rb3e_send_event_string, &DTASendModData);
#endif
    RB3E_MSG("Added DTA functions!", NULL);
}