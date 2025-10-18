/*
    RB3Enhanced - MiloSceneHooks.h
    Hooks dealing with Milo scenes.
*/

#include "rb3/BinStream.h"
#include "rb3/DirLoader.h"
#include "rb3/Object.h"
#include "rb3/Vector.h"
#include "rb3/String.h"

// void DirLoaderOpenFileHook(DirLoader *thisDirLoader);
void LoadObj(Object *object, BinStream *stream);
void VertexReadHook(BinStream *thisBinStream, Vector3 *vec3);
void MakeOutfitPathHook(void *thisBandCharDesc, Symbol part, String *filePath);