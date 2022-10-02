#ifndef _BANDLABEL_H
#define _BANDLABEL_H

#include "SortNode.h"

typedef struct _BandLabel
{
#ifdef RB3E_WII
    char unk[0x1ec];
#else
    char unk[0x24c];
#endif
    int strLength;
    char *string;
} BandLabel;

extern void SetSongAndArtistName(BandLabel *label, SortNode *unk);
extern void SetSongNameFromNode(BandLabel *label, SortNode *unk);
extern void BandLabelSetDisplayText(BandLabel *label, char *text, int unk);

#endif // _BANDLABEL_H