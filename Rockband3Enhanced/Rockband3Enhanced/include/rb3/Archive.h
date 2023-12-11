#ifndef _ARCHIVE_H
#define _ARCHIVE_H

typedef struct _Archive
{
    // replace with actual structure later
    char unk[0x78];
} Archive;

extern void ArchiveInit(void);
extern void ArchiveConstructor(Archive *thisArchive, char *path, int unknown);
extern void ArchiveDestructor(Archive *thisArchive);
extern void ArchiveMerge(Archive *thisArchive, Archive *otherArchive);
extern void ArchiveSetLocationHardDrive(Archive *thisArchive);

#endif // _ARCHIVE_H