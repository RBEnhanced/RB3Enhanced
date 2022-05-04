#ifndef _MODIFIERMANAGER_H
#define _MODIFIERMANAGER_H

typedef char bool;
#include "Symbol.h"

typedef struct _Modifier
{
    int *dataArray;
    bool enabled;
} Modifier;

extern Modifier *ModifierActive(int thisModifierManager, Symbol symbol, bool defaultValue);
extern void *ModifierManagerConstructor(int thisModifierManager, int unk);

#endif // _MODIFIERMANAGER_H