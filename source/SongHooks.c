/*
    RB3Enhanced - SongHooks.c
    Hooks related to loading songs into the game.
*/

#include <string.h>
#include <stdio.h>
#include "ports.h"
#include "rb3/PassiveMessagesPanel.h"
#include "rb3/Data.h"
#include "rb3/Random.h"

static int HasShownCorrectionError = 0;
static int CorrectSongID(DataNode *id_node)
{
    unsigned int i, sum = 0;
    // do a very basic shitty checksum.. maybe move into xxhash?
    // WARNING: this may collide. BAD CUSTOMS ARE STILL BAD!!!
    if (id_node->value.string == NULL) // never meant to get here
        return 0x41414141;
    for (i = 0; i < strlen(id_node->value.string); i++)
        sum += id_node->value.string[i];
    // move it around a bit just to make things more consistent
    sum &= 0x8FFFFF;
    sum += 2130000000;
    RB3E_MSG("Corrected song_id '%s' into ID: %i", id_node->value.string, sum);
    if (HasShownCorrectionError == 0)
    {
        HasShownCorrectionError = 1;
        DisplayMessage("One (or more) of your songs has been corrected.");
    }
    return sum;
}

int MetadataSongIDHook(DataNode *song_id)
{
    DataNode *eval = DataNodeEvaluate(song_id);
    if (eval->type != INT_VALUE)
        eval->value.intVal = CorrectSongID(eval);
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
        array = DataFindArray(missing_data_maybe, song_id);
    if (array == NULL)
        return 0;
    found = DataNodeEvaluate(&array->mNodes->n[1]);
    if (found == NULL)
        return 0;
    if (found->type != INT_VALUE)
        return CorrectSongID(found);
    return found->value.intVal;
}
