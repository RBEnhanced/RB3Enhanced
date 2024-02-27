/*
    RB3Enhanced - ps3_net.c
    PS3-specific networking routines.
*/

#ifdef RB3E_PS3

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netex/errno.h>
#include <netex/net.h>
#include "net.h"
#include "ports.h"

int opt_true = 1;
int RB3E_CreateSocket(int protocol)
{
    int type;
    int sock_protocol;
    int sock;
    if (protocol == RB3E_TYPE_TCP)
        type = SOCK_STREAM;
    else if (protocol == RB3E_TYPE_UDP)
        type = SOCK_DGRAM;
    else
        return -1;
    // create the socket
    sock = socket(AF_INET, type, 0);
    if (sock < 0)
        return -1;
    if (protocol == RB3E_TYPE_UDP) // allow broadcasting
        setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&opt_true, sizeof(int));
    return (int)sock;
}

void RB3E_DisposeSocket(int socket)
{
    socketclose(socket);
}

int RB3E_LastError()
{
    // TODO: convert the last WSA error into generic type
    return sys_net_errno;
}

unsigned int RB3E_GetInternalIP()
{
    return 0;
}

unsigned int RB3E_GetGatewayIP()
{
    return 0;
}

int RB3E_BindPort(int socket, unsigned short port)
{
    struct sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = port;
    return bind(socket, (struct sockaddr *)&local_addr, sizeof(local_addr));
}

int RB3E_SetNonBlocking(int socket, int enable)
{
    return setsockopt(socket, SOL_SOCKET, SO_NBIO, (const char *)&enable, sizeof(enable));
}

int RB3E_SetRecvTimeout(int socket, int timeout_ms)
{
    return setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout_ms, sizeof(timeout_ms));
}

int RB3E_SetSendTimeout(int socket, int timeout_ms)
{
    return setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout_ms, sizeof(timeout_ms));
}

int RB3E_SetTimeout(int socket, int timeout_ms)
{
    int r = RB3E_SetRecvTimeout(socket, timeout_ms);
    r = RB3E_SetRecvTimeout(socket, timeout_ms);
    return r;
}

int RB3E_UDP_SendTo(int socket, unsigned int ipv4, unsigned short port, void *data, int size)
{
    struct sockaddr_in target_addr = {0};
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = port;
    target_addr.sin_addr.s_addr = ipv4;
    return sendto(socket, data, size, 0, (struct sockaddr *)&target_addr, sizeof(target_addr));
}

int RB3E_UDP_RecvFrom(int socket, unsigned int *ipv4, unsigned short *port, void *data, int size)
{
    struct sockaddr_in target_addr = {0};
    int r = 0;
    int fromlen = sizeof(target_addr);
    target_addr.sin_family = AF_INET;
    if (port != NULL)
        target_addr.sin_port = *port;
    if (ipv4 != NULL)
        target_addr.sin_addr.s_addr = *ipv4;
    r = recvfrom(socket, data, size, 0, (struct sockaddr *)&target_addr, &fromlen);
    if (r >= 0)
    {
        if (port != NULL)
            *port = target_addr.sin_port;
        if (ipv4 != NULL)
            *ipv4 = target_addr.sin_addr.s_addr;
    }
    return r;
}

int RB3E_TCP_Connect(int socket, unsigned int ipv4, unsigned short port)
{
    struct sockaddr_in target_addr = {0};
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = port;
    target_addr.sin_addr.s_addr = ipv4;
    return connect(socket, (struct sockaddr *)&target_addr, sizeof(target_addr));
}

int RB3E_TCP_Send(int socket, void *data, int size)
{
    return send(socket, data, size, 0);
}

int RB3E_TCP_Recv(int socket, void *data, int size)
{
    return recv(socket, data, size, 0);
}

int RB3E_TCP_Listen(int socket)
{
    return listen(socket, 5);
}

int RB3E_TCP_Accept(int socket, unsigned int *ipv4, unsigned short *port)
{
    struct sockaddr_in connection_addr = {0};
    int fromlen = sizeof(connection_addr);
    int r = accept(socket, (struct sockaddr *)&connection_addr, &fromlen);
    if (r < 0)
        return -1;
    if (port != NULL)
        *port = connection_addr.sin_port;
    if (ipv4 != NULL)
        *ipv4 = connection_addr.sin_addr.s_addr;
    return (int)r;
}

#endif
