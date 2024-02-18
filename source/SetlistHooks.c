/*
    RB3Enhanced - SetlistHooks.h
    Hooks for modifying the setlist/song selection screen.
*/

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "ports.h"
#include "rb3/BandLabel.h"
#include "rb3/FilePath.h"
#include "rb3/MusicLibrary.h"
#include "rb3/UI/UIListSlot.h"
#include "rb3/UI/UIPanel.h"
#include "SongSort.h"
#include "rb3/SortNode.h"
#include "rb3/Rnd/RndMat.h"
#include "rb3/Rnd/RndTex.h"

static RndTex *tex = {0};
static RndMat *mat = {0};

void CreateMaterials()
{
    // create our new RndTex and RndMat
    tex = RndTexNewObject();
    mat = RndMatNewObject();

    if (tex != NULL && mat != NULL)
    {
        int i = 0;
        FilePath texPath = {0};
        FilePathConstructor(&texPath, "ui/resource/game_origins/rb3_keep.bmp");

        RndTexSetBitmap(tex, &texPath);

        mat->diffuseTex.tex = tex;

        // allow the material to become transparent so alpha channel does not appear black
        // not sure if all of this is really necessary
        mat->blend = kSrcAlpha;
        mat->preLit = 1;
        mat->useEnviron = 0;
        mat->alphaCut = 0;
        mat->alphaThreshold = 0;
        mat->intensify = 0;
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
    CreateMaterials();
    return MusicLibraryConstructor(thisMusicLibrary, songPreview);
}

RndMat *MusicLibraryMatHook(MusicLibrary *thisMusicLibrary, int data, int idx, UIListSlot *listSlot)
{
    if (listSlot != NULL)
    {
        if (mat != NULL && tex != NULL)
        {
            if (strcmp(listSlot->mMatchName.buf, "game_origin_picture_slot") == 0)
            {
                int *ret = 0;
                SortNode *node = 0;
                SongNodeType nodeType = kNodeNone;

                ret = ((int *)(*(void **)PORT_THESONGSORTMGR))[thisMusicLibrary->unk2 + 0x13];
                if (ret != NULL)
                {
                    node = MusicLibraryGetNodeByIndex(ret, idx);
                    if (node != NULL)
                    {
                        nodeType = node->vtable->getNodeType();
                        if (nodeType == kNodeSong)
                        {
                            return mat;
                        }
                    }
                }
            }
        }
    }

    return MusicLibraryMat(thisMusicLibrary, data, idx, listSlot);
}