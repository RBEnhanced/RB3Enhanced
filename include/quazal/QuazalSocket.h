#ifndef _QUAZALSOCKET_H
#define _QUAZALSOCKET_H

#ifdef RB3E_XBOX
#include <xtl.h>
#elif RB3E_WII
#include <rvl/so.h>
#include "../rb3/WiiNetworkSocket.h"
#endif
#include "quazal/InetAddress.h"

// TODO(Emma): Make this work for Wii
typedef struct _QuazalSocket
{
    int unknown;
    InetAddress *inetAddress;
    char padding[0x88];
#ifdef RB3E_XBOX
    SOCKET native_socket;
    char isVDP;
    char unk[0x3];
    int *network_socket; // maybe?
#elif RB3E_WII
    int *unk;
    WiiNetworkSocket *network_socket;
#endif
} QuazalSocket;

extern int QuazalSocketBind(QuazalSocket *socket, InetAddress *address);
// takes a Quazal::QueuingSocket, but that inherits from Quazal::Socket
extern char QueuingSocketBind(QuazalSocket *socket, InetAddress *address, unsigned short *bind_port);

#endif // _QUAZALSOCKET_H
