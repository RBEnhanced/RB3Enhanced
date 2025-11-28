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
static char customPrefabNames[128][64];
static int customPrefabCount = 0;

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


#ifdef RB3E_XBOX

// the third arg is not really a String but a FilePath but shhh i wont tell if you dont
void MakeOutfitPathHook(BandCharDesc *thisBandCharDesc, Symbol part, String *filePath)
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

            // Check if file exists in ARK or raw filesystem
            if (FileExists((char *)path, 0) || FileExists((char *)path, 0x10002))
            {
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
        }
        else if (strcmp(genderSym->sym, "male") == 0)
        {
            path = "char/main/head/male/head_custom.milo";

            // Check if file exists in ARK or raw filesystem
            if (FileExists((char *)path, 0) || FileExists((char *)path, 0x10002))
            {
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
            }
        }
    }

    // check if we are loading eyebrows, if so this is perhaps a custom prefab part
    if (strcmp(part.sym, "eyebrows") == 0)
    {
        // check if the eyebrows path is actually a prefab name
        if (strstr(thisBandCharDesc->outfit.parts[0].mName.sym, "prefab_") != NULL)
        {
            char customPath[512] = {0};
            sprintf(customPath, "char/main/custom/characters/%s.milo", thisBandCharDesc->outfit.parts[0].mName.sym);
            len = strlen(customPath);
            RB3E_DEBUG("Loading custom prefab part from %s", customPath);
            copy = (char *)malloc(len + 1);
            if (copy != NULL)
            {
                strcpy(copy, customPath);
                filePath->buf = copy;
                filePath->length = (int)len;
            }
            return;
        }
    }

    MakeOutfitPath(thisBandCharDesc, part, filePath);
}

// ads a custom prefab name to the list
static void RegisterCustomPrefab(const char* name) {
    if (customPrefabCount >= 128) return;
    strncpy(customPrefabNames[customPrefabCount], name, 63);
    customPrefabNames[customPrefabCount][63] = '\0';
    customPrefabCount++;
}

// checks if the given string is a custom prefab name and returns its index, or -1 if not found
static int CustomPrefabIdx(const char* name) {
    int i;
    for (i = 0; i < customPrefabCount; i++) {
        if (strstr(name, customPrefabNames[i]) != NULL)
            return i;
    }
    return -1;
}

void* DirLoaderLoadObjsHook(String * filePath, int unk, int unk2)
{
    // gotta stay compliant with c89 so declare everything first
    void* prefabDir;
    int i;
    int j;
    char fullPrefabPath[256];
    int fileCount;
    char** customMiloFiles;
    char* fullPath;
    char* filename;
    char prefabNameStr[128];
    char* extensionStart;
    Symbol prefabNameSym;
    Object* rawObject;
    BandCharDesc* newPrefab;
    
    // need different sepearators / paths / extensions for 360 and wii
    #ifdef RB3E_XBOX
    const char* searchRoots[] = {"Game:", "RB3HDD:", "RB3USB0:", "RB3USB1:", "RB3USB2:"};
    int numSearchRoots = sizeof(searchRoots) / sizeof(searchRoots[0]);
    const char* miloExtension = "milo_xbox";
    const char path_separator = '\\';
    const char* customPrefabPath = "char\\main\\custom\\characters\\gen";
    #else
    const char* miloExtension = "milo_wii";
    const char path_separator = '/';
    const char* customPrefabPath = "char/main/custom/characters/gen";
    #endif

    // check if we are loading the main character prefabs milo
    if (strcmp(filePath->buf, "char/main/shared/prefabs.milo") == 0)
    {
        // load the original prefabs
        prefabDir = DirLoaderLoadObjs(filePath, unk, unk2);
        if (!prefabDir) {
            RB3E_MSG("Failed to load original prefabs.milo, your Rock Band 3 installation may be corrupt", 0);
            return NULL;
        }
    

        #ifdef RB3E_XBOX
        for (j = 0; j < numSearchRoots; ++j)
        #else
        for (j = 0; j < 1; ++j)
        #endif
        {
            #ifdef RB3E_XBOX
            sprintf(fullPrefabPath, "%s\\%s\\%s", searchRoots[j], config.RawfilesDir, customPrefabPath);
            #else
            sprintf(fullPrefabPath, "sd:/%s/%s", config.RawfilesDir, customPrefabPath);
            #endif
            
            fileCount = 0;
            customMiloFiles = RB3E_ListFiles(fullPrefabPath, miloExtension, &fileCount);

            // check if we found any custom milo files in this location
            if (customMiloFiles) {
                RB3E_DEBUG("Found %d custom character prefabs to inject from %s", fileCount, fullPrefabPath);

                for (i = 0; i < fileCount; i++) {
                    char strippedPath[256];
                    FileStream fs;
                    char endianBuf[4];
                    int endianResult;
                    char *extPos;
                    int partIdx;
                    char relativePath[256];

                    fullPath = customMiloFiles[i];
                    filename = strrchr(fullPath, path_separator);
                    
                    filename = filename ? filename + 1 : fullPath;
                    
                    strncpy(prefabNameStr, filename, sizeof(prefabNameStr) - 1);
                    prefabNameStr[sizeof(prefabNameStr) - 1] = '\0';

                    // cut it at the .milo_ part
                    extensionStart = strstr(prefabNameStr, ".milo_");
                    if (extensionStart) {
                        *extensionStart = '\0';
                    }

                    RB3E_DEBUG("Processing custom prefab: %s", prefabNameStr);

                    RegisterCustomPrefab(prefabNameStr);

                    SymbolConstruct(&prefabNameSym, prefabNameStr);

                    rawObject = BandCharDescNewObject();
                    if (!rawObject) {
                        RB3E_MSG("Failed to create new BandCharDesc object for %s", prefabNameStr);
                        continue;
                    }

                    // get the BandCharDesc pointer from the raw Hmx::Object pointer
                    // horrible hardcoded pointer math incoming
                    #ifdef RB3E_XBOX
                    newPrefab = (BandCharDesc*)((char*)rawObject - 0x238);
                    #else
                    newPrefab = (BandCharDesc*)((char*)rawObject - 0x21C);
                    #endif

                    strncpy(strippedPath, fullPath, sizeof(strippedPath) - 1);
                    strippedPath[sizeof(strippedPath) - 1] = '\0';
                    extPos = strstr(strippedPath, ".milo_");
                    if (extPos) {
                        *extPos = '\0';
                    }

                    // try to construct the filestream to read the prefab BandCharDesc right from disk
                    FileStreamConstructor(&fs, strippedPath, 2, 0);

                    if (!fs.vtable->fail(&fs)) {
                        RB3E_DEBUG("Loading raw BandCharDesc from disk: %s", prefabNameStr);
                        fs.vtable->readImpl(&fs, endianBuf, 4);
                        endianResult = (endianBuf[0] << 24) | (endianBuf[1] << 16) | (endianBuf[2] << 8) | endianBuf[3];
                        fs.littleEndian = (endianResult > 0xFFFF) ? 1 : 0;
                    
                        // rewind the filestream to the beginning
                        fs.vtable->seekImpl(&fs, 0, 0);
                    
                        // load the raw object data from the filestream
                        rawObject->table->load(rawObject, (BinStream*)&fs);
                    } else {

                        newPrefab->prefab = prefabNameSym;
                    }

                    // clean it up
                    fs.vtable->destructor(&fs, 0);
                

                    // nullify all 12 outfit parts
                    for (partIdx = 0; partIdx < 12; partIdx++) {
                        newPrefab->outfit.parts[partIdx].mName = *(Symbol*)PORT_NULLSYMBOL;
                    }

                    // set the prefab outfit path to the name of the prefab so we can later catch it in make outfit path
                    // shitty but meh, it works :tm:
                    SymbolConstruct(&newPrefab->outfit.parts[0].mName, prefabNameStr);

                    // set the name and add to the directory
                    rawObject->table->setName(rawObject, prefabNameSym, prefabDir);
                }

                // free the file list since we don't need it anymore
                RB3E_FreeFileList(customMiloFiles, fileCount);
            } else {
                RB3E_DEBUG("No custom prefabs found in %s.", fullPrefabPath);
            }
        }

        return prefabDir;
    }

    // if it's not the prefabs milo, load it normally
    return DirLoaderLoadObjs(filePath, unk, unk2);
}

const char* GetPrefabPortraitPathHook(void* thisPrefabChar)
{
    const char * ret = GetPrefabPortraitPath(thisPrefabChar);
    int idx = CustomPrefabIdx(ret);
    if(idx != -1) {
        static char customPortraitPath[256];
        sprintf(customPortraitPath, "char/main/custom/characters/%s.bmp", customPrefabNames[idx]);
        RB3E_DEBUG("Using custom portrait path for prefab %s: %s", customPrefabNames[idx], customPortraitPath);
        return customPortraitPath;
    }
    return ret;
}

#endif