/*
    RB3Enhanced - net_events.c
    Local network events API for sending out game state.
*/

#include <stddef.h>
#include <stdio.h>
#include "rb3/InetAddress.h"
#include "rb3enhanced.h"
#include "version.h"
#include "ports.h"
#include "config.h"
#include "net.h"
#include "net_events.h"

#define BROADCAST_IP 0xFFFFFFFF // 255.255.255.255, UDP multicast address
#define BROADCAST_PORT 0x524E   // 'RN' for uhhh rocknet or something

int RB3E_EventsSocket = -1;
static int InitFailed = 0;
static int EventsIP = BROADCAST_IP;
static short EventsPort = BROADCAST_PORT;

void RB3E_InitEvents()
{
    InetAddress address;
    // if event broadcasting is disabled, or if we failed before, don't bother doing anything
    if (config.EnableEvents == 0 || InitFailed)
        return;
    // parse the IP from the config file using Quazal
    if (config.BroadcastTarget[0] != '\0')
    {
        memset(&address, 0, sizeof(InetAddress));
        SetAddress(&address, config.BroadcastTarget);
        if (address.address != 0)
            EventsIP = address.address;
        if (address.port != 0)
            EventsPort = address.port;
    }
    RB3E_DEBUG("Events target: %08x:%i", EventsIP, EventsPort);
    // try to create the socket, but this might fail, if it does then don't try doing anything else
    RB3E_EventsSocket = RB3E_CreateSocket(RB3E_TYPE_UDP);
    // TODO(Emma): will the socket api ever return another error?
    if (RB3E_EventsSocket == -1)
    {
#ifdef RB3E_WII
        int wii_lasterror = RB3E_LastError();
        // check for SO_ENOENT/SO_ENOLINK and just fail out without setting failed
        // the game mightn't have initialised networking yet
        // TODO(Emma): is there any Xbox edge case similar to this?
        if (wii_lasterror == -48 || wii_lasterror == -45)
            return;
#endif
        InitFailed = 1;
        return;
    }
    // bind the local port and set it to be non-blocking
    RB3E_SetNonBlocking(RB3E_EventsSocket, 1);
    RB3E_BindPort(RB3E_EventsSocket, BROADCAST_PORT);
    // send an init message
    // TODO(Emma): put this in a loop and send it out every 10 seconds or so
    RB3E_SendEvent(RB3E_EVENT_ALIVE, RB3E_BUILDTAG, sizeof(RB3E_BUILDTAG));
}

void RB3E_SendEvent(int type, void *data, int size)
{
    RB3E_EventPacket packet;
    // dont bother trying to set stuff up if things are disabled
    if (config.EnableEvents == 0 || InitFailed)
        return;
    // if we don't have a socket, initialise one
    if (RB3E_EventsSocket == -1)
    {
        RB3E_InitEvents();
        if (RB3E_EventsSocket == -1)
            return;
    }
    // truncate packets bigger than this, to avoid buffer overflows and network congestion
    if (size > RB3E_EVENTS_MAXPACKET)
        size = RB3E_EVENTS_MAXPACKET;
    // sort out the packet we're sending
    packet.Header.ProtocolMagic = RB3E_EVENTS_MAGIC;
    packet.Header.ProtocolVersion = RB3E_EVENTS_PROTOCOL;
    packet.Header.PacketType = type;
    packet.Header.PacketSize = size;
#ifdef RB3E_XBOX
    packet.Header.Platform = RB3E_IsEmulator() ? RB3E_PLATFORM_XENIA : RB3E_PLATFORM_XBOX;
#elif RB3E_WII
    packet.Header.Platform = RB3E_IsEmulator() ? RB3E_PLATFORM_DOLPHIN : RB3E_PLATFORM_WII;
#else
    packet.Header.Platform = RB3E_PLATFORM_UNKNOWN;
#endif
    memcpy(packet.Data, data, size);
    // Deploy!
    RB3E_UDP_SendTo(RB3E_EventsSocket, EventsIP, EventsPort, &packet, sizeof(RB3E_EventHeader) + size);
}
