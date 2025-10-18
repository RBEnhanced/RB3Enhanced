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

static char headPath[512] = "char/main/head/male/head_custom.milo";

void LoadObj(Object *object, BinStream *stream)
{
    char newPath[512] = {0};
    char *origPath;
    char *replacementPath;
    FileStream fileStream = {0};
    char endianBuf[4] = {0};
    int endianResult = 0;

    origPath = stream->vtable->name(stream);
    // length sanity check
    if (strlen(origPath) + strlen(object->name) - 20 > sizeof(newPath))
        goto object_pre_load;
    strncpy(newPath, origPath, strlen(origPath) - 10);
    // HACK(Emma): we don't have strncat on Wii so just strcat. THIS IS NOT SAFE!
    strcat(newPath, "/");          //, 1);
    strcat(newPath, object->name); //, strlen(object->name));

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

        object->table->preLoad(object, (BinStream *)&fileStream);

        // destroy the FileStream so we do not leak memory
        fileStream.vtable->destructor(&fileStream, 0);
        return;
    }

object_pre_load:
    object->table->preLoad(object, stream);
    return;
}

// This hook allows for GH2-360/RB1/RB2 meshes to load correctly in RB3
void VertexReadHook(BinStream *thisBinStream, Vector3 *vec3)
{
#ifdef RB3E_XBOX
    // the gRev of the current mesh
    int gRev = *(int *)PORT_MESH_GREV;
    char empty[4] = {0};

    BinstreamReadEndian(thisBinStream, (void *)&vec3->x, 4);
    BinstreamReadEndian(thisBinStream, (void *)&vec3->y, 4);
    BinstreamReadEndian(thisBinStream, (void *)&vec3->z, 4);

    // if the current RndMesh being read is the GH2-360/RB1/RB2 format, we need to skip over the W component
    // RB3 (for whatever reason) expects vertices in these versions of meshes to not include W
    // even though they *do* in actual GH2-360/RB1/RB2 meshes
    if (gRev == 34)
    {
        BinstreamReadEndian(thisBinStream, (void *)&empty, 4);
    }

    return;
#endif
}


// the third arg is not really a String but a FilePath but shhh i wont tell if you dont
void MakeOutfitPathHook(void *thisBandCharDesc, Symbol part, String *filePath)
{
    const char *path;
    char *copy;
    size_t len;

    // check if the head is hidden on the prefab and we are trying to load the head part
    if (*(char *)((char *)thisBandCharDesc + 0x20) == 1 && strcmp(part.sym, "head") == 0)
    {
        // determine gender, gender is a Symbol at 0x10
        Symbol *genderSym = (Symbol *)((char *)thisBandCharDesc + 0x10);
        if (strcmp(genderSym->sym, "female") == 0)
        {
            path = "char/main/head/female/head_custom.milo";
            len = strlen(path);

            RB3E_DEBUG("Head is hidden on female prefab, using custom head milo", 0);

            copy = (char *)malloc(len + 1);
            if (copy != NULL)
            {
                strcpy(copy, path);
                filePath->buf = copy;
                filePath->length = (int)len;
            }
            return;
        } 
        else if (strcmp(genderSym->sym, "male") == 0)
        {
            path = "char/main/head/male/head_custom.milo";
            len = strlen(path);

            RB3E_DEBUG("Head is hidden on male prefab, using custom head milo", 0);

            copy = (char *)malloc(len + 1);
            if (copy != NULL)
            {
                strcpy(copy, path);
                filePath->buf = copy;
                filePath->length = (int)len;
            }
            return;
        } else {
            RB3E_DEBUG("Unknown gender, cannot load custom head milo", 0);
            return;
        }
    }

    MakeOutfitPath(thisBandCharDesc, part, filePath);
}