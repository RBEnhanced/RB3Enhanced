#ifdef RB3E_XBOX

#include <xtl.h>
#include <stdio.h>
#include "STUN.h"
#include "ports.h"

// gets the external IP address of the Xbox 360 console via STUN
// returns -1 on failure, 0 on success
int getExternalIP(char *stunServerIP, short stunPort, char *returnedIP)
{
    unsigned char stunRequest[20] = {0x00, 0x01, 0x00, 0x00, 0x52, 0x42, 0x33, 0x45, 0x53, 0x54, 0x55, 0x4E, 0x43, 0x4C, 0x49, 0x45, 0x4E, 0x54, 0x21, 0x21};
    unsigned char recvBuffer[1000];

    // recv timeout
    int timeout = 4000;

    // the size of the BINDING-RESPONSE header
    int skip = 20;

    // status codes for winsock
    int wsaErr, optErr, bindErr, sendErr, recvErr, closeErr;

    SOCKET stunSocket;

    struct sockaddr_in stunAddr, localAddr;

    short attribType, attribLength;

    BOOL optEnable = TRUE;

    WSADATA wsaData;

    RB3E_DEBUG("Attempting to get external IP address via STUN...", NULL);

    wsaErr = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (wsaErr != 0)
    {
        RB3E_DEBUG("WSAStartup failed with error: %d", wsaErr);
        return -1;
    }

    RB3E_DEBUG("Creating STUN socket...", NULL);
    stunSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (stunSocket == INVALID_SOCKET)
    {
        RB3E_DEBUG("socket failed with error: %d", WSAGetLastError());
        return -1;
    }

    RB3E_DEBUG("Setting sockopts...", NULL);

    // set insecure & recv timeout
    optErr = setsockopt(stunSocket, SOL_SOCKET, 0x5801, (PCSTR)&optEnable, sizeof(BOOL));

    if (optErr == SOCKET_ERROR)
    {
        RB3E_DEBUG("setsockopt insecure failed with error: %d", WSAGetLastError());
        return -1;
    }

    optErr = setsockopt(stunSocket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(timeout));

    if (optErr == SOCKET_ERROR)
    {
        RB3E_DEBUG("setsockopt timeout failed with error: %d", WSAGetLastError());
        return -1;
    }

    memset(&stunAddr, 0, sizeof(stunAddr));
    memset(&localAddr, 0, sizeof(localAddr));

    stunAddr.sin_family = AF_INET;
    stunAddr.sin_addr.s_addr = inet_addr(stunServerIP);
    stunAddr.sin_port = htons(stunPort);

    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(1337);

    RB3E_DEBUG("Binding STUN socket...", NULL);

    bindErr = bind(stunSocket, (struct sockaddr *)&localAddr, sizeof(localAddr));
    if (bindErr == SOCKET_ERROR)
    {
        RB3E_DEBUG("bind failed with error: %d", WSAGetLastError());
        return -1;
    }

    RB3E_DEBUG("Sending STUN request...", NULL);

    sendErr = sendto(stunSocket, stunRequest, sizeof(stunRequest), 0, (struct sockaddr *)&stunAddr, sizeof(stunAddr));
    if (sendErr == SOCKET_ERROR)
    {
        RB3E_DEBUG("sendto failed with error: %d", WSAGetLastError());
        return -1;
    }

    RB3E_DEBUG("Receiving STUN response...", NULL);

    recvErr = recvfrom(stunSocket, recvBuffer, sizeof(recvBuffer), 0, NULL, 0);
    if (recvErr == SOCKET_ERROR)
    {
        RB3E_DEBUG("recvfrom failed with error: %d", WSAGetLastError());
        return -1;
    }

    RB3E_DEBUG("Parsing STUN response...", NULL);

    // check for the BINDING_RESPONSE message type
    if (*(short *)(&recvBuffer[0]) == 0x0101)
    {
        // skip past the length/transaction id/etc. and read the first attribute
        attribType = *(short *)(&recvBuffer[skip]);
        attribLength = *(short *)(&recvBuffer[skip + 2]);
        switch (attribType)
        {
        case 0x0001:
            RB3E_DEBUG("Found MAPPED-ADDRESS attribute...", NULL);
            sprintf(returnedIP, "%d.%d.%d.%d", recvBuffer[skip + 8], recvBuffer[skip + 9], recvBuffer[skip + 10], recvBuffer[skip + 11]);
            break;
        default:
            break;
        }
    }

    RB3E_DEBUG("Closing STUN socket...", NULL);

    closeErr = closesocket(stunSocket);
    if (closeErr == SOCKET_ERROR)
    {
        RB3E_DEBUG("closesocket failed with error: %d", WSAGetLastError());
        return -1;
    }

    return 0;
}

#endif