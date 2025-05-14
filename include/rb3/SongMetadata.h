#ifndef _SONGMETADATA_H
#define _SONGMETADATA_H

#include "String.h"
#include "Symbol.h"

// technically BandSongMetadata?
typedef struct _SongMetadata
{
    int *vtable;
#ifdef RB3E_WII
    char unknown[0x1c];
#else
    char unknown[0x28];
#endif
    Symbol shortname;
    int song_id;
    char unknown2[0x4];
    Symbol gameOrigin;
    char unknown3[0x10];
    String title;
    String artist;
    String album;
#ifdef RB3E_WII
    char unknown6[0x10];
#else
    char unknown6[0x18];
#endif
    Symbol genre;
    int animTempo;
    Symbol vocalGender;
    int lengthMs;
} SongMetadata;

extern SongMetadata *InitSongMetadata(SongMetadata *data);

#endif // _SONGMETADATA_H
