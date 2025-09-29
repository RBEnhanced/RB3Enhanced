/*
    RB3Enhanced - xbox360_content.c
    Xbox 360 content and cache hooks.
*/

#ifdef RB3E_XBOX

#include <xtl.h>
#include <string.h>
#include <stdio.h>
#include "rb3/XboxCache.h"
#include "rb3/XboxContent.h"
#include "config.h"
#include "ports.h"
#include "utilities.h"
#include "rb3enhanced.h"
#include "xbox360.h"

int hasLastMountedCache = 0;
CacheIDXbox lastMountedCache;

int RB3E_DeleteSongCache()
{
    DWORD r;
    XDEVICE_DATA deviceData;
    if (hasLastMountedCache == 0)
    {
        RB3E_MSG("Tried to delete a song cache, but we don't have one...", NULL);
        return 0;
    }
    if (XContentGetDeviceData(lastMountedCache.mContentData.DeviceID, &deviceData) == 0)
    {
        RB3E_DEBUG("Deleting song cache '%s' on \"%S\"", lastMountedCache.mContentData.szFileName, deviceData.wszFriendlyName);
        r = XContentDelete(0xFF, &lastMountedCache.mContentData, NULL);
        if (r != 0)
        {
            RB3E_MSG("Song cache delete failed (%i)", r);
            return 0;
        }
        return 1;
    }
    else
    {
        RB3E_MSG("Tried to delete a song cache, but the device is gone...", NULL);
        return 0;
    }
}

int CacheMgrXbox_MountAsync_Hook(CacheMgrXbox *thisCacheMgr, CacheIDXbox *cacheID, Cache **outCache, void *object)
{
    XDEVICE_DATA deviceData;
    int r = CacheMgrXbox_MountAsync(thisCacheMgr, cacheID, outCache, object);
    if ((strcmp(cacheID->mContentData.szFileName, "songcache") == 0 || strcmp(cacheID->mContentData.szFileName, "rbdxcache") == 0) &&
        XContentGetDeviceData(cacheID->mContentData.DeviceID, &deviceData) == 0)
    {
        RB3E_DEBUG("Song cache '%s' on \"%S\" mounted", cacheID->mContentData.szFileName, deviceData.wszFriendlyName);
        memcpy(&lastMountedCache, cacheID, sizeof(CacheIDXbox));
        hasLastMountedCache = 1;
    }
    return r;
}

XboxContent *XboxContentConstructHook(XboxContent *thisXboxContent, XCONTENT_CROSS_TITLE_DATA *titleData, int contentIndex, int padNum, char needsMount)
{
    XDEVICE_DATA deviceData;
    XboxContent *r = XboxContentConstruct(thisXboxContent, titleData, contentIndex, padNum, needsMount);
    if (config.ContentLogging && XContentGetDeviceData(titleData->DeviceID, &deviceData) == 0)
    {
        RB3E_DEBUG("Content \"%S\" (\"%S\", %08x/%08x/%s) = %s",
            thisXboxContent->mTitleData.szDisplayName,
            deviceData.wszFriendlyName,
            thisXboxContent->mTitleData.dwTitleId, thisXboxContent->mTitleData.dwContentType, thisXboxContent->mTitleData.szFileName,
            thisXboxContent->mRoot.buf
        );
    }
    return r;
}

void InitContentHooks()
{
    HookFunction(PORT_CACHEMGRXBOX_MOUNTASYNC, &CacheMgrXbox_MountAsync, &CacheMgrXbox_MountAsync_Hook);
    HookFunction(PORT_XBOXCONTENT_CONSTRUCTOR, &XboxContentConstruct, &XboxContentConstructHook);
}

#endif // RB3E_XBOX
