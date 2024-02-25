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
#include "rb3/Rnd/RndMat.h"
#include "rb3/Rnd/RndTex.h"

RndMat *materials[100] = {0};
RndTex *textures[100] = {0};

void CreateMaterials(GameOriginInfo *info)
{
    RB3E_DEBUG("Creating material for game origin '%s'", info->gameOrigin);

    // create our new RndTex and RndMat
    textures[info->num] = RndTexNewObject();
    materials[info->num] = RndMatNewObject();

    if (textures[info->num] != NULL && materials[info->num] != NULL)
    {
        int i = 0;
        char texPath[1024] = {0};
        FilePath textureFilePath = {0};

        strcat(texPath, "ui/resource/game_origins/");
        strcat(texPath, info->gameOrigin);
        strcat(texPath, ".png");
        FilePathConstructor(&textureFilePath, texPath);

        RndTexSetBitmap(textures[info->num], &textureFilePath);

        materials[info->num]->diffuseTex.tex = textures[info->num];

        // allow the material to become transparent so alpha channel does not appear black
        // not sure if all of this is really necessary
        materials[info->num]->blend = kSrcAlpha;
        materials[info->num]->preLit = 1;
        materials[info->num]->useEnviron = 0;
        materials[info->num]->alphaCut = 0;
        materials[info->num]->alphaThreshold = 0;
        materials[info->num]->intensify = 0;
    }
}

void SetSongAndArtistNameHook(BandLabel *label, SortNode *sortNode)
{
    char newLabel[1024] = {0};
    char *originLabel = "  "; // default

    if (config.GameOriginIcons == 1 && strlen(label->string) < 1000)
    {
        SetSongAndArtistName(label, sortNode);
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
    char *originLabel = "  "; // default

    if (config.GameOriginIcons == 1 && strlen(label->string) < 1000)
    {
        SetSongNameFromNode(label, sortNode);
        strcat(newLabel, originLabel);
        strcat(newLabel, label->string);
        BandLabelSetDisplayText(label, newLabel, 1);
        return;
    }
    SetSongNameFromNode(label, sortNode);
    return;
}

int *MusicLibraryConstructorHook(MusicLibrary *thisMusicLibrary, int *songPreview)
{
    // CreateMaterials();
    return MusicLibraryConstructor(thisMusicLibrary, songPreview);
}

RndMat *MusicLibraryMatHook(MusicLibrary *thisMusicLibrary, int data, int idx, UIListSlot *listSlot)
{
    if (listSlot != NULL)
    {
        if (strcmp(listSlot->mMatchName.buf, "game_origin_picture_slot") == 0)
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
                    // do a basic null check here, sometimes it can be null
                    if (node->record->metaData != NULL &&
                        node->record->metaData->mGameOrigin != NULL)
                    {
                        nodeType = node->vtable->getNodeType();
                        if (nodeType == kNodeSong)
                        {
                            // this shit fucking sucks lol
                            for (curInfo = 0; curInfo < numGameOrigins; curInfo++)
                            {
                                if (strcmp(node->record->metaData->mGameOrigin, originInfo[curInfo].gameOrigin) == 0)
                                {
                                    if (materials[originInfo[curInfo].num] != NULL)
                                    {
                                        RB3E_DEBUG("Returning material for game origin '%s'", originInfo[curInfo].gameOrigin);
                                        return materials[originInfo[curInfo].num];
                                    }
                                    else
                                    {
                                        RB3E_DEBUG("Material for game origin '%s' is NULL, creation seemingly failed", originInfo[curInfo].gameOrigin);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return MusicLibraryMat(thisMusicLibrary, data, idx, listSlot);
}

int numGameOrigins;
GameOriginInfo originInfo[100] = {0};

void AddGameOriginToIconList(char *gameOrigin)
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
    CreateMaterials(&originInfo[numGameOrigins]);
    numGameOrigins++;
    RB3E_DEBUG("Adding game origin '%s' to icon list, total is now %i", gameOrigin, numGameOrigins);
}

// this will be called any time a song is loaded from DTA (on disc or when loading into the cache)
SongMetadata *SongMetadataConstructorHook(SongMetadata *thisSongMetadata, DataArray *data, DataArray *backupData, char isOnDisc)
{
    thisSongMetadata = SongMetadataConstructor(thisSongMetadata, data, backupData, isOnDisc);

    // make sure the game origin isn't null
    if (thisSongMetadata->mGameOrigin != 0)
    {
        char gameOrigin[0x200];
        strcpy(gameOrigin, "ui/resource/game_origins/gen/");
        strcat(gameOrigin, thisSongMetadata->mGameOrigin);
#ifdef RB3E_XBOX
        strcat(gameOrigin, ".png_xbox");
#else
        strcat(gameOrigin, ".png_wii");
#endif

        // check the game origin image exists in the ARK
        if (FileExists(gameOrigin, kRead) == 0)
        {
            if (FileExists(gameOrigin, kReadNoArk) == 0)
            {
                RB3E_DEBUG("Game origin icon at path '%s' for origin '%s' does not exist!", gameOrigin, thisSongMetadata->mGameOrigin);
                return thisSongMetadata;
            }
        }
        AddGameOriginToIconList(thisSongMetadata->mGameOrigin);
        return thisSongMetadata;
    }

    return thisSongMetadata;
}

// this will be called when a song is loaded from cache
char SongMetadataLoadHook(SongMetadata *thisSongMetadata, BinStream *stream)
{
    char ret = SongMetadataLoad(thisSongMetadata, stream);

    // make sure the game origin isn't null
    if (thisSongMetadata->mGameOrigin != 0)
    {
        char gameOrigin[0x200];
        strcpy(gameOrigin, "ui/resource/game_origins/gen/");
        strcat(gameOrigin, thisSongMetadata->mGameOrigin);
#ifdef RB3E_XBOX
        strcat(gameOrigin, ".png_xbox");
#else
        strcat(gameOrigin, ".png_wii");
#endif

        // check the game origin image exists in the ARK
        if (FileExists(gameOrigin, kRead) == 0)
        {
            if (FileExists(gameOrigin, kReadNoArk) == 0)
            {
                RB3E_DEBUG("Game origin icon at path '%s' for origin '%s' does not exist!", gameOrigin, thisSongMetadata->mGameOrigin);
                return ret;
            }
        }
        AddGameOriginToIconList(thisSongMetadata->mGameOrigin);
        return ret;
    }

    return ret;
}