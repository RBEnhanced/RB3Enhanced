#ifndef _SONGMETADATA_H
#define _SONGMETADATA_H

#include "rb3/BinStream.h"
#include "rb3/Data.h"
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
    char *mShortName;
    int mSongID;
    char mIsOnDisc;
    char pad[3];
    char *mGameOrigin;
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
extern SongMetadata *SongMetadataConstructor(SongMetadata *thisSongMetadata, DataArray *data, DataArray *backupData, char isOnDisc);
extern char SongMetadataLoad(SongMetadata *thisSongMetadata, BinStream *stream);

#endif // _SONGMETADATA_H
