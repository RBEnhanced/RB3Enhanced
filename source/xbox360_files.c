/*
    RB3Enhanced - xbox360_files.c
    Xbox 360 filesystem functions.
*/

#ifdef RB3E_XBOX

#include <xtl.h>
#include <stdio.h>
#include "rb3/Mem.h"
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
        // TODO: Figure out why paths containing + do not work
        // if path contains + character, ignore it
        if (path[i] == '+')
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
    DWORD desiredAccess = readWrite ? (GENERIC_READ | GENERIC_WRITE) : GENERIC_READ;
    DWORD shareMode = readWrite ? (FILE_SHARE_READ | FILE_SHARE_WRITE) : FILE_SHARE_READ;
    DWORD creationDisposition = readWrite ? OPEN_ALWAYS : OPEN_EXISTING;
    HANDLE handle = CreateFile(filename, desiredAccess, shareMode, NULL, creationDisposition, 0, NULL);
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
int RB3E_WriteFile(int file, int offset, void *buffer, int size)
{
    DWORD wroteBytes;
    SetFilePointer((HANDLE)file, offset, NULL, FILE_BEGIN);
    if (WriteFile((HANDLE)file, buffer, size, &wroteBytes, NULL) == FALSE)
        return -1;
    return wroteBytes;
}
void RB3E_CloseFile(int file)
{
    XCloseHandle((HANDLE)file);
}

char** RB3E_ListFiles(const char* directoryPath, const char* extension, int* fileCount)
{
    // c89 compliance
    char searchPath[MAX_PATH];
    WIN32_FIND_DATA findFileData;
    HANDLE findHandle;
    int count = 0;
    int dirPathLength = strlen(directoryPath);
    int currentIdx = 0;
    char** fileList = NULL;

    *fileCount = 0;

    if (dirPathLength > 0 && directoryPath[dirPathLength - 1] == '\\') {
        sprintf(searchPath, "%s*.%s", directoryPath, extension);
    } else {
        sprintf(searchPath, "%s\\*.%s", directoryPath, extension);
    }

    // count the number of files
    findHandle = FindFirstFile(searchPath, &findFileData);
    if (findHandle == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    do {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            count++;
        }
    } while (FindNextFile(findHandle, &findFileData) != 0);

    FindClose(findHandle);

    if (count == 0) {
        return NULL;
    }

    fileList = (char**)MemAlloc(count * sizeof(char*), 0);
    if (!fileList) {
        // the game is probably already cooked if we hit this, but just in case
        RB3E_MSG("RB3E_ListFiles: Failed to allocate memory for file list", NULL);
        return NULL;
    }
    memset(fileList, 0, count * sizeof(char*));

    // actually buidl the paths
    findHandle = FindFirstFile(searchPath, &findFileData);
    if (findHandle == INVALID_HANDLE_VALUE) {
        MemFree(fileList);
        return NULL;
    }

    // i hate win32 apis
    do {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {

            int needsSlash = 0;
            int fullPathLen;
            char* fullPath;
            if (dirPathLength > 0 && directoryPath[dirPathLength - 1] != '\\') {
                needsSlash = 1;
            }

            fullPathLen = dirPathLength;
            if (needsSlash) {
                fullPathLen += 1;
            }
            fullPathLen += strlen(findFileData.cFileName);
            fullPathLen += 1;

            fullPath = (char*)MemAlloc(fullPathLen, 0);
            if (!fullPath) {
                continue;
            }

            if (needsSlash) {
                sprintf(fullPath, "%s\\%s", directoryPath, findFileData.cFileName);
            } else {
                sprintf(fullPath, "%s%s", directoryPath, findFileData.cFileName);
            }

            fileList[currentIdx++] = fullPath;
        }

    } while (FindNextFile(findHandle, &findFileData) != 0 && currentIdx < count);

    FindClose(findHandle);

    *fileCount = currentIdx;
    return fileList;
}

// frees the file list using the games MemFree
void RB3E_FreeFileList(char** fileList, int fileCount)
{
    int i;
    if (!fileList) return;
    for (i = 0; i < fileCount; i++) {
        if (fileList[i])
        {
            MemFree(fileList[i]);
        }
    }
    MemFree(fileList);
}

#endif