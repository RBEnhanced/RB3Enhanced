#ifndef _GEMTRACKRESOURCEMANAGER_H
#define _GEMTRACKRESOURCEMANAGER_H

#include "rb3/Object.h"

typedef enum _SmasherPlateType
{
    kPlateGuitar = 0,
    kPlateDrums = 1,
    kPlateBass = 2,
    // 3 seems to be unused
    kPlateKeys = 4,
    kPlateRealGuitar = 5,
    kPlateRealBass = 6,
    kPlateRealKeys = 7

} SmasherPlateType;

typedef struct _SmasherPlate
{
    void *vtable;
    void *mResourceManager;
    Object *mSmasherPlateObject; // not sure what this type of object this actually is so just use generic obj for now
    SmasherPlateType mSmasherType;
    char mUsed;
    char pad[3];
} SmasherPlate;

void AddSmasherPlateToVector(void *smasherPlates, SmasherPlate *plate);

#endif // _GEMTRACKRESOURCEMANAGER_H
