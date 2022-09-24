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
    FLOAT_VALUE = 1,
    ARRAY = 16,
    IFNDEF = 35,
    MERGE = 34,
    FUNC = 3,
    VAR = 2,
    ENDIF = 9,
    OBJECT_PROP_REF = 19,
    OBJECT = 4,
    IFDEF = 7,
    DEFINE = 32,
    INT_VALUE = 0,
    SYMBOL = 5,
    ELSE = 8,
    STRING_VALUE = 18,
    GLOB = 20,
    AUTORUN = 36,
    COMMAND = 17,
    INCLUDE = 33,
    EMPTY = 6,
    UNDEF = 37
} DataNode_Type;

typedef struct _DataNode
{
    DataNode_Value value;
    DataNode_Type type;
} DataNode;

typedef struct _DataArray
{
    DataNode *mNodes[1];
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

#endif // _DATA_H