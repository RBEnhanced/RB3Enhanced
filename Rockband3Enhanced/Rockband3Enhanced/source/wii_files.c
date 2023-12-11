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
#include "../include/ports.h"
#include "../include/config.h"
#include "../include/rb3enhanced.h"

int RB3E_Mounted = 0;

void RB3E_MountFileSystems()
{
    RB3E_Mounted = (SD_Mount() == 0);
}
static char path_to_check[300] = {0};
char *RB3E_GetRawfilePath(char *path, int root_allowed)
{
    char corrected_path[256] = {0};
    int path_length = strlen(path);
    int i = 0;
    // if the drive isn't mounted, don't bother
    RB3E_Mounted = 0;
    // if its bigger than this, we're unsafe, we might buffer overflow
    if (path_length > sizeof(corrected_path))
        return NULL;
    // if we're loading original DLC, don't touch any paths
    if (strncmp(path, "dlc/", 4) == 0)
        return NULL;
    if (strncmp(path, "nand/", 5) == 0)
        return NULL;
    // run through our original path
    for (i = 0; i < path_length; i++)
    {
        // if we're a .. path, change to x.
        if (path[i] == '.' && (path[i + 1] == '.' || path[i + 1] == '/'))
            corrected_path[i] = 'x';
        else
            corrected_path[i] = path[i];
    }
    // check the SD card to see if we have that file
    if (root_allowed)
    {
        sprintf(path_to_check, "sd:/%s", corrected_path);
        if (RB3E_FileExists(path_to_check))
            return path_to_check;
    }
    sprintf(path_to_check, "sd:/%s/%s", config.RawfilesDir, corrected_path);
    if (RB3E_FileExists(path_to_check))
        return path_to_check;
    // we don't have it, just give up
    return NULL;
}
int RB3E_FileExists(char *filename)
{
    static struct stat st;
    return (SD_stat(filename, &st) == 0);
}
int RB3E_OpenFile(char *filename, char readWrite)
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