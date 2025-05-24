#ifndef _NODESORT_H
#define _NODESORT_H

#include "rb3/List.h"
#include "rb3/Object.h"
#include "rb3/SortNode.h"
#include "rb3/Vector.h"

// very clear naming here. this couldn't possibly have been confusing during development
typedef struct _NodeSort
{
    void *mListProvider;
    Object mObject;
    vector mTree;
    list mFunctions;
    vector mList;
    Symbol mSortName;
} NodeSort;

SortNode *NodeSortGetNode(int *thisNodeSort, int idx);

#endif // _NODESORT_H