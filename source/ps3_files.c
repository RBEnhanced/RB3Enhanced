
#ifdef RB3E_PS3

#include <cell/cell_fs.h>
#include <string.h>
#include "ports.h"
#include "config.h"
#include "rb3enhanced.h"

int RB3E_Mounted = 0;

void RB3E_MountFileSystems()
{
    RB3E_Mounted = 1;
}

int RB3E_FileExists(char *filename)
{
    CellFsStat st;
    // make sure there's a mounted card before trying
    if (RB3E_Mounted == 0)
        return 0;
    return (cellFsStat(filename, &st) == CELL_FS_SUCCEEDED);
}

static char path_to_check[300] = {0};
char *RB3E_GetRawfilePath(char *path, int root_allowed)
{
    // TODO(Emma): this function likely has a lot of inefficiencies
    // it shouldn't affect in-game performance but it'd be nice to load faster
    char corrected_path[256] = {0};
    int path_length = strlen(path);
    int i = 0;
    // if its bigger than this, we're unsafe, we might buffer overflow
    if (path_length > sizeof(corrected_path))
        return NULL;
    // run through our original path
    for (i = 0; i < path_length; i++)
    {
        // if this is already a drive path, we shouldn't patch that
        if (path[i] == ':')
            return NULL;
        // TODO: Figure out why paths containing + do not work
        // if path contains + character, ignore it
        if (path[i] == '+')
            return NULL;
        // if we're a .. path, change to x.
        if (path[i] == '.' && (path[i + 1] == '.' || path[i + 1] == '/'))
            corrected_path[i] = 'x';
        else
            corrected_path[i] = path[i];
    }
    // scan GD: to see if we have it there
    sprintf(path_to_check, "/dev_hdd0/rb3/%s", corrected_path);
    if (RB3E_FileExists(path_to_check))
        return path_to_check;
    // we don't have it, just give up
    return NULL;
}

int RB3E_OpenFile(char *filename, char readWrite)
{
    int fd = -1;
    int openflags = CELL_FS_O_RDONLY;
    if (readWrite == 1)
        openflags = CELL_FS_O_CREAT | CELL_FS_O_RDWR;
    CellFsErrno r = cellFsOpen(filename, openflags, &fd, NULL, 0);
    return r == CELL_FS_SUCCEEDED ? fd : -1;
}

int RB3E_FileSize(int file)
{
    CellFsStat st = {0};
    cellFsFstat(file, &st);
    return st.st_size;
}

int RB3E_ReadFile(int file, int offset, void *buffer, int size)
{
    uint64_t read_bytes = 0;
    CellFsErrno r = cellFsReadWithOffset(file, offset, buffer, size, &read_bytes);
    return r == CELL_FS_SUCCEEDED ? read_bytes : -1;
}

int RB3E_WriteFile(int file, int offset, void *buffer, int size)
{
    uint64_t write_bytes = 0;
    CellFsErrno r = cellFsWriteWithOffset(file, offset, buffer, size, &write_bytes);
    return r == CELL_FS_SUCCEEDED ? write_bytes : -1;
}

void RB3E_CloseFile(int file)
{
    cellFsClose(file);
}

#endif // RB3E_PS3
