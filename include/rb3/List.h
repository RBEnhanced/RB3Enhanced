#ifndef _LIST_H
#define _LIST_H

// basic list type
// used constantly across the code
// in C++ it would be used like list<GameGem> or etc.
typedef struct _list
{
    void *next;
    void *prev;
} list;

#endif // _LIST_H