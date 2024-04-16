#ifndef _FILE_H
#define _FILE_H

#include "String.h"

typedef struct _File File;

typedef void *(*FileDestructor_t)(File *thisFile);
typedef String *(*FileFilename_t)(String *str, File *thisFile);
typedef int (*FileRead_t)(File *thisFile, void *iData, int iBytes);
typedef char (*FileReadAsync_t)(File *thisFile, void *iData, int iBytes);
typedef int (*FileWrite_t)(File *thisFile, void *iData, int iBytes);
typedef char (*FileWriteAsync_t)(File *thisFile, void *iData, int iBytes);
typedef int (*FileSeek_t)(File *thisFile, int iOffset, int iSeekType);
typedef int (*FileTell_t)(File *thisFile);
typedef void (*FileFlush_t)(File *thisFile);
typedef char (*FileEof_t)(File *thisFile);
typedef char (*FileFail_t)(File *thisFile);
typedef int (*FileSize_t)(File *thisFile);
typedef int (*FileUncompressedSize_t)(File *thisFile);
typedef char (*FileReadDone_t)(File *thisFile, int *oBytes);
typedef char (*FileWriteDone_t)(File *thisFile, int *oBytes);

typedef struct _File_vtable
{
    FileDestructor_t destructor;
    FileFilename_t Filename;
    FileRead_t Read;
    FileReadAsync_t ReadAsync;
    FileWrite_t Write;
    FileWriteAsync_t WriteAsync;
    FileSeek_t Seek;
    FileTell_t Tell;
    FileFlush_t Flush;
    FileEof_t Eof;
    FileFail_t Fail;
    FileSize_t Size;
    FileUncompressedSize_t UncompressedSize;
    FileReadDone_t ReadDone;
    FileWriteDone_t WriteDone;
} File_vtable;

struct _File
{
    File_vtable *vtable;
};

// flags can be used to check the existence of files both inside and outside the ARK, similar to how the NewFile flags work
char FileExists(char *path, int flags);

#endif // _FILE_H
