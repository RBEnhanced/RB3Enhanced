#include "rb3/PropSync.h"

#ifdef RB3E_WII

// TODO: find a simpler way to do this, if possible, preferably without needing a hook
int PropSyncHook(char *value, DataNode *unk, DataArray *dataArray, int unk3, int propOp)
{
    if (dataArray->mNodeCount != 0 && dataArray->mNodes->n[0].type == SYMBOL)
    {
        if (strcmp(dataArray->mNodes->n[0].value.string, "multi_event") == 0)
        {
            *value = 1;
        }
    }

    return PropSyncBool(value, unk, dataArray, unk3, propOp);
}
#endif