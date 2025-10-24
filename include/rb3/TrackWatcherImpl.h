#ifndef _TRACKWATCHERIMPL_H
#define _TRACKWATCHERIMPL_H

typedef struct _TrackWatcherImpl
{
    void *vtable;         // 0x000
    char mUserGuid[0x10]; // 0x004
    char mIsLocalUser;    // 0x014
    char pad[0x3];        // 0x015 
    int mPlayerSlot;      // 0x018
    void *mGameGemList;   // 0x01C
    
    // TODO: the rest
} TrackWatcherImpl;

#endif // _TRACKWATCHERIMPL_H
