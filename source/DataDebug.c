/*
    RB3Enhanced - DataDebug.h
    Hooks for debugging scripting errors, similar to debug.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include "rb3/Data.h"
#include "ports.h"

DataNode *DataSetHook(DataNode *ret, DataArray *array)
{
    if (array != NULL)
    {
        if (array->mNodes->n[1].type != VAR && array->mNodes->n[1].type != OBJECT_PROP_REF)
        {
            if (array->mFile.sym != NULL)
            {
                RB3E_DEBUG("Invalid set called of type %i in %s (line %d)", array->mNodes->n[1].type, array->mFile.sym, array->mLine);
            }
            else
            {
                RB3E_DEBUG("Invalid set called of type %i", array->mNodes->n[1].type);
            }
            ret->type = INT_VALUE;
            ret->value.intVal = 0;
            return ret;
        }
    }
    return DataSet(ret, array);
}

DataNode *DataSetElemHook(DataNode *ret, DataArray *array)
{
    if (array != NULL)
    {
        DataArray *setArray = array->mNodes->n[1].value.dataArray;
        DataNode *evaluated = DataNodeEvaluate(&array->mNodes->n[1]);
        if (evaluated->type == ARRAY)
        {
            setArray = evaluated->value.dataArray;
        }
        else
        {
            if (array->mFile.sym != NULL)
            {
                RB3E_DEBUG("Tried to set_elem with non-array in %s  (line %d)", array->mFile.sym, array->mLine);
            }
            else
            {
                RB3E_DEBUG("Tried to set_elem with non-array", NULL);
            }
            ret->type = INT_VALUE;
            ret->value.intVal = 0;
            return ret;
        }
        if (setArray != NULL)
        {
            DataNode *idxNode = DataNodeEvaluate(&array->mNodes->n[2]);
            if (idxNode != NULL)
            {
                int idx = idxNode->value.intVal;
                if (idx < 0 || idx > setArray->mNodeCount)
                {
                    if (array->mFile.sym != NULL)
                    {
                        RB3E_DEBUG("Tried to set_elem with an invalid index %d in %s  (line %d)", idx, array->mFile.sym, array->mLine);
                    }
                    else
                    {
                        RB3E_DEBUG("Tried to set_elem with an invalid index", NULL);
                    }
                    ret->type = INT_VALUE;
                    ret->value.intVal = 0;
                    return ret;
                }
            }
            else
            {
                if (array->mFile.sym != NULL)
                {
                    RB3E_DEBUG("Tried to set_elem with a null index in %s  (line %d)", array->mFile.sym, array->mLine);
                }
                else
                {
                    RB3E_DEBUG("Tried to set_elem with a null index", NULL);
                }
                ret->type = INT_VALUE;
                ret->value.intVal = 0;
                return ret;
            }
        }
        else
        {
            if (array->mFile.sym != NULL)
            {
                RB3E_DEBUG("Tried to set_elem with a null array in %s  (line %d)", array->mFile.sym, array->mLine);
            }
            else
            {
                RB3E_DEBUG("Tried to set_elem with a null array", NULL);
            }
            ret->type = INT_VALUE;
            ret->value.intVal = 0;
            return ret;
        }
    }
    return DataSetElem(ret, array);
}

DataNode *DataOnElemHook(DataNode *ret, DataArray *array)
{
    if (array != NULL)
    {
        DataArray *elemArray = array->mNodes->n[1].value.dataArray;
        DataNode *evaluated = DataNodeEvaluate(&array->mNodes->n[1]);
        if (evaluated->type == ARRAY)
        {
            elemArray = evaluated->value.dataArray;
        }
        else
        {
            if (array->mFile.sym != NULL)
            {
                RB3E_DEBUG("Tried to elem with non-array in %s  (line %d)", array->mFile.sym, array->mLine);
            }
            else
            {
                RB3E_DEBUG("Tried to elem with non-array", NULL);
            }
            ret->type = INT_VALUE;
            ret->value.intVal = 0;
            return ret;
        }
        if (elemArray != NULL)
        {
            DataNode *idxNode = DataNodeEvaluate(&array->mNodes->n[2]);
            if (idxNode != NULL)
            {
                int idx = idxNode->value.intVal;
                if (idx < 0 || idx >= elemArray->mNodeCount)
                {
                    if (array->mFile.sym != NULL)
                    {
                        RB3E_DEBUG("Tried to elem with an invalid index %d in %s  (line %d)", idx, array->mFile.sym, array->mLine);
                    }
                    else
                    {
                        RB3E_DEBUG("Tried to elem with an invalid index %d", idx);
                    }
                    ret->type = INT_VALUE;
                    ret->value.intVal = 0;
                    return ret;
                }
            }
            else
            {
                if (array->mFile.sym != NULL)
                {
                    RB3E_DEBUG("Tried to elem with a null index in %s  (line %d)", array->mFile.sym, array->mLine);
                }
                else
                {
                    RB3E_DEBUG("Tried to elem with a null index", NULL);
                }
                ret->type = INT_VALUE;
                ret->value.intVal = 0;
                return ret;
            }
        }
        else
        {
            if (array->mFile.sym != NULL)
            {
                RB3E_DEBUG("Tried to elem with a null array in %s  (line %d)", array->mFile.sym, array->mLine);
            }
            else
            {
                RB3E_DEBUG("Tried to elem with a null array", NULL);
            }
            ret->type = INT_VALUE;
            ret->value.intVal = 0;
            return ret;
        }
    }
    return DataOnElem(ret, array);
}

void *DataNodeGetObjHook(DataNode *node)
{
    DataNode *eval = DataNodeEvaluate(node);
    if (eval->type != OBJECT && eval->type != STRING_VALUE && eval->type != SYMBOL)
    {
        RB3E_DEBUG("Tried to get object of non-object node type %d", node->type);
        return NULL;
    }
    return DataNodeGetObj(node);
}
