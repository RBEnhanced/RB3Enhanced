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
#include "rb3/Data.h"

// TODO: figure out what the type of args truly is
DataNode *PrintToDebugger(DataNode *node, int *args)
{
    DataNode *firstArg = DataNodeEvaluate((DataNode *)(*args + 0x8));
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
DataNode *ChangeMusicSpeed(DataNode *node, int *args)
{
    DataNode *firstArg = DataNodeEvaluate((DataNode *)(*args + 0x8));
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
DataNode *ChangeTrackSpeed(DataNode *node, int *args)
{
    DataNode *firstArg = DataNodeEvaluate((DataNode *)(*args + 0x8));
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
DataNode *DTASetVenue(DataNode *node, int *args)
{
    DataNode *firstArg = DataNodeEvaluate((DataNode *)(*args + 0x8));
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

int AddDTAFunctions(int r3_presv)
{
    *(int *)HmxFactoryFuncAt((int *)PORT_GDATAFUNCS, &globalSymbols.print_debug) = (int)PrintToDebugger;
    *(int *)HmxFactoryFuncAt((int *)PORT_GDATAFUNCS, &globalSymbols.rb3e_change_music_speed) = (int)ChangeMusicSpeed;
    *(int *)HmxFactoryFuncAt((int *)PORT_GDATAFUNCS, &globalSymbols.rb3e_change_track_speed) = (int)ChangeTrackSpeed;
    *(int *)HmxFactoryFuncAt((int *)PORT_GDATAFUNCS, &globalSymbols.rb3e_get_music_speed) = (int)GetMusicSpeed;
    *(int *)HmxFactoryFuncAt((int *)PORT_GDATAFUNCS, &globalSymbols.rb3e_get_track_speed) = (int)GetTrackSpeed;
    *(int *)HmxFactoryFuncAt((int *)PORT_GDATAFUNCS, &globalSymbols.rb3e_set_venue) = (int)DTASetVenue;
    RB3E_MSG("Added DTA functions!", NULL);
    return r3_presv;
}