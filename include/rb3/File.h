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
#ifdef RB3E_WII
    void *rtti;
    void *null;
#endif
    FileDestructor_t destructor;
#ifdef RB3E_PS3
    FileDestructor_t destructor2;
#endif
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

struct File
{
    File_vtable *vtable;
};

typedef struct _AsyncFile AsyncFile;

typedef void (*AsyncFile_OpenAsync_t)(AsyncFile *thisFile);
typedef char (*AsyncFile_OpenDone_t)(AsyncFile *thisFile);
typedef void (*AsyncFile_WriteAsync_t)(AsyncFile *thisFile, void *iData, int iBytes);
typedef char (*AsyncFile_WriteDone_t)(AsyncFile *thisFile);
typedef void (*AsyncFile_SeekToTell_t)(AsyncFile *thisFile);
typedef void (*AsyncFile_ReadAsync_t)(AsyncFile *thisFile, void *iData, int iBytes);
typedef char (*AsyncFile_ReadDone_t)(AsyncFile *thisFile);
typedef void (*AsyncFile_Close_t)(AsyncFile *thisFile);

typedef struct _AsyncFile_vtable
{
    File_vtable file;
    AsyncFile_OpenAsync_t _OpenAsync;
    AsyncFile_OpenDone_t _OpenDone;
    AsyncFile_WriteAsync_t _WriteAsync;
    AsyncFile_WriteDone_t _WriteDone;
    AsyncFile_SeekToTell_t _SeekToTell;
    AsyncFile_ReadAsync_t _ReadAsync;
    AsyncFile_ReadDone_t _ReadDone;
    AsyncFile_Close_t _Close;
} AsyncFile_vtable;

struct AsyncFile
{
    AsyncFile_vtable *vtable;
    int mMode;
    char mFail;
    char unk9;
    String mFilename;
    unsigned int mTell;
    int mOffset;
    unsigned int mSize;
    unsigned int mUCSize;
    char *mBuffer;
    char *mData;
    int mBytesLeft;
    int mBytesRead;
};

// flags can be used to check the existence of files both inside and outside the ARK, similar to how the NewFile flags work
char FileExists(char *path, int flags);

char FileIsDLC(char *path);
char FileIsLocal(char *path);

#endif // _FILE_H
