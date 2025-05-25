#ifdef RB3E_WII

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <io/fat-sd.h>
#include <fcntl.h>
#include "FileSD.h"
#include "rb3/File.h"
#include "rb3/Mem.h"
#include "rb3enhanced.h"
#include "ports.h"

#define LE16(i) ((((i) & 0xFF) << 8 | ((i) >> 8) & 0xFF) & 0xFFFF)
#define LE(i) (((i) & 0xff) << 24 | ((i) & 0xff00) << 8 | ((i) & 0xff0000) >> 8 | ((i) >> 24) & 0xff)

File_vtable FileSD_vtable;

FileSD *FileSD_Destroy(FileSD *file)
{
    RB3E_DEBUG("FileSD_Destroy(file=%p)", file);
    if (file->mFd != -1)
        SD_close(file->mFd);
    if (file->mFilePath != NULL)
        MemFree(file->mFilePath);
    MemFree(file);
    return NULL;
}

FileSD *FileSD_New(const char *filepath, int flags)
{
    RB3E_DEBUG("FileSD_New(filepath='%s', flags=0x%x)", filepath, flags);
    // create the new object
    FileSD *sd = MemAlloc(sizeof(FileSD), 0);
    if (sd == NULL) {
        RB3E_DEBUG("Not enough memory to read file '%s' from SD!", filepath);
        return NULL;
    }
    memset(sd, 0, sizeof(FileSD));
    FileSD_InitVtable(); // TODO(Emma): find a better place to put this
    sd->vtable = &FileSD_vtable;
    sd->mFd = -1;

    // set the file path
    sd->mFilePath = MemAlloc(strlen(filepath) + 1, 0);
    if (sd->mFilePath  == NULL) {
        RB3E_DEBUG("Not enough memory to read file '%s' from SD!", filepath);
        FileSD_Destroy(sd);
        return NULL;
    }
    strcpy(sd->mFilePath, filepath);

    // determine the flags to use
    int fileMode = O_RDONLY;
    if ((flags & 0x4)) {
        fileMode |= (O_RDWR | O_CREAT);
        sd->mWriteEnabled = true;
    }
    if ((flags & 0x100)) {
        fileMode |= O_APPEND;
    }
    RB3E_DEBUG("fileMode = 0x%x", fileMode);

    // open the file
    sd->mFd = SD_open(&sd->mFileStruct, filepath, fileMode);
    if (sd->mFd == -1) {
        RB3E_DEBUG("Failed to open file '%s'", filepath);
        FileSD_Destroy(sd);
        return NULL;
    }

    sd->mFilesize = sd->mFileStruct.filesize;

    return sd;
}

String *FileSD_Filename(String *str, FileSD *file)
{
    // really we should be creating a new SD card
    RB3E_DEBUG("FileSD_Filename(str=%p, file=%p)", str, file);
    str->length = strlen(file->mFilePath);
    str->buf = file->mFilePath;
    return str;
}

int FileSD_Read(FileSD *file, void *iData, int iBytes) {
    //RB3E_DEBUG("FileSD_Read(file=%p, iData=%p, iBytes=%i)", file, iData, iBytes);
    int read = SD_read(file->mFd, iData, iBytes);
    if (read >= 0) {
        file->mCurrentPosition += read;
    }
    return read;
}

char FileSD_ReadAsync(FileSD *file, void *iData, int iBytes) {
    // TODO(Emma): async SD card reads
    //RB3E_DEBUG("FileSD_ReadAsync(file=%p, iData=%p, iBytes=%i)", file, iData, iBytes);
    int read = FileSD_Read(file, iData, iBytes);
    file->mLastBytesRead = read;
    return (read >= 0);
}

int FileSD_Write(FileSD *file, void *iData, int iBytes) {
    // RB3E_DEBUG("FileSD_Write(file=%p, iData=%p, iBytes=%i)", file, iData, iBytes);
    int wrote = SD_write(file->mFd, iData, iBytes);
    if (wrote >= 0) {
        file->mCurrentPosition += wrote;
    }
    return wrote;
}

char FileSD_WriteAsync(FileSD *file, void *iData, int iBytes) {
    // TODO(Emma): async SD card writes
    // RB3E_DEBUG("FileSD_WriteAsync(file=%p, iData=%p, iBytes=%i)", file, iData, iBytes);
    int wrote = FileSD_Write(file, iData, iBytes);
    file->mLastBytesWrote = wrote;
    return (wrote >= 0);
}

int FileSD_Seek(FileSD *file, int iOffset, int iSeekType) {
    RB3E_DEBUG("FileSD_Seek(file=%p, iOffset=%i, iSeekType=%i)", file, iOffset, iSeekType);
    switch (iSeekType) {
    case SEEK_SET:
        file->mCurrentPosition = iOffset;
        break;
    case SEEK_CUR:
        file->mCurrentPosition += iOffset;
        break;
    case SEEK_END:
        file->mCurrentPosition = file->mFilesize - iOffset;
        break;
    }
    file->mCurrentPosition = SD_seek(file->mFd, file->mCurrentPosition, SEEK_SET);
    return file->mCurrentPosition;
}

int FileSD_Tell(FileSD *file) {
    RB3E_DEBUG("FileSD_Tell(file=%p)", file);
    return file->mCurrentPosition;
}

void FileSD_Flush(FileSD *file) {
    RB3E_DEBUG("FileSD_Flush(file=%p)", file);
    return;
}

char FileSD_Eof(FileSD *file) {
    //RB3E_DEBUG("FileSD_Eof(file=%p)", file);
    return (file->mCurrentPosition >= file->mFilesize);
}

char FileSD_Fail(FileSD *file) {
    //RB3E_DEBUG("FileSD_Fail(file=%p)", file);
    return file->mFailed;
}

int FileSD_Size(FileSD *file) {
    RB3E_DEBUG("FileSD_Size(file=%p)", file);
    return file->mFilesize;
}

char FileSD_ReadDone(FileSD *file, int *oBytes) {
    //RB3E_DEBUG("FileSD_ReadDone(file=%p)", file);
    *oBytes = file->mLastBytesRead;
    return 1;
}

char FileSD_WriteDone(FileSD *file, int *oBytes) {
    //RB3E_DEBUG("FileSD_WriteDone(file=%p)", file);
    *oBytes = file->mLastBytesWrote;
    return 0;
}

void FileSD_InitVtable() {
    FileSD_vtable.destructor = (FileDestructor_t)FileSD_Destroy;
    FileSD_vtable.Filename = (FileFilename_t)FileSD_Filename;
    FileSD_vtable.Read = (FileRead_t)FileSD_Read;
    FileSD_vtable.ReadAsync = (FileReadAsync_t)FileSD_ReadAsync;
    FileSD_vtable.Write = (FileWrite_t)FileSD_Write;
    FileSD_vtable.WriteAsync = (FileWriteAsync_t)FileSD_WriteAsync;
    FileSD_vtable.Seek = (FileSeek_t)FileSD_Seek;
    FileSD_vtable.Tell = (FileTell_t)FileSD_Tell;
    FileSD_vtable.Flush = (FileFlush_t)FileSD_Flush;
    FileSD_vtable.Eof = (FileEof_t)FileSD_Eof;
    FileSD_vtable.Fail = (FileFail_t)FileSD_Fail;
    FileSD_vtable.Size = (FileSize_t)FileSD_Size;
    FileSD_vtable.UncompressedSize = (FileUncompressedSize_t)FileSD_Size;
    FileSD_vtable.ReadDone = (FileReadDone_t)FileSD_ReadDone;
    FileSD_vtable.WriteDone = (FileWriteDone_t)FileSD_WriteDone;
}

#endif // RB3E_WII
