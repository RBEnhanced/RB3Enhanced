#ifndef _SONGMETADATA_H
#define _SONGMETADATA_H

// TODO: - Would like to get this 100% mapped out or close to it since other fields (such as artist) are likely also in here
typedef struct _SongMetadata
{
#ifdef RBE3_WII
    char unknown[0x2c];
#else
    char unknown[0x38];
#endif
    char *gameOrigin;
    char unknown2[0x18];
    char *title;
    char unknown3[0x28];
    char *genre;
    int animTempo;
    char *vocalGender;
    int lengthMs;
} SongMetadata;

#endif // _SONGMETADATA_H