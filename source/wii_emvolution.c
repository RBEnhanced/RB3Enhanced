/*
    RB3Enhanced - wii_emvolution.c
    Emvolution engine modified for usage in RB3Enhanced.
    https://github.com/InvoxiPlayGames/emvolution
    See original repo for license.
*/

#ifdef RB3E_WII

#include <bslug.h>
#include <stdio.h>
#include <rvl/cache.h>
#include <io/fat-sd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "emvolution/dvd.h"

static bool DVDOpenHook(const char *path, DVDFileInfo *fileInfo)
{
    static FILE_STRUCT fs;
    int fd = -1;
    if (path[2] == ':')
    {
        fd = SD_open(&fs, path, O_RDONLY);
        if (fd == -1)
        {
            path += 8;
            goto OPEN_ORIG;
        }
        fileInfo->start = fd;
        fileInfo->filesize = fs.filesize;
        return true;
    }
OPEN_ORIG:
    return DVDOpen(path, fileInfo);
}
static int DVDConvertPathToEntrynumHook(const char *path)
{
    static FILE_STRUCT fs;
    int fd = -1;
    if (path[2] == ':')
    {
        fd = SD_open(&fs, path, O_RDONLY);
        if (fd == -1)
        {
            path += 8;
            goto OPEN_ORIG;
        }
        return fd;
    }
OPEN_ORIG:
    return DVDConvertPathToEntrynum(path);
}
static bool DVDFastOpenHook(int entrynum, DVDFileInfo *fileInfo)
{
    static struct stat fs;
    int fd = -1;
    if (entrynum >= 0x80000000)
    {
        fd = entrynum;
        if (SD_fstat(fd, &fs) != 0)
        {
            goto OPEN_ORIG;
        }
        fileInfo->start = entrynum;
        fileInfo->filesize = fs.st_size; // not accurate? but it works
        return true;
    }
OPEN_ORIG:
    return DVDFastOpen(entrynum, fileInfo);
}
static int DVDReadHook(DVDFileInfo *fileInfo, void *buf, int len, int offset, int prio)
{
    int fd = -1;
    int read = 0;
    if (fileInfo->start > 0x80000000)
    {
        fd = fileInfo->start;
        SD_seek(fd, offset, 0);
        read = SD_read(fd, buf, len);
        DCFlushRange(buf, len);
        return read;
    }
    return DVDReadPrio(fileInfo, buf, len, offset, prio);
}
static int DVDGetCommandBlockStatusHook(DVDFileInfo *fileInfo)
{
    if (fileInfo->start > 0x80000000)
    {
        return 0;
    }
    return DVDGetCommandBlockStatus(fileInfo);
}
static void DVDReadAsyncHook(DVDFileInfo *fileInfo, void *buf, int len, int offset, DVDCallback cb, int prio)
{
    int fd = -1;
    int read = 0;
    if (fileInfo->start > 0x80000000)
    {
        fd = fileInfo->start;
        SD_seek(fd, offset, 0);
        read = SD_read(fd, buf, len);
        DCFlushRange(buf, len);
        if (cb != NULL)
            cb(read, fileInfo); // this isn't async, but it works..? maybe causes hitching, TODO: fix
        return;
    }
    return DVDReadAsyncPrio(fileInfo, buf, len, offset, cb, prio);
}
static bool DVDCloseHook(DVDFileInfo *fileInfo)
{
    int fd = -1;
    if (fileInfo->start > 0x80000000)
    {
        fd = fileInfo->start;
        return (SD_close(fd) == 0);
    }
    return DVDClose(fileInfo);
}
BSLUG_MUST_REPLACE(DVDOpen, DVDOpenHook);
BSLUG_MUST_REPLACE(DVDConvertPathToEntrynum, DVDConvertPathToEntrynumHook);
BSLUG_MUST_REPLACE(DVDFastOpen, DVDFastOpenHook);
BSLUG_MUST_REPLACE(DVDGetCommandBlockStatus, DVDGetCommandBlockStatusHook);
BSLUG_MUST_REPLACE(DVDReadPrio, DVDReadHook);
BSLUG_MUST_REPLACE(DVDReadAsyncPrio, DVDReadAsyncHook);
BSLUG_MUST_REPLACE(DVDClose, DVDCloseHook);

#endif