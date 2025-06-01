
#ifndef _XBOXCACHE_H
#define _XBOXCACHE_H
#ifdef RB3E_XBOX

#include <xtl.h>
#include "rb3/Cache.h"
#include "rb3/String.h"
#include "rb3/Symbol.h"

typedef struct _CacheIDXbox
{
    void *vtable;
    String mStrCacheName;
    XCONTENT_DATA mContentData;
} CacheIDXbox;

typedef struct _CacheMgrXbox
{
    CacheMgr root;
    int mEnumerationHandle;
    XOVERLAPPED mOverlapped;
    CacheIDXbox **mOutCacheIDPtr;
    void *mPendingCachePtr;
    void *mPendingObject;
    CacheIDXbox *mPendingCacheID;
    XCONTENT_DATA mContentData;
    String mCacheSearchName;
    String unk;
} CacheMgrXbox;

int CacheMgrXbox_MountAsync(CacheMgrXbox *thisCacheMgr, CacheIDXbox *cacheID, Cache **outCache, void *object);

#endif // RB3E_XBOX
#endif // _XBOXCACHE_H
