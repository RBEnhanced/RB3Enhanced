/*
    RB3Enhanced - net_stun.c
    Using the STUN protocol to get the external IP address of the user.
*/

// STUN is only needed for Xbox, so guard it
#ifdef RB3E_XBOX

#include <xtl.h>
#include <stdio.h>
#include "net.h"
#include "net_events.h"
#include "ports.h"
#include "quazal/InetAddress.h"
#include "rb3/Random.h"

// structs for data types - not worth a whole header file
PACKSTART
typedef struct _STUN_Header
{
    unsigned short type;
    unsigned short size;
    union
    {
        unsigned int cookie;
        char id[16];
    };
} STUN_Header;
typedef struct _STUN_AttrHeader
{
    unsigned short type;
    unsigned short length;
} STUN_AttrHeader;
typedef struct _STUN_AttrIPv4
{
    char unused;
    char family;
    unsigned short port;
    union
    {
        unsigned char ip_bytes;
        unsigned int ip;
    };
} STUN_AttrIPv4;
PACKEND

// TODO(Emma): move this into game loop or another thread to run in the background

// parses the MAPPED-ADDRESS/XOR-MAPPED-ADDRESS from the response
static int STUN_ParseIPFromResponse(char *response, int response_length)
{
    STUN_Header *header = (STUN_Header *)response;
    STUN_AttrHeader *attribute = (STUN_AttrHeader *)(response + sizeof(STUN_Header));
    STUN_AttrIPv4 *ipv4 = NULL;
    if (header->type != 0x0101) // BINDING-RESPONSE
    {
        RB3E_DEBUG("Response was not of BINDING-RESPONSE!");
        return -1;
    }
    if (header->size > response_length - sizeof(STUN_Header))
    {
        RB3E_DEBUG("Mismatched STUN header size! Header: %i, packet: %i", header->size, response_length);
        return -1;
    }
    // iterate through every attribute until we find MAPPED-ADDRESS
    while (ipv4 == NULL)
    {
        switch (attribute->type)
        {
        case 0x0001: // MAPPED-ADDRESS
            ipv4 = (STUN_AttrIPv4 *)((char *)attribute + sizeof(STUN_AttrHeader));
            RB3E_DEBUG("Got MAPPED-ADDRESS attribute!", NULL);
            break;
        case 0x0020: // XOR-MAPPED-ADDRESS
            ipv4 = (STUN_AttrIPv4 *)((char *)attribute + sizeof(STUN_AttrHeader));
            RB3E_DEBUG("Got XOR-MAPPED-ADDRESS attribute!", NULL);
            ipv4->ip ^= header->cookie;
            break;
        default:
            RB3E_DEBUG("Unknown attribute %04x, length %04x", attribute->type, attribute->length);
            break;
        }
        attribute = (STUN_AttrHeader *)((char *)attribute + sizeof(STUN_AttrHeader) + attribute->length);
        // very hacky check to see if we've surpassed the header size
        if (((char *)attribute - (char *)header) - sizeof(STUN_Header) >= header->size)
            break;
    }
    if (ipv4 == NULL)
    {
        RB3E_DEBUG("Failed to get IP address");
        return -1; // valid as no IP is 0xFFFFFFFF
    }
    return ipv4->ip;
}

// gets the external IP address - returns 0 on success, -1 on failure
int STUN_GetExternalIP(char *stunServerIP, short stunPort, char *returnedIP, int socket_type)
{
    unsigned char stunRequest[20];
    unsigned char recvBuffer[1000];
    int socket;
    int r, i;
    WSADATA wsaData;
    InetAddress inetAddr;
    STUN_Header *request = (STUN_Header *)stunRequest;

    // set up the request header
    request->type = 0x0001; // binding request
    request->size = 0;
    for (i = 4; i < 16; i++)
    {
        request->id[i] = RandomInt(0, 0xFF);
    }
    request->cookie = 0x2112A442;

    RB3E_DEBUG("Attempting to get external IP address via STUN...", NULL);

    // start up winsock, we might be a bit too early for the game to have done it
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // resolve the STUN server's IP/domain name (Quazal will do DNS for us)
    memset(&inetAddr, 0, sizeof(InetAddress));
    SetAddress(&inetAddr, stunServerIP);

    RB3E_DEBUG("Creating STUN socket...", NULL);
    socket = RB3E_CreateSocket(socket_type);
    if (socket == INVALID_SOCKET)
        return -1;
    RB3E_SetRecvTimeout(socket, 4000);

    RB3E_DEBUG("Sending STUN request...", NULL);
    RB3E_UDP_SendTo(socket, inetAddr.address, stunPort, stunRequest, sizeof(stunRequest));

    RB3E_DEBUG("Receiving STUN response...", NULL);
    r = RB3E_UDP_RecvFrom(socket, NULL, NULL, recvBuffer, sizeof(recvBuffer));
    if (r == SOCKET_ERROR)
        return -1;

    RB3E_DEBUG("Parsing STUN response...", NULL);
    r = STUN_ParseIPFromResponse(recvBuffer, r);
    if (r != -1)
    {
        sprintf(returnedIP, "%d.%d.%d.%d", (r >> 24) & 0xFF, (r >> 16) & 0xFF, (r >> 8) & 0xFF, r & 0xFF);
        r = 0;
    }

    RB3E_DEBUG("Closing STUN socket...", NULL);
    RB3E_DisposeSocket(socket);

    return r;
}

#endif
