#ifndef _MODIFIERMANAGER_H
#define _MODIFIERMANAGER_H

typedef char bool;
#include "Data.h"
#include "Symbol.h"

typedef struct _Modifier
{
    DataArray *dataArray;
    bool enabled;
} Modifier;

extern Modifier *ModifierActive(int thisModifierManager, Symbol symbol, bool defaultValue);
extern void *ModifierManagerConstructor(int thisModifierManager, int unk);

#endif // _MODIFIERMANAGER_H