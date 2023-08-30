#ifndef _DATA_H
#define _DATA_H

#include "Symbol.h"

typedef union _DataNode_Value
{
    int intVal;
    float floatVal;
    int *dataArray;
    int *object;
    char *string;
} DataNode_Value;

typedef enum _DataNode_Type
{
    INT_VALUE = 0,
    FLOAT_VALUE = 1,
    VAR = 2,
    FUNC = 3,
    OBJECT = 4,
    SYMBOL = 5,
    EMPTY = 6,
    IFDEF = 7,
    ELSE = 8,
    ENDIF = 9,
    ARRAY = 16,
    COMMAND = 17,
    STRING_VALUE = 18,
    OBJECT_PROP_REF = 19,
    GLOB = 20,
    DEFINE = 32,
    INCLUDE = 33,
    MERGE = 34,
    IFNDEF = 35,
    AUTORUN = 36,
    UNDEF = 37
} DataNode_Type;

typedef struct _DataNode
{
    DataNode_Value value;
    DataNode_Type type;
} DataNode;

typedef struct _DataNodes
{
    DataNode n[1];
} DataNodes;

typedef struct _DataArray
{
    DataNodes *mNodes;
    Symbol mFile; /* Sometimes this is a Symbol, sometimes a type */
    short mNodeCount;
    short mRefCount;
    short mLine;
    short mUnknown;
} DataArray;

// gets a pointer to the addr of a DTA function
// if there is no function in the list with the symbol's name
// return a pointer to a new entry that you overwrite with your own function ptr
extern int *HmxFactoryFuncAt(int *gDataFuncs, Symbol *sym);

extern DataNode *DataNodeEvaluate(DataNode *ret);

extern DataArray *DataReadFile(char *file, int dtb);

extern DataArray *DataFindArray(DataArray *data, Symbol name);
extern int DataFindData(DataArray *data, Symbol name, DataNode *out);

#endif // _DATA_H
