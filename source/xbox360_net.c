/*
    RB3Enhanced - xbox360_net.c
    Xbox 360-specific networking routines.
*/

#ifdef RB3E_XBOX

#include <xtl.h>
#include "net.h"

BOOL opt_true = TRUE;
int RB3E_CreateSocket(int protocol)
{
    int type;
    int sock_protocol;
    SOCKET sock;
    if (protocol == RB3E_TYPE_TCP)
    { // TCP
        type = SOCK_STREAM;
        sock_protocol = IPPROTO_TCP;
    }
    else if (protocol == RB3E_TYPE_UDP)
    { // UDP
        type = SOCK_DGRAM;
        sock_protocol = IPPROTO_UDP;
    }
    else
    {
        return -1;
    }
    // create the socket
    sock = socket(AF_INET, type, sock_protocol);
    if (sock == INVALID_SOCKET)
        return -1;
    // mark the socket as insecure
    setsockopt(sock, SOL_SOCKET, 0x5801, (PCSTR)&opt_true, sizeof(BOOL));
    // if (protocol == RB3E_TYPE_TCP) // tcp sockets need this as well - or do they...?
    //     setsockopt(sock, SOL_SOCKET, 0x5802, (PCSTR)&opt_true, sizeof(BOOL));
    if (protocol == RB3E_TYPE_UDP) // allow broadcasting
        setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (PCSTR)&opt_true, sizeof(BOOL));
    return (int)sock;
}

void RB3E_DisposeSocket(int socket)
{
    shutdown((SOCKET)socket, SD_SEND);
    closesocket((SOCKET)socket);
}

int RB3E_LastError()
{
    // TODO: convert the last WSA error into generic type
    return WSAGetLastError();
}

int RB3E_BindPort(int socket, unsigned short port)
{
    SOCKET sock = (SOCKET)socket;
    struct sockaddr_in local_addr = {0};
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = port;
    return bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr));
}

int RB3E_SetNonBlocking(int socket, int enable)
{
    return ioctlsocket((SOCKET)socket, FIONBIO, (unsigned long *)&enable);
}

int RB3E_SetRecvTimeout(int socket, int timeout_ms)
{
    return setsockopt((SOCKET)socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout_ms, sizeof(timeout_ms));
}

int RB3E_SetSendTimeout(int socket, int timeout_ms)
{
    return setsockopt((SOCKET)socket, SOL_SOCKET, SO_SNDTIMEO, (const char *)&timeout_ms, sizeof(timeout_ms));
}

int RB3E_SetTimeout(int socket, int timeout_ms)
{
    int r = RB3E_SetRecvTimeout(socket, timeout_ms);
    r = RB3E_SetRecvTimeout(socket, timeout_ms);
    return r;
}

int RB3E_UDP_SendTo(int socket, unsigned int ipv4, unsigned short port, void *data, int size)
{
    SOCKET sock = (SOCKET)socket;
    struct sockaddr_in target_addr = {0};
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = port;
    target_addr.sin_addr.S_un.S_addr = ipv4;
    return sendto(sock, data, size, 0, (struct sockaddr *)&target_addr, sizeof(target_addr));
}

int RB3E_UDP_RecvFrom(int socket, unsigned int *ipv4, unsigned short *port, void *data, int size)
{
    SOCKET sock = (SOCKET)socket;
    struct sockaddr_in target_addr = {0};
    int r = 0;
    int fromlen = sizeof(target_addr);
    target_addr.sin_family = AF_INET;
    if (port != NULL)
        target_addr.sin_port = *port;
    if (ipv4 != NULL)
        target_addr.sin_addr.S_un.S_addr = *ipv4;
    r = recvfrom(sock, data, size, 0, (struct sockaddr *)&target_addr, &fromlen);
    if (r >= 0)
    {
        if (port != NULL)
            *port = target_addr.sin_port;
        if (ipv4 != NULL)
            *ipv4 = target_addr.sin_addr.S_un.S_addr;
    }
    return r;
}

int RB3E_TCP_Connect(int socket, unsigned int ipv4, unsigned short port)
{
    SOCKET sock = (SOCKET)socket;
    struct sockaddr_in target_addr = {0};
    target_addr.sin_family = AF_INET;
    target_addr.sin_port = port;
    target_addr.sin_addr.S_un.S_addr = ipv4;
    return connect(sock, (struct sockaddr *)&target_addr, sizeof(target_addr));
}

int RB3E_TCP_Send(int socket, void *data, int size)
{
    return send((SOCKET)socket, data, size, 0);
}

int RB3E_TCP_Recv(int socket, void *data, int size)
{
    return recv((SOCKET)socket, data, size, 0);
}

int RB3E_TCP_Listen(int socket)
{
    return listen((SOCKET)socket, SOMAXCONN);
}

int RB3E_TCP_Accept(int socket, unsigned int *ipv4, unsigned short *port)
{
    SOCKET sock = (SOCKET)socket;
    struct sockaddr_in connection_addr = {0};
    int fromlen = sizeof(connection_addr);
    SOCKET r = accept(sock, (struct sockaddr *)&connection_addr, &fromlen);
    if (r == INVALID_SOCKET)
        return -1;
    if (port != NULL)
        *port = connection_addr.sin_port;
    if (ipv4 != NULL)
        *ipv4 = connection_addr.sin_addr.S_un.S_addr;
    return (int)r;
}

#endif
