
#ifndef _XBOXCONTENT_H
#define _XBOXCONTENT_H
#ifdef RB3E_XBOX

#include <xtl.h>
#include "rb3/String.h"
#include "rb3/Symbol.h"

typedef enum _ContentState
{
    kUnmounted = 0,
    kNeedsMounting = 1,
    kMounting = 2,
    kUnmounting = 3,
    kMounted = 4,
    kDeleting = 6,
    kDeleted = 7,
    kFailed = 8,
} ContentState;

typedef struct _XboxContent
{
    void *vtable;
    XOVERLAPPED *mOverlapped;
    XCONTENT_CROSS_TITLE_DATA mTitleData;
    int mLicenseBits;
    char mLicenseBitsValid; // bool
    String mRoot;
    String mMountPath;
    int mState;
    int mPadNum;
    char mDeleted; // bool
    Symbol mFileName;
    int mLRM; // what?
} XboxContent;

XboxContent *XboxContentConstruct(XboxContent *thisXboxContent, XCONTENT_CROSS_TITLE_DATA *titleData, int contentIndex, int padNum, char needsMount);

#endif // RB3E_XBOX
#endif // _XBOXCONTENT_H
