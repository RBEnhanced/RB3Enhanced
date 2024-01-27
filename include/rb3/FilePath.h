#ifndef _FILEPATH_H
#define _FILEPATH_H

#include "String.h"

typedef struct _FilePath
{
    String path;
} FilePath;

// even though the structure of FilePath is very simplistic we need to use the constructor as it calls
// extra functions to properly construct paths and etc.
FilePath *FilePathConstructor(FilePath *thisFilePath, char *path);

#endif // _FILEPATH_H