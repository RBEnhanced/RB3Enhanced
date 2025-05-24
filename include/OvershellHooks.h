/*
    RB3Enhanced - OvershellHooks.h
    Hooks to the "overshell" and related functionality.
*/

#include "rb3/BandUser.h"

typedef struct _OvershellSlot
{
#ifdef RB3E_XBOX
    char unk1[0x2c];
#elif RB3E_WII
    char unk1[0x20];
#endif
    ControllerType controllerType;
    int list_vector_maybe;
    int unk_var_2;
#ifdef RB3E_XBOX
    char unk2[0x4];
#endif
    BandUser *bandUser;
} OvershellSlot;

typedef struct _OvershellListEntry
{
    char *sym;
    TrackType trackType;
    char icon;
} OvershellListEntry;

void OvershellPartSelectProviderReload(OvershellSlot *thisOvershellSlot, ControllerType controllerType, BandUser *bandUser);

// Not overshell related but this is the only place we use them and IDK how to correctly organise code
void PrepareSomeVectorMaybe(int *r3, int r4, int r5);
void SomeVectorPushBackMaybe(int *r3, OvershellListEntry *r4); // vector<class_Key<class_Vector2>,class_stlpmtx_std::StlNodeAlloc<class_Key<class_Vector2>_>_>::push_back ??
