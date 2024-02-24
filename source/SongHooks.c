/*
    RB3Enhanced - SongHooks.c
    Hooks related to loading songs into the game.
*/

#include <string.h>
#include <stdio.h>
#include "ports.h"
#include "crc32.h"
#include "GameOriginInfo.h"
#include "rb3/File.h"
#include "rb3/PassiveMessagesPanel.h"
#include "rb3/SongMetadata.h"
#include "rb3/Data.h"
#include "rb3/Random.h"

int RB3E_LoadedSongCount = 0;

static int HasShownCorrectionError = 0;
static int CorrectSongID(DataNode *id_node, int ct_metadata)
{
    unsigned int checksum = 0;
    // shouldn't be possible to get here
    if (id_node->type == INT_VALUE || id_node->value.string == NULL)
        return id_node->value.intVal;
    // run a CRC32 sum over the whole length of the string
    crc32(id_node->value.string, strlen(id_node->value.string), &checksum);
    // move it around a bit just to make things more consistent
    // risks introducing more collisions, BAD CUSTOMS ARE STILL BAD!!
    checksum %= 9999999;
    checksum += 2130000000;
    // output to the log whenever things are corrected when crafting SongMetadata
    if (ct_metadata)
        RB3E_MSG("Corrected song_id '%s' into ID: %i", id_node->value.string, checksum);
    // display a warning message on screen for the first bad song found
    if (HasShownCorrectionError == 0)
    {
        HasShownCorrectionError = 1;
        DisplayMessage("One (or more) of your songs has been corrected.");
    }
    return checksum;
}

int MetadataSongIDHook(DataNode *song_id)
{
    DataNode *eval = DataNodeEvaluate(song_id);
    // if our song id isn't an int, correct it with crc32 over the string
    if (eval->type != INT_VALUE)
        eval->value.intVal = CorrectSongID(eval, 1);
    return eval->value.intVal;
}

int GetSongIDHook(DataArray *song, DataArray *missing_data_maybe)
{
    Symbol song_id;
    DataNode *found;
    DataArray *array;
    SymbolConstruct(&song_id, "song_id");
    if (song == NULL)
        return 0;
    array = DataFindArray(song, song_id);
    if (array == NULL)
    {
        if (missing_data_maybe == NULL)
            return 0;
        else
            array = DataFindArray(missing_data_maybe, song_id);
    }
    if (array == NULL)
        return 0;
    found = DataNodeEvaluate(&array->mNodes->n[1]);
    if (found == NULL)
        return 0;
    if (found->type != INT_VALUE)
        return CorrectSongID(found, 0);
    return found->value.intVal;
}

SongMetadata *InitSongMetadataHook(SongMetadata *songMetadata)
{
    // increment the loaded song count based on how many songmetadata objects there are
    RB3E_LoadedSongCount++;
    return InitSongMetadata(songMetadata);
}
