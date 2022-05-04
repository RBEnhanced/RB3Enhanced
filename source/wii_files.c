/*
    RB3Enhanced - wii_files.c
    Wii filesystem functions.
*/

#ifdef RB3E_WII

#include <bslug.h>
#include <stdio.h>
#include <io/fat-sd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ports.h"

int RB3E_Mounted = 0;

void RB3E_MountFileSystems()
{
    RB3E_Mounted = (SD_Mount() == 0);
}
int RB3E_FileExists(const char *filename)
{
    static struct stat st;
    return (SD_stat(filename, &st) == 0);
}
int RB3E_OpenFile(const char *filename, char readWrite)
{
    static FILE_STRUCT st;
    return SD_open(&st, filename, O_RDONLY);
}
int RB3E_FileSize(int file)
{
    static struct stat fs;
    if (SD_fstat(file, &fs) != 0)
        return -1;
    return (int)fs.st_size; // not accurate? but works
}
int RB3E_ReadFile(int file, int offset, void *buffer, int size)
{
    SD_seek(file, offset, 0);
    return SD_read(file, buffer, size);
}
void RB3E_CloseFile(int file)
{
    SD_close(file);
}

#endif