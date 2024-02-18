#ifndef _MUSICLIBRARY_H
#define _MUSICLIBRARY_H

#include "Symbol.h"

typedef struct _MusicLibrary
{
    char unk[0xfc];
    int unk2;
} MusicLibrary;

// Jumps to a given entry in the music library
void MusicLibrarySelect(int theMusicLibrary, Symbol entryName, int sortType, int unk_r6);
int *MusicLibraryConstructor(int *thisMusicLibrary, int *songPreview);
int *MusicLibraryConstructorHook(MusicLibrary *thisMusicLibrary, int *songPreview);

// in the RB3 debug this function is inlined
SortNode *MusicLibraryGetNodeByIndex(int *ret, int idx);

#endif // _MUSICLIBRARY_H_
