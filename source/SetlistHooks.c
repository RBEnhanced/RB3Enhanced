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
#include "rb3/SortNode.h"

static char *originToIcon[][2] = {
    {"rb1", "<alt>Y</alt> "},
    {"rb2", "<alt>2</alt> "},
    {"rb3", "<alt>B</alt> "},
    {"rb4", "<alt>1</alt> "},
    {"rb_blitz", "<alt>X</alt> "},
    {"ugc", "<alt>U</alt> "},
    {"ugc_c3", "<alt>y</alt> "},
    {"ugc_plus", "<alt>U</alt> "},
    {"ugc1", "<alt>U</alt> "},
    {"ugc2", "<alt>U</alt> "},
    {"lego", "<alt>A</alt> "},
    {"greenday", "<alt>0</alt> "},
    {"beatles", "<alt>b</alt> "},
    {"gh1", "<alt>R</alt> "},
    {"gh2", "<alt>S</alt> "},
    {"gh3", "<alt>s</alt> "},
    {"onyxite", "<alt>G</alt> "},
};
static int numOriginToIcon = sizeof(originToIcon) / sizeof(originToIcon[0]);

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
            if (strcmp(sortNode->somethingElse->metaData->gameOrigin, originToIcon[i][0]) == 0)
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
            if (strcmp(sortNode->somethingElse->metaData->gameOrigin, originToIcon[i][0]) == 0)
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