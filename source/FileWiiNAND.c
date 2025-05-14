#ifdef RB3E_WII

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <rvl/ipc.h>
#include <fcntl.h>
#include "FileWiiNAND.h"
#include "rb3/File.h"
#include "rb3/Mem.h"
#include "rb3enhanced.h"
#include "ports.h"

#define LE16(i) ((((i) & 0xFF) << 8 | ((i) >> 8) & 0xFF) & 0xFFFF)
#define LE(i) (((i) & 0xff) << 24 | ((i) & 0xff00) << 8 | ((i) & 0xff0000) >> 8 | ((i) >> 24) & 0xff)

File_vtable FileWiiNAND_vtable;

FileWiiNAND *FileWiiNAND_Destroy(FileWiiNAND *file)
{
    RB3E_DEBUG("FileWiiNAND_Destroy(file=%p)", file);
    if (file->mFd != -1)
        IOS_Close(file->mFd);
    if (file->mFilePath != NULL)
        MemFree(file->mFilePath);
    MemFree(file);
    return NULL;
}

typedef struct _nand_file_stats_t {
    uint32_t length;
    uint32_t pos;
} nand_file_stats_t;

FileWiiNAND *FileWiiNAND_New(const char *filepath)
{
    // never ever allow this on real hardware
    if (RB3E_IsEmulator())
        return NULL;

    RB3E_DEBUG("FileWiiNAND_New(filepath='%s')", filepath);
    // create the new object
    FileWiiNAND *nd = MemAlloc(sizeof(FileWiiNAND), 0);
    if (nd == NULL) {
        RB3E_DEBUG("Not enough memory to read file '%s' from NAND!", filepath);
        return NULL;
    }
    memset(nd, 0, sizeof(FileWiiNAND));
    FileWiiNAND_InitVtable(); // TODO(Emma): find a better place to put this
    nd->vtable = &FileWiiNAND_vtable;
    nd->mFd = -1;

    // set the file path
    nd->mFilePath = MemAlloc(strlen(filepath) + 1, 0);
    if (nd->mFilePath  == NULL) {
        RB3E_DEBUG("Not enough memory to read file '%s' from NAND!", filepath);
        FileWiiNAND_Destroy(nd);
        return NULL;
    }
    strcpy(nd->mFilePath, filepath);

    // open the file
    nd->mFd = IOS_Open(filepath, IPC_OPEN_READ);
    if (nd->mFd == -1) {
        RB3E_DEBUG("Failed to open file '%s'", filepath);
        FileWiiNAND_Destroy(nd);
        return NULL;
    }
    
    // get the filesize
    nand_file_stats_t stats __attribute__((aligned(32)));
    ios_ret_t r = IOS_Ioctl(nd->mFd, 11, NULL, 0, &stats, sizeof(nand_file_stats_t));
    if (r != IPC_OK) {
        RB3E_DEBUG("IOS returned %i trying to get stats", r);
        FileWiiNAND_Destroy(nd);
        return NULL;
    }

    nd->mFilesize = stats.length;

    return nd;
}

String *FileWiiNAND_Filename(String *str, FileWiiNAND *file)
{
    RB3E_DEBUG("FileWiiNAND_Filename(str=%p, file=%p)", str, file);
    str->length = strlen(file->mFilePath);
    str->buf = file->mFilePath;
    return str;
}

int FileWiiNAND_Read(FileWiiNAND *file, void *iData, int iBytes) {
    //RB3E_DEBUG("FileWiiNAND_Read(file=%p, iData=%p, iBytes=%i)", file, iData, iBytes);
    int read = IOS_Read(file->mFd, iData, iBytes);
    if (read >= 0) {
        file->mCurrentPosition += read;
    }
    return read;
}

char FileWiiNAND_ReadAsync(FileWiiNAND *file, void *iData, int iBytes) {
    // TODO(Emma): async SD card reads
    //RB3E_DEBUG("FileWiiNAND_ReadAsync(file=%p, iData=%p, iBytes=%i)", file, iData, iBytes);
    int read = FileWiiNAND_Read(file, iData, iBytes);
    file->mLastBytesRead = read;
    return (read >= 0);
}

int FileWiiNAND_Write(FileWiiNAND *file, void *iData, int iBytes) {
    return 0;
}

char FileWiiNAND_WriteAsync(FileWiiNAND *file, void *iData, int iBytes) {
    return 0;
}

int FileWiiNAND_Seek(FileWiiNAND *file, int iOffset, int iSeekType) {
    RB3E_DEBUG("FileWiiNAND_Seek(file=%p, iOffset=%i, iSeekType=%i)", file, iOffset, iSeekType);
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
    IOS_Seek(file->mFd, file->mCurrentPosition, SEEK_SET);
    return file->mCurrentPosition;
}

int FileWiiNAND_Tell(FileWiiNAND *file) {
    RB3E_DEBUG("FileWiiNAND_Tell(file=%p)", file);
    return file->mCurrentPosition;
}

void FileWiiNAND_Flush(FileWiiNAND *file) {
    RB3E_DEBUG("FileWiiNAND_Flush(file=%p)", file);
    return;
}

char FileWiiNAND_Eof(FileWiiNAND *file) {
    //RB3E_DEBUG("FileWiiNAND_Eof(file=%p)", file);
    return (file->mCurrentPosition >= file->mFilesize);
}

char FileWiiNAND_Fail(FileWiiNAND *file) {
    //RB3E_DEBUG("FileWiiNAND_Fail(file=%p)", file);
    return file->mFailed;
}

int FileWiiNAND_Size(FileWiiNAND *file) {
    RB3E_DEBUG("FileWiiNAND_Size(file=%p)", file);
    return file->mFilesize;
}

char FileWiiNAND_ReadDone(FileWiiNAND *file, int *oBytes) {
    //RB3E_DEBUG("FileWiiNAND_ReadDone(file=%p)", file);
    *oBytes = file->mLastBytesRead;
    return 1;
}

char FileWiiNAND_WriteDone(FileWiiNAND *file, int *oBytes) {
    //RB3E_DEBUG("FileWiiNAND_WriteDone(file=%p)", file);
    *oBytes = file->mLastBytesWrote;
    return 0;
}

void FileWiiNAND_InitVtable() {
    FileWiiNAND_vtable.destructor = (FileDestructor_t)FileWiiNAND_Destroy;
    FileWiiNAND_vtable.Filename = (FileFilename_t)FileWiiNAND_Filename;
    FileWiiNAND_vtable.Read = (FileRead_t)FileWiiNAND_Read;
    FileWiiNAND_vtable.ReadAsync = (FileReadAsync_t)FileWiiNAND_ReadAsync;
    FileWiiNAND_vtable.Write = (FileWrite_t)FileWiiNAND_Write;
    FileWiiNAND_vtable.WriteAsync = (FileWriteAsync_t)FileWiiNAND_WriteAsync;
    FileWiiNAND_vtable.Seek = (FileSeek_t)FileWiiNAND_Seek;
    FileWiiNAND_vtable.Tell = (FileTell_t)FileWiiNAND_Tell;
    FileWiiNAND_vtable.Flush = (FileFlush_t)FileWiiNAND_Flush;
    FileWiiNAND_vtable.Eof = (FileEof_t)FileWiiNAND_Eof;
    FileWiiNAND_vtable.Fail = (FileFail_t)FileWiiNAND_Fail;
    FileWiiNAND_vtable.Size = (FileSize_t)FileWiiNAND_Size;
    FileWiiNAND_vtable.UncompressedSize = (FileUncompressedSize_t)FileWiiNAND_Size;
    FileWiiNAND_vtable.ReadDone = (FileReadDone_t)FileWiiNAND_ReadDone;
    FileWiiNAND_vtable.WriteDone = (FileWriteDone_t)FileWiiNAND_WriteDone;
}

#endif // RB3E_WII
