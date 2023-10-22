#ifndef _WIINETWORKSOCKET_H
#define _WIINETWORKSOCKET_H
#ifdef RB3E_WII

#include <rvl/so.h>

typedef int (*NetworkSocketBind_t)(void *networkSocket, unsigned short port);

// todo: the rest
typedef struct _NetworkSocket_vtable
{
    void *destructor;
    void *connect;
    void *fail;
    void *disconnect;
    NetworkSocketBind_t bind;
} NetworkSocket_vtable;

typedef struct _WiiNetworkSocket
{
    NetworkSocket_vtable *vtable;
    so_fd_t native_socket;
    int unk_maybe_bools;
} WiiNetworkSocket;

#endif // RB3E_WII
#endif // _WIINETWORKSOCKET_H
