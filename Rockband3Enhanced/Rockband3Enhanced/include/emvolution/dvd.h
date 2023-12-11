#ifdef RB3E_WII
#include <stdint.h>
#include <stdbool.h>
typedef struct DVDFileInfo DVDFileInfo;
typedef void (*DVDCallback)(int32_t result, DVDFileInfo* fileInfo);
struct DVDFileInfo
{
    char unk[0x30];
    uint32_t start;
    uint32_t filesize;
    DVDCallback cb;
};

int32_t DVDConvertPathToEntrynum(const char* path);
bool DVDOpen(const char* path, DVDFileInfo* fileInfo);
bool DVDFastOpen(int32_t entrynum, DVDFileInfo* fileInfo);
bool DVDClose(DVDFileInfo* fileInfo);
int DVDReadPrio(DVDFileInfo* fileInfo, void* buf, int len, int offset, int prio);
void DVDReadAsyncPrio(DVDFileInfo* fileInfo, void* buf, int len, int offset, DVDCallback cb, int prio);
int DVDGetCommandBlockStatus(DVDFileInfo *fileInfo);
#endif