#ifdef RB3E_WII

#include <rvl/ipc.h>
#include "rb3/File.h"

typedef struct _FileWiiNAND
{
    File_vtable *vtable; // 0x0

    char *mFilePath;
    char mWriteEnabled;
    char mFailed;
    ios_fd_t mFd;
    int mFilesize;
    int mCurrentPosition;
    int mLastBytesRead;
    int mLastBytesWrote;
} FileWiiNAND;

void FileWiiNAND_InitVtable();
FileWiiNAND *FileWiiNAND_New(const char *filepath);

#endif
