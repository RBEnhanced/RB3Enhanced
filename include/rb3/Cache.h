
#ifndef _CACHE_H
#define _CACHE_H

typedef struct _Cache
{
    void *vtable;
    int mOpCur;
    int mLastResult;
} Cache;

typedef struct _CacheMgr
{
    void *vtable;
    int mCacheIDStore[3]; // vector
    int mOpCur;
    int mLastResult;
} CacheMgr;

#endif // _CACHE_H
