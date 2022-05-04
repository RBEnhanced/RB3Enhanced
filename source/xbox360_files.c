/*
    RB3Enhanced - xbox360_files.c
    Xbox 360 filesystem functions.
*/

#ifdef RB3E_XBOX

#include <xtl.h>
#include <stdio.h>
#include "ports.h"
#include "xbox360.h"

int RB3E_Mounted = 0;
char *mountToPoint[][2] = {
    {"RB3HDD:", "\\Device\\Harddisk0\\Partition1"},
    {"RB3USB0:", "\\Device\\Mass0"},
    {"RB3USB1:", "\\Device\\Mass1"},
    {"RB3USB2:", "\\Device\\Mass2"},
    {"RB3INTUSB:", "\\Device\\BuiltInMuUsb\\Storage"},
    {"RB3INTMMC:", "\\Device\\BuiltInMuMmc\\Storage"},
    {"RB3MUINT:", "\\Device\\BuiltInMuSfc"},
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
    return !(attr == -1 && (lastError == 2 || lastError == 3));
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