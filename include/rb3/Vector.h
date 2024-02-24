#ifndef _VECTOR_H
#define _VECTOR_H

// basic vector type
// used constantly across the code
// in C++ it would be used like vector<GameGem> or etc.
typedef struct _vector
{
    // structure of a vector differs on 360 vs Wii
#ifdef RB3E_XBOX
    void *begin;
    void *end;
    void *capacity;
#else
    void *data;
    short count;
    short capacity;
#endif
} vector;

void vector_push_back(vector *v, void *item);

#endif // _VECTOR_H