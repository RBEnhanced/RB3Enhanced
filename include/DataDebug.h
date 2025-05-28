/*
    RB3Enhanced - DataDebug.h
    Hooks for debugging scripting errors, similar to debug.
*/

#include "rb3/Data.h"

DataNode *DataSetHook(DataNode *ret, DataArray *array);
DataNode *DataSetElemHook(DataNode *ret, DataArray *array);
DataNode *DataOnElemHook(DataNode *ret, DataArray *array);
void *DataNodeGetObjHook(DataNode *node);
