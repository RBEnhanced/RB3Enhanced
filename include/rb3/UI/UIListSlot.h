#ifndef _UILISTSLOT_H
#define _UILISTSLOT_H

#include "rb3/Object.h"
#include "rb3/String.h"
#include "rb3/Vector.h"
#include "rb3/UI/UIColor.h"

typedef enum _UIListWidgetDrawType
{
    kUIListWidgetDrawAlways = 0,
    kUIListWidgetDrawOnlyFocused = 1,
    kUIListWidgetDrawNever = 2
} UIListWidgetDrawType;

typedef enum _UIListSlotDrawType
{
    kUIListSlotDrawAlways = 0,
    kUIListSlotDrawHighlight = 1,
    kUIListSlotDrawNoHighlight = 2
} UIListSlotDrawType;

typedef struct _UIListWidget
{
    Object mObject;
    int mUnk;
    float mDrawOrder;
    ObjPtr_UIColor mDefaultColor;
    vector mColors;
    UIListWidgetDrawType mWidgetDrawType;
    void *mParentList;
} UIListWidget;

typedef struct _UIListSlot
{
    UIListWidget mWidget;
    vector mElements;
    UIListSlotDrawType mSlotDrawType;
    int mNextElement;
    String mMatchName;
} UIListSlot;

#endif // _UILISTSLOT_H