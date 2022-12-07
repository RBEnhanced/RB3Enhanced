/*
    RB3Enhanced - MiloSceneHooks.c
    Hooks dealing with Milo scenes.
*/

#include "rb3e_include.h"
#include <string.h>

// TODO: Implement a hook for this for optimization purposes
// The problem with why it isn't currently here is that dirLoader->mFilePath.buf is the "uncorrected" path
// So ui/shared/gen/example.milo_xbox = ui/shared/example.milo which makes it harder to check if the folder exists
// Need to figure out if there is a good func for "correcting" the path that I can run here
// Once that is in place, this can be optimized significantly as we can check if the Milo's folder exists before doing any further checking
/*
void DirLoaderOpenFileHook(DirLoader *thisDirLoader)
{
    RB3E_MSG("Loading directory %s", thisDirLoader->mFilePath.buf);
    DirLoaderOpenFile(thisDirLoader);
    return;
}
*/

void LoadObj(Object *object, BinStream *stream)
{
    char newPath[512] = {0};
    char *origPath;
    char *replacementPath;
    FileStream fileStream = {0};
    char endianBuf[4] = {0};
    int endianResult = 0;

    origPath = stream->vtable->name(stream);
    strncpy(newPath, origPath, strlen(origPath) - 10);
    strncat(newPath, "/", 1);
    strncat(newPath, object->name, strlen(object->name));

    replacementPath = RB3E_GetRawfilePath(newPath, 0);
    if (replacementPath != NULL)
    {
        RB3E_DEBUG("Replacing Milo scene asset %s with file at %s", object->name, replacementPath);

        // create FileStream with filetype 2 so it looks outside of the ARK for the file
        fileStream = *FileStreamConstructor(&fileStream, replacementPath, 2, 0);

        // attempt to detect endianness of replacement asset
        // this allows assets from GH2 and earlier to work
        fileStream.vtable->readImpl(&fileStream, &endianBuf, 4);
        endianResult = (endianBuf[0] << 24) | (endianBuf[1] << 16) | (endianBuf[2] << 8) | endianBuf[3];
        RB3E_DEBUG("Endianness of replacement file is: %s", (endianResult > 100) ? "Little endian" : "Big endian");
        fileStream.littleEndian = (endianResult > 100) ? 1 : 0;

        // return the FileStream back to its starting position
        // not sure if this is 100% necessary, but cannot hurt
        fileStream.vtable->seekImpl(&fileStream, 0, 0);

        object->table->preLoad(object, &fileStream);

        // destroy the FileStream so we do not leak memory
        fileStream.vtable->destructor(&fileStream, 0);
        return;
    }

    object->table->preLoad(object, stream);
    return;
}