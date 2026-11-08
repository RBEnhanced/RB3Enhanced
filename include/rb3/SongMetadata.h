#ifndef _SONGMETADATA_H
#define _SONGMETADATA_H

#include "rb3/BinStream.h"
#include "rb3/Data.h"
#include "String.h"
#include "Symbol.h"

typedef struct _DateTime
{
    unsigned char sec;
    unsigned char min;
    unsigned char hour;
    unsigned char day;
    unsigned char month;
    unsigned char year; // years since 1900
} DateTime;

// node of the STLport hashtable behind the Xbox build's ranks map (a singly linked list)
typedef struct _SongRankHashNode
{
    struct _SongRankHashNode *next;
    Symbol part;
    float rank;
} SongRankHashNode;

// STLport hash map<Symbol, float>, used for ranks on Xbox
typedef struct _SongRankHashMap
{
    int hashEquals;          // empty hash/equals functors + padding
    SongRankHashNode *first; // head of the element list, NULL-terminated
    void **bucketsBegin;
    void **bucketsEnd;
    void **bucketsCapacity;
    unsigned int count;
    float maxLoadFactor;
} SongRankHashMap;

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
    short albumTrackNum;
    DateTime dateRecorded;
    DateTime dateReleased;
    short unknown6;
    Symbol genre;
    int animTempo;
    Symbol vocalGender;
    int lengthMs;
    int basePoints;
#ifdef RB3E_XBOX
    int unknown7[2]; // unknown, only on Xbox
    SongRankHashMap ranks;
#endif
} SongMetadata;

extern SongMetadata *InitSongMetadata(SongMetadata *data);
extern SongMetadata *SongMetadataConstructor(SongMetadata *thisSongMetadata, DataArray *data, DataArray *backupData, char isOnDisc);
extern char SongMetadataLoad(SongMetadata *thisSongMetadata, BinStream *stream);

#endif // _SONGMETADATA_H
