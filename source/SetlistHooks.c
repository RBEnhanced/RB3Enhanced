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

DynamicTex *textures[100] = {0};
static int numOriginToIcon = sizeof(numOriginToIcon) / sizeof(originToIcon[0]);

void SetSongAndArtistNameHook(BandLabel *label, SortNode *sortNode)
{
    char newLabel[1024] = {0};
    char *originLabel = "<alt>0</alt> "; // default
    int i = 0;

    if (config.GameOriginIcons == 1 && strlen(label->string) < 1000)
    {
        SetSongAndArtistName(label, sortNode);
        for (i = 0; i < numOriginToIcon; i++)
        {
            if (strcmp(sortNode->something->metaData->gameOrigin.sym, originToIcon[i][0]) == 0)
            {
                originLabel = originToIcon[i][1];
                break;
            }
        }
        strcat(newLabel, originLabel);
        strcat(newLabel, label->string);
        BandLabelSetDisplayText(label, newLabel, 1);
        return;
    }
    SetSongAndArtistName(label, sortNode);
    return;
}

void SetSongNameFromNodeHook(BandLabel *label, SortNode *sortNode)
{
    char newLabel[1024] = {0};
    char *originLabel = "<alt>0</alt> "; // default
    int i = 0;

    RB3E_DEBUG("SetSongNameFromNode: %s", label->string);

    if (config.GameOriginIcons == 1 && strlen(label->string) < 1000)
    {
        SetSongNameFromNode(label, sortNode);
        for (i = 0; i < numOriginToIcon; i++)
        {
            if (strcmp(sortNode->something->metaData->gameOrigin.sym, originToIcon[i][0]) == 0)
            {
                originLabel = originToIcon[i][1];
                break;
            }
        }
        strcat(newLabel, originLabel);
        strcat(newLabel, label->string);
        BandLabelSetDisplayText(label, newLabel, 1);
        return;
    }
    SetSongNameFromNode(label, sortNode);
    return;
}

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
    if (listSlot != NULL && thisMusicLibrary != NULL)
    {
        if (strcmp(listSlot->mMatchName.buf, "game_origin_icon") == 0)
        {
            int *ret = 0;
            SortNode *node = 0;
            SongNodeType nodeType = kNodeNone;
            int curInfo = 0;

            ret = SongSortMgrGetSort(*(SongSortMgr **)PORT_THESONGSORTMGR, thisMusicLibrary->mSortType);
            if (ret != NULL)
            {
                node = NodeSortGetNode(int, idx);
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

    return MusicLibraryMat(thisMusicLibrary, data, idx, UIListSlot);
}

int numGameOrigins;
GameOriginInfo originInfo[100] = {0};

void AddGameOriginToIconList(char *gameOrigin)
{
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
}

// this will be called any time a song is loaded from DTA (on disc or when loading into the cache)
SongMetadata *SongMetadataConstructorHook(SongMetadata *thisSongMetadata, DataArray *data, DataArray *backupData, char isOnDisc)
{
    thisSongMetadata = SongMetadataConstructor(thisSongMetadata, data, backupData, isOnDisc);

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
    char ret = SongMetadataLoad(thisSongMetadata, stream);

    // make sure the game origin isn't null
    if (thisSongMetadata->gameOrigin.sym != 0)
    {
        AddGameOriginToIconList(thisSongMetadata->gameOrigin.sym);
        return ret;
    }

    return ret;
}