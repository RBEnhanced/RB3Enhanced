/*
    RB3Enhanced - SetlistHooks.h
    Hooks for modifying the setlist/song selection screen.
*/

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "ports.h"
#include "GameOriginInfo.h"
#include "rb3/BandLabel.h"
#include "rb3/File.h"
#include "rb3/PassiveMessagesPanel.h"
#include "rb3/SongMetadata.h"
#include "rb3/Data.h"
#include "rb3/Random.h"
#include "rb3/FilePath.h"
#include "rb3/MusicLibrary.h"
#include "rb3/NodeSort.h"
#include "rb3/UI/UIListSlot.h"
#include "rb3/UI/UIPanel.h"
#include "SongSort.h"
#include "rb3/SortNode.h"
#include "rb3/SongSortMgr.h"
#include "rb3/Rnd/DynamicTex.h"
#include "rb3/Rnd/RndMat.h"

int RB3E_LoadedSongCount = 0;

DynamicTex *textures[100] = {0};
static int HasShownCorrectionError = 0;
static int CorrectSongID(DataNode *id_node, int ct_metadata)

void CreateMaterial(GameOriginInfo *info)
{
    // alloc the memory for the dynamic tex
    DynamicTex *tex = NULL;
    char path[0x200];

    // TODO: we should identify and hook some function that runs when you leave the song select screen to call DynamicTex's destructor
    // it will free the dynamic tex itself, the material it created, and the texture too, so it nicely wraps it all up for you
    // this way there is not a chunk of memory permanently dedicated to game origin icons (even if it is not a large amount)
    tex = MemAlloc(0x20, 0);

    // build the ark path (so dont include /gen/ or the _platform extension etc.)
    RB3E_DEBUG("Creating dynamic tex for game origin '%s'", info->gameOrigin);
    strcpy(path, "ui/resource/game_origins/");
    strcat(path, info->gameOrigin);
    strcat(path, ".png");

    // create and pray
    DynamicTexConstructor(tex, path, info->gameOrigin, 1, 0);

    textures[info->num] = tex;

    RB3E_DEBUG("Setting diffuse texture for dynamic tex '%s'", path);
    RndTexSetBitmap3(tex->mTex, tex->mFileLoader);

    // diffuse tex setter function doesn't exist on 360, so we manually set it, but manually setting it doesn't work on wii and we must use the setter
    // TODO: figure out why we can't just set it manually on both, probably a structure inaccuracy or something
    #ifdef RB3E_XBOX
    tex->mMat->diffuseTex.tex = tex->mTex;
    #else
    RndMatSetDiffuseTex(tex->mMat, tex->mTex);
    #endif

    // print the material name
    RB3E_DEBUG("Dynamic tex created at %p with material '%s'", textures[info->num], textures[info->num]->mMatName.buf);

    
}

int *MusicLibraryConstructorHook(MusicLibrary *thisMusicLibrary, int *songPreview)
{
    return MusicLibraryConstructor(thisMusicLibrary, songPreview);
}

RndMat *MusicLibraryMatHook(MusicLibrary *thisMusicLibrary, int data, int idx, UIListSlot *listSlot)
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
    if (listSlot != NULL && thisMusicLibrary != NULL)
    {
        HasShownCorrectionError = 1;
        DisplayMessage("One (or more) of your songs has been corrected.");
        if (strcmp(listSlot->mMatchName.buf, "game_origin_icon") == 0)
        {
            int *ret = 0;
            SortNode *node = 0;
            SongNodeType nodeType = kNodeNone;
            int curInfo = 0;

            ret = SongSortMgrGetSort(*(SongSortMgr **)PORT_THESONGSORTMGR, thisMusicLibrary->mSortType);
            if (ret != NULL)
            {
                node = NodeSortGetNode(ret, idx);
                if (node != NULL)
                {
                    nodeType = node->vtable->getNodeType();

                    // ensure this is actually a song node and not like a function node or etc.
                    if (nodeType == kNodeSong)
                    {
                        // do a basic null check here, sometimes it can be null
                        if (node != NULL && node->record != NULL &&
                            node->record->metaData != NULL &&
                            node->record->metaData->gameOrigin.sym != NULL)
                        {
                            // this shit fucking sucks lol
                            for (curInfo = 0; curInfo < numGameOrigins; curInfo++)
                            {
                                if (strcmp(node->record->metaData->gameOrigin.sym, originInfo[curInfo].gameOrigin) == 0)
                                {
                                    if (textures[originInfo[curInfo].num] != NULL && textures[originInfo[curInfo].num]->mMat != NULL)
                                    {
                                        RB3E_DEBUG("Returning material for game origin '%s'", originInfo[curInfo].gameOrigin);
                                        return textures[originInfo[curInfo].num]->mMat;
                                    }
                                    else
                                    {
                                        RB3E_DEBUG("Material for game origin '%s' is NULL, creation seemingly failed", originInfo[curInfo].gameOrigin);
                                    }
                                }
                            }
                        } else {
                            RB3E_DEBUG("Node record or its metadata is NULL, skipping", NULL);
                        }
                    }
                }
            }
        }
    }

    return MusicLibraryMat(thisMusicLibrary, data, idx, listSlot);
}

int MetadataSongIDHook(DataNode *song_id)
int numGameOrigins;
GameOriginInfo originInfo[100] = {0};

void AddGameOriginToIconList(char *gameOrigin)
{
    DataNode *eval = DataNodeEvaluate(song_id);
    if(gameOrigin != NULL && strcmp(gameOrigin, "") != 0)
    {
    int i = 0;

    // check that we havent already added 100 game origins
    if (numGameOrigins >= 100)
    {
        RB3E_DEBUG("Too many game origins with icons, not adding '%s'", gameOrigin);
        return;
    }

    // make sure the game origin isn't already in the array
    for (i = 0; i < numGameOrigins; i++)
    {
        if (strcmp(originInfo[i].gameOrigin, gameOrigin) == 0)
        {
            return;
        }
    }

    originInfo[numGameOrigins].gameOrigin = gameOrigin;
    originInfo[numGameOrigins].num = numGameOrigins;
    CreateMaterial(&originInfo[numGameOrigins]);
    numGameOrigins++;
    RB3E_DEBUG("Adding game origin '%s' to icon list, total is now %i", gameOrigin, numGameOrigins);
    } 
    else
     {
        RB3E_DEBUG("Game origin is NULL or empty, not adding to icon list", NULL);
    }
    // if our song id isn't an int, correct it with crc32 over the string
    if (eval->type != INT_VALUE)
        eval->value.intVal = CorrectSongID(eval, 1);
    return eval->value.intVal;
}

// this will be called any time a song is loaded from DTA (on disc or when loading into the cache)
SongMetadata *SongMetadataConstructorHook(SongMetadata *thisSongMetadata, DataArray *data, DataArray *backupData, char isOnDisc)
{
    Symbol song_id;
    DataNode *found = NULL;
    DataArray *array = NULL;
    SymbolConstruct(&song_id, "song_id");
    thisSongMetadata = SongMetadataConstructor(thisSongMetadata, data, backupData, isOnDisc);
    if (song == NULL)
        return 0;
    // check missing song data first if we have it
    if (missing_data_maybe != NULL)
        array = DataFindArray(missing_data_maybe, song_id);
    // if there's nothing in missing song data, check song array
    if (array == NULL)
        array = DataFindArray(song, song_id);
    // no song_id? idk man
    if (array == NULL)
        return 0;
    // get the value from song_id
    found = DataNodeEvaluate(&array->mNodes->n[1]);
    if (found == NULL)
        return 0;
    if (found->type != INT_VALUE)
        return CorrectSongID(found, 0);

    // make sure the game origin isn't null
    if (thisSongMetadata->gameOrigin.sym != 0)
    {
        AddGameOriginToIconList(thisSongMetadata->gameOrigin.sym);
        return thisSongMetadata;
    }

    return thisSongMetadata;
}

// this will be called when a song is loaded from cache
char SongMetadataLoadHook(SongMetadata *thisSongMetadata, BinStream *stream)
{
    // increment the loaded song count based on how many songmetadata objects there are
    char ret = SongMetadataLoad(thisSongMetadata, stream);

    // make sure the game origin isn't null
    if (thisSongMetadata->gameOrigin.sym != 0)
    {
        RB3E_LoadedSongCount++;
        AddGameOriginToIconList(thisSongMetadata->gameOrigin.sym);
        return ret;
    }

    return ret;
}