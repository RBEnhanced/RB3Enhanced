#ifndef _SONGMETADATA_H
#define _SONGMETADATA_H

#include "String.h"

// technically BandSongMetadata?
typedef struct _SongMetadata
{
    int *vtable;
#ifdef RB3E_WII
    char unknown[0x1c];
#else
    char unknown[0x28];
#endif
    char *shortname;
    int song_id;
    char unknown2[0x4];
    char *gameOrigin;
    char unknown3[0x10];
    String title;
    String artist;
    String album;
#ifdef RB3E_WII
    char unknown6[0x10];
#else
    char unknown6[0x18];
#endif
    char *genre;
    int animTempo;
    char *vocalGender;
    int lengthMs;
} SongMetadata;

extern SongMetadata *InitSongMetadata(SongMetadata *data);

#endif // _SONGMETADATA_H
