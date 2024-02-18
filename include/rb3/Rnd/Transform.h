#ifndef _XFM_H
#define _XFM_H

typedef struct _Vector3
{
    float x;
    float y;
    float z;
    float w; // not sure what this is? its not defined in RB3 symbols but is definitely in the structure
} Vector3;

typedef struct _Matrix3
{
    Vector3 x;
    Vector3 y;
    Vector3 z;
} Matrix3;

typedef struct _Transform
{
    Matrix3 m;
    Vector3 v;
} Transform;
#endif // _XFM_H