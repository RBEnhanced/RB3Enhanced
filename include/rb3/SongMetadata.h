#ifndef _SONGMETADATA_H
#define _SONGMETADATA_H

// TODO: - Would like to get this 100% mapped out or close to it since other fields (such as artist) are likely also in here
typedef struct _SongMetadata
{
#ifdef RBE3_WII
    char unknown[0x2c];
#else
    int *vtable;
    char unknown[0x28];
    char *shortname;
    int song_id;
    char unknown2[0x4];
#endif
    // TODO(Emma): is this all right on Wii too????
    char *gameOrigin;
    char unknown3[0x18];
    char *title;
    char unknown4[0x8]; // these may be String:: objects
    char *artist;
    char unknown5[0x8];
    char *album;
    char unknown6[0x18];
    char *genre;
    int animTempo;
    char *vocalGender;
    int lengthMs;
} SongMetadata;

#endif // _SONGMETADATA_H