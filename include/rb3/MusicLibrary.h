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
int *MusicLibraryConstructor(int *thisMusicLibrary, int unk);
int *MusicLibraryConstructorHook(MusicLibrary *thisMusicLibrary, int unk); //

//// i dont know what these are really called so i just put something lol
int MusicLibraryUnk1(int *unk, int unk2);

#endif // _MUSICLIBRARY_H_
