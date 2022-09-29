/*
    RB3Enhanced - xbox360_files.c
    Xbox 360 filesystem functions.
*/

#ifdef RB3E_XBOX

#include <xtl.h>
#include <stdio.h>
#include "ports.h"
#include "config.h"
#include "xbox360.h"
#include "rb3enhanced.h"

int RB3E_Mounted = 0;
char *mountToPoint[][2] = {
    {"RB3HDD:", "\\Device\\Harddisk0\\Partition1"},
    {"RB3USB0:", "\\Device\\Mass0"},
    {"RB3USB1:", "\\Device\\Mass1"},
    {"RB3USB2:", "\\Device\\Mass2"},
    // TODO(Emma): some consoles will report a different error from these devices
    // {"RB3INTUSB:", "\\Device\\BuiltInMuUsb\\Storage"},
    // {"RB3INTMMC:", "\\Device\\BuiltInMuMmc\\Storage"},
    // {"RB3MUINT:", "\\Device\\BuiltInMuSfc"},
};
int numMountToPoint = sizeof(mountToPoint) / sizeof(mountToPoint[0]);

BOOL RB3E_MountXboxDrive(char *mountPoint, char *deviceName)
{
    char LinkName[20] = {0};
    STRING sLinkName, sDeviceName;
    strcat(LinkName, "\\??\\");
    strcat(LinkName, mountPoint);
    RtlInitAnsiString(&sLinkName, LinkName);
    RtlInitAnsiString(&sDeviceName, deviceName);
    return ObCreateSymbolicLink(&sLinkName, &sDeviceName) >= 0;
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
        // if we're a .. path, change to x.
        if (path[i] == '.' && (path[i + 1] == '.' || path[i + 1] == '/'))
            corrected_path[i] = 'x';
        // the 360 OS expects \paths\like\this because its stupid and dumb
        else if (path[i] == '/')
            corrected_path[i] = '\\';
        else
            corrected_path[i] = path[i];
    }
    // scan GAME: to see if we have it there
    sprintf(path_to_check, "GAME:\\%s", corrected_path);
    if (RB3E_FileExists(path_to_check))
        return path_to_check;
    // scan through every drive's "rb3" folder
    for (i = 0; i < numMountToPoint; i++)
    {
        // check the root of the drive, basically only for config
        if (root_allowed)
        {
            sprintf(path_to_check, "%s\\%s", mountToPoint[i][0], corrected_path);
            if (RB3E_FileExists(path_to_check))
                return path_to_check;
        }
        sprintf(path_to_check, "%s\\%s\\%s", mountToPoint[i][0], config.RawfilesDir, corrected_path);
        if (RB3E_FileExists(path_to_check))
            return path_to_check;
    }
    // we don't have it, just give up
    return NULL;
}

void RB3E_MountFileSystems()
{
    int i = 0;
    for (i = 0; i < numMountToPoint; i++)
    {
        RB3E_Mounted += RB3E_MountXboxDrive(mountToPoint[i][0], mountToPoint[i][1]);
    }
    RB3E_DEBUG("Mounted %i drives.", RB3E_Mounted);
}
int RB3E_FileExists(char *filename)
{
    DWORD attr = GetFileAttributesA(filename);
    DWORD lastError = GetLastError();
    return !(attr == -1 && (lastError == 2 || lastError == 3 || lastError == 1617));
}
int RB3E_OpenFile(char *filename, char readWrite)
{
    HANDLE handle = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (handle == INVALID_HANDLE_VALUE)
        return -1;
    return (int)handle;
}
int RB3E_FileSize(int file)
{
    return GetFileSize((HANDLE)file, NULL);
}
int RB3E_ReadFile(int file, int offset, void *buffer, int size)
{
    DWORD readBytes;
    SetFilePointer((HANDLE)file, offset, NULL, FILE_BEGIN);
    if (ReadFile((HANDLE)file, buffer, size, &readBytes, NULL) == FALSE)
        return -1;
    return readBytes;
}
void RB3E_CloseFile(int file)
{
    XCloseHandle((HANDLE)file);
}

#endif