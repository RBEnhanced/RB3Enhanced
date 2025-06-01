#ifndef _WIIMEMMGR_H
#define _WIIMEMMGR_H
#ifdef RB3E_WII

typedef struct _FreeBlock FreeBlock;

typedef struct _FreeBlock
{
    unsigned int mSizeWords;
    unsigned int mTimeStamp;
    FreeBlock *mNextBlock;
} FreeBlock;

typedef struct _Heap
{
    FreeBlock *freeBlockChain;
    int *mStart;
    const char *mName;
    int mSizeWords;
    int mNum;
    int mIsHandleHeap;
    int mDebugLevel;
    int mStrategy;
    int mAllowTemp;
    int unk_1;
    int unk_2;
    int unk_3;
    int unk_4;
} Heap;

#endif // RB3E_WII
#endif // _WIIMEMMGR_H
