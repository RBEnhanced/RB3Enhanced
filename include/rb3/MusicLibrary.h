#ifndef _MUSICLIBRARY_H
#define _MUSICLIBRARY_H

#include "Symbol.h"

typedef struct _MusicLibrary
{
#ifdef RB3E_XBOX
    char unk[0xfc];
#else
    char unk[0xdc];
#endif
    int mSortType;
} MusicLibrary;

// Jumps to a given entry in the music library
void MusicLibrarySelect(int theMusicLibrary, Symbol entryName, int sortType, int unk_r6);
int *MusicLibraryConstructor(int *thisMusicLibrary, int *songPreview);
int *MusicLibraryConstructorHook(int *thisMusicLibrary, int *songPreview);
void MusicLibraryOnEnter(void* thisMusicLibrary);
void MusicLibraryOnUnload(void* thisMusicLibrary);
void MusicLibraryOnEnterHook(void* thisMusicLibrary);
void MusicLibraryOnUnloadHook(void* thisMusicLibrary);

#endif // _MUSICLIBRARY_H_
