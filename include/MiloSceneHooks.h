/*
    RB3Enhanced - MiloSceneHooks.h
    Hooks dealing with Milo scenes.
*/

#include "rb3/BinStream.h"
#include "rb3/DirLoader.h"
#include "rb3/Object.h"

// void DirLoaderOpenFileHook(DirLoader *thisDirLoader);
void LoadObj(Object *object, BinStream *stream);