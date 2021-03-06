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

char *originToIcon[][2] = {
    {"rb1", "<alt>Y</alt> "},
    {"rb2", "<alt>2</alt> "},
    {"rb3", "<alt>B</alt> "},
    {"rb4", "<alt>1</alt> "},
    {"rb_blitz", "<alt>X</alt> "},
    {"ugc", "<alt>U</alt> "},
    {"ugc_c3", "<alt>y</alt> "},
    {"ugc_plus", "<alt>U</alt> "},
    {"lego", "<alt>A</alt> "},
    {"greenday", "<alt>0</alt> "},
    {"beatles", "<alt>b</alt> "},
    {"gh1", "<alt>R</alt> "},
    {"gh2", "<alt>S</alt> "},
    {"gh3", "<alt>s</alt> "},
    {"onyxite", "<alt>G</alt> "},
};
int numOriginToIcon = sizeof(originToIcon) / sizeof(originToIcon[0]);

void SetSongAndArtistNameHook(BandLabel *label, SortNode *sortNode)
{
    // TODO: fix the buffer overflow that is present here
    char newLabel[1024] = {0};
    int i = 0;
    // TODO: make this func smaller by using a game origin --> icon character table or something
    if (config.GameOriginIcons == 1)
    {
        SetSongAndArtistName(label, sortNode);
        for (i = 0; i < numOriginToIcon; i++)
        {
            if (strcmp(sortNode->somethingElse->metaData->gameOrigin, originToIcon[i][0]) == 0)
            {
                strcat(newLabel, originToIcon[i][1]);
                strcat(newLabel, label->string);
                BandLabelSetDisplayText(label, newLabel, 1);
                return;
            }
        }
        strcat(newLabel, "<alt>0</alt> ");
        strcat(newLabel, label->string);
        BandLabelSetDisplayText(label, newLabel, 1);
        return;
    }
    SetSongAndArtistName(label, sortNode);
    return;
}