/*
    RB3Enhanced - SetlistHooks.h
    Hooks for modifying the setlist/song selection screen.
*/

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "config.h"
#include "ports.h"
#include "GameOriginInfo.h"
#include "rb3/BandLabel.h"
#include "rb3/File.h"
#include "rb3/FilePath.h"
#include "rb3/Mem.h"
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

void CreateMaterial(GameOriginInfo *info)
{
    // alloc the memory for the dynamic tex
    DynamicTex *tex = NULL;
    char path[0x200];

    // TODO: we should identify and hook some function that runs when you leave the song select screen to call DynamicTex's destructor
    // it will free the dynamic tex itself, the material it created, and the texture too, so it nicely wraps it all up for you
    // this way there is not a chunk of memory permanently dedicated to game origin icons (even if it is not a large amount)
    tex = MemAlloc(0x20, 0);
    if (tex == NULL) {
        RB3E_DEBUG("MemAlloc failed for dynamic tex '%s'", info->gameOrigin);
        return;
    }

    // build the ark path (so dont include /gen/ or the _platform extension etc.)
    RB3E_DEBUG("Creating dynamic tex for game origin '%s'", info->gameOrigin);
    strcpy(path, "ui/resource/game_origins/");
    strcat(path, info->gameOrigin);
    strcat(path, ".png");

    // create and pray
    DynamicTexConstructor(tex, path, info->gameOrigin, 1, 0);
    if (tex->mMat == NULL || tex->mTex == NULL) {
        RB3E_DEBUG("DynamicTex::__ct failed for dynamic tex '%s'", path);

        // we should prob do cleanup here but that is a
        // TODO
        return;
    }

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

    RB3E_DEBUG("Dynamic tex created at %p with mat %p tex %p fileloader %p", tex, tex->mMat, tex->mTex, tex->mFileLoader);

    
}

int *MusicLibraryConstructorHook(int *thisMusicLibrary, int *songPreview)
{
    return MusicLibraryConstructor(thisMusicLibrary, songPreview);
}

RndMat *MusicLibraryMatHook(MusicLibrary *thisMusicLibrary, int data, int idx, UIListSlot *listSlot)
{
    if (listSlot != NULL && thisMusicLibrary != NULL)
    {
        if (strcmp(listSlot->mMatchName.buf, "game_origin_icon") == 0)
        {
            NodeSort *ret = 0;
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

    return (RndMat*)(intptr_t)MusicLibraryMat((void*)thisMusicLibrary, data, idx, (int*)listSlot);
}

// called when entering the music library
// allocate the memory and load the textures here
void MusicLibraryOnEnterHook(void *thisMusicLibrary)
{
    int i;

    // allocate and create the materials for any game origins we found while loading songs
    for (i = 0; i < numGameOrigins; i++) {
        int slot = originInfo[i].num;
        if (slot >= 0 && slot < 100) {
            if (textures[slot] == NULL) {
                CreateMaterial(&originInfo[i]);
            }
        }
    }

    // do the original logic
    MusicLibraryOnEnter(thisMusicLibrary);
}

// called when the music library panel is unloaded
void MusicLibraryOnUnloadHook(void *thisMusicLibrary)
{
    int i;

    MusicLibraryOnUnload(thisMusicLibrary);

    // call the dynamictex destructor for any textures we created
    // lets just sweep all 100 to be safe
    for (i = 0; i < 100; i++) {
        if (textures[i] != NULL) {
            // we can't call the DynamicTexDestructor because the FileLoader it used has already been freed by this point, so we must destruct everything but the FileLoader
            // extremely nasty but it seems to work
            if(textures[i]->mMat != NULL) {
                ((Object *)textures[i]->mMat)->table->destructor((Object *)textures[i]->mMat);
                textures[i]->mMat = NULL;
            }
            if(textures[i]->mTex != NULL) {
                ((Object *)textures[i]->mTex)->table->destructor((Object *)textures[i]->mTex);
                textures[i]->mTex = NULL;
            }
            // TODO: set up a proper vtable for String instead of doign this shit
            if(textures[i]->mMatName.length != 0) {
                if(textures[i]->mMatName.vtable != NULL) {
                    #ifdef RB3E_WII
                    ((void (*)(String *))textures[i]->mMatName.vtable[2])(&textures[i]->mMatName);
                    #else
                    ((void (*)(String *))textures[i]->mMatName.vtable[0])(&textures[i]->mMatName);
                    #endif
                }
            }
            textures[i] = NULL;
        }
    }

    RB3E_DEBUG("Freed game origin icon textures", NULL);
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