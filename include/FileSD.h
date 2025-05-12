#ifdef RB3E_WII

#include <io/fat-sd.h>
#include "rb3/File.h"

typedef struct _FileSD
{
    File_vtable *vtable; // 0x0

    char *mFilePath;
    char mWriteEnabled;
    char mFailed;
    int mFd;
    FILE_STRUCT mFileStruct;
    int mFilesize;
    int mCurrentPosition;
    int mLastBytesRead;
    int mLastBytesWrote;

} FileSD;

void FileSD_InitVtable();
FileSD *FileSD_New(const char *filepath, int flags);

#endif
