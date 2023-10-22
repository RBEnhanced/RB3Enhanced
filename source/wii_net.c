/*
    RB3Enhanced - wii_net.c
    Wii-specific networking routines.
*/

#ifdef RB3E_WII

#include <rvl/so.h>
#include <errno.h>
#include <fcntl.h>
#include "net.h"

int RB3E_CreateSocket(int protocol)
{
    int type;
    int sock_protocol;
    so_fd_t sock;
    if (protocol == RB3E_TYPE_TCP)
    { // TCP
        type = SOCK_STREAM;
        sock_protocol = PROTO_STREAM_UDP;
    }
    else if (protocol == RB3E_TYPE_UDP)
    { // UDP
        type = SOCK_DGRAM;
        sock_protocol = PROTO_DGRAM_TCP;
    }
    else
    {
        return -1;
    }
    // create the socket
    sock = SOSocket(AF_INET, type, sock_protocol);
    if (sock < 0)
        return -1;
    // TODO: allow broadcasting?
    return (int)sock;
}

void RB3E_DisposeSocket(int socket)
{
    SOClose((so_fd_t)socket);
}

int RB3E_LastError()
{
    // TODO: convert the last WSA error into generic type
    return errno;
}

unsigned int RB3E_GetInternalIP()
{
    return SOGetHostID();
}

unsigned int RB3E_GetGatewayIP()
{
    // TODO(Emma): THIS IS A HACK LOL - replaces last octet of IP with .1
    // should try to use NCDGetCurrentIpConfig(ncd_ip_config *config);
    // but that involves finding a bslug symbol for NCDGetCurrentIpConfig and documenting ncd_ip_config
    unsigned int int_ip = RB3E_GetInternalIP();
    int_ip &= 0xFFFFFF00;
    int_ip |= 0x00000001;
    return int_ip;
}

int RB3E_BindPort(int socket, unsigned short port)
{
    so_fd_t sock = (so_fd_t)socket;
    so_addr_t local_addr = {0};
    local_addr.sa_len = sizeof(so_addr_t);
    local_addr.sa_family = AF_INET;
    local_addr.sa_port = port;
    return SOBind(sock, &local_addr);
}

int RB3E_SetNonBlocking(int socket, int enable)
{
    so_fd_t sock = (so_fd_t)socket;
    int fcntl = SOFcntl(sock, F_GETFL, 0);
    // WiiNetworkSocket::__ct uses 4 instead of what bslug describes as O_NONBLOCK
    if (enable)
        fcntl |= 4; // O_NONBLOCK;
    else
        fcntl &= ~(4); //~O_NONBLOCK;
    return SOFcntl(sock, F_SETFL, fcntl);
}

int RB3E_SetRecvTimeout(int socket, int timeout_ms)
{
    // TODO(Emma): can SO do this?
    return -1;
}

int RB3E_SetSendTimeout(int socket, int timeout_ms)
{
    // TODO(Emma): can SO do this?
    return -1;
}

int RB3E_SetTimeout(int socket, int timeout_ms)
{
    // TODO(Emma): can SO do this?
    return -1;
}

int RB3E_UDP_SendTo(int socket, unsigned int ipv4, unsigned short port, void *data, int size)
{
    so_fd_t sock = (so_fd_t)socket;
    so_addr_t target_addr = {0};
    target_addr.sa_len = sizeof(so_addr_t);
    target_addr.sa_family = AF_INET;
    target_addr.sa_port = port;
    target_addr.sa_addr = ipv4;
    return SOSendTo(sock, data, size, 0, &target_addr);
}

int RB3E_UDP_RecvFrom(int socket, unsigned int *ipv4, unsigned short *port, void *data, int size)
{
    so_fd_t sock = (so_fd_t)socket;
    so_addr_t target_addr = {0};
    int r = 0;
    target_addr.sa_len = sizeof(so_addr_t);
    target_addr.sa_family = AF_INET;
    if (port != NULL)
        target_addr.sa_port = *port;
    if (ipv4 != NULL)
        target_addr.sa_addr = *ipv4;
    r = SORecvFrom(sock, data, size, 0, &target_addr);
    if (r >= 0)
    {
        if (port != NULL)
            *port = target_addr.sa_port;
        if (ipv4 != NULL)
            *ipv4 = target_addr.sa_addr;
    }
    return r;
}

int RB3E_TCP_Connect(int socket, unsigned int ipv4, unsigned short port)
{
    so_fd_t sock = (so_fd_t)socket;
    so_addr_t target_addr = {0};
    target_addr.sa_len = sizeof(so_addr_t);
    target_addr.sa_family = AF_INET;
    target_addr.sa_port = port;
    target_addr.sa_addr = ipv4;
    return SOConnect(sock, &target_addr);
}

int RB3E_TCP_Send(int socket, void *data, int size)
{
    return SOSend((so_fd_t)socket, data, size, 0);
}

int RB3E_TCP_Recv(int socket, void *data, int size)
{
    return SORecv((so_fd_t)socket, data, size, 0);
}

#endif
