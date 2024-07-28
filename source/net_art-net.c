/*
    RB3Enhanced - net_art-net.c
    Art-Net protocol : https://art-net.org.uk/
    
    Art-Net™ is a trademark of Artistic Licence Engineering Ltd.
*/

#include <stddef.h>
#include <stdio.h>
#include "quazal/InetAddress.h"
#include "rb3enhanced.h"
#include "version.h"
#include "ports.h"
#include "config.h"
#include "net.h"
#include "net_art-net.h"

#define ARTNET_IP 0xFFFFFFFF // 255.255.255.255, UDP multicast address, can be overwritten by ini if needed.
#define ARTNET_PORT 6454     // As defined in Art-Net protocol, can be overwritten by ini if needed.

int ArtNetSocket = -1;
static int ArtNetInitFailed = 0;
static int ArtNetIP = ARTNET_IP;
static short ArtNetPort = 0;
static RB3E_ArtNetPacket ArtNetPacket;
static RB3E_ArtNetPollPacket ArtNetPollPacket;
static short ArtNetPacketSize;

void RB3E_InitArtNet()
{
    InetAddress address;

    memset(ArtNetPacket.Data, 0, 513 * sizeof(ArtNetPacket.Data[0]));

    // if event broadcasting is disabled, or if we failed before, don't bother doing anything
    if (config.ArtNetEnable == 0 || ArtNetInitFailed)
        return;
    // parse the IP from the config file using Quazal
    if (config.ArtNetTarget[0] != '\0')
    {
        memset(&address, 0, sizeof(InetAddress));
        
        SetAddress(&address, config.ArtNetTarget);
        address.port = config.ArtNetPort;
        if (address.address != 0)
            ArtNetIP = address.address;
        if (address.port != 0)
            ArtNetPort = address.port;
    }
    RB3E_DEBUG("ArtNet Target: %08x:%i", ArtNetIP, ArtNetPort);

    // try to create the socket, but this might fail, if it does then don't try doing anything else
    ArtNetSocket = RB3E_CreateSocket(RB3E_TYPE_UDP);
    // TODO(Emma): will the socket api ever return another error?
    if (ArtNetSocket == -1)
    {
#ifdef RB3E_WII
        int wii_lasterror = RB3E_LastError();
        // check for SO_ENOENT/SO_ENOLINK and just fail out without setting failed
        // the game mightn't have initialised networking yet
        // TODO(Emma): is there any Xbox edge case similar to this?
        if (wii_lasterror == -48 || wii_lasterror == -45)
            return;
#endif
        ArtNetInitFailed = 1;
        return;
    }
    // set local port to be non-blocking
    RB3E_SetNonBlocking(ArtNetSocket, 1);
    
    // Setup default ArtNet packet structure for dmx.  A not so elegant way, just visable.
    ArtNetPacket.ID[ 0 ]     = 'A';
    ArtNetPacket.ID[ 1 ]     = 'r';
    ArtNetPacket.ID[ 2 ]     = 't';
    ArtNetPacket.ID[ 3 ]     = '-';
    ArtNetPacket.ID[ 4 ]     = 'N';
    ArtNetPacket.ID[ 5 ]     = 'e';
    ArtNetPacket.ID[ 6 ]     = 't';
    ArtNetPacket.ID[ 7 ]     = 0;
    ArtNetPacket.OpCode[ 0 ] = 0x00; // 0x5000 - DMX data
    ArtNetPacket.OpCode[ 1 ] = 0x50;
    ArtNetPacket.ProtocolHi  = 0x00; // Version - 14
    ArtNetPacket.ProtocolLo  = 0x0E;
    ArtNetPacket.Sequence    = 0x01;
    ArtNetPacket.Physical    = 0x00;
    ArtNetPacket.UniverseLo  = config.ArtNetUniverse;
    ArtNetPacket.UniverseHi  = 0x00;
    ArtNetPacket.Length      = 512;  // DMX512 so 512 byte payload
    
    // Pre-Clear data & set the global brightness
    memset(ArtNetPacket.Data, 0, 513 * sizeof(ArtNetPacket.Data[0]));
    ArtNetPacket.Data[ config.ArtNetGlobalBrightnessChannel ] = config.ArtNetGlobalBrightnessValue;
    
    // Setup default ArtPoll packet structure.
    ArtNetPollPacket.ID[ 0 ]      = 'A';
    ArtNetPollPacket.ID[ 1 ]      = 'r';
    ArtNetPollPacket.ID[ 2 ]      = 't';
    ArtNetPollPacket.ID[ 3 ]      = '-';
    ArtNetPollPacket.ID[ 4 ]      = 'N';
    ArtNetPollPacket.ID[ 5 ]      = 'e';
    ArtNetPollPacket.ID[ 6 ]      = 't';
    ArtNetPollPacket.ID[ 7 ]      = 0;
    ArtNetPollPacket.OpCode[ 0 ]  = 0x00; // 0x2000 - ArtPoll
    ArtNetPollPacket.OpCode[ 1 ]  = 0x20;
    ArtNetPollPacket.ProtocolHi   = 0x00; // Version 14
    ArtNetPollPacket.ProtocolLo   = 0x0E;
    ArtNetPollPacket.Flags        = 0x00; // Don't want reply other than ArtPollReply to this.
    ArtNetPollPacket.DiagPriority = 0x10; // Low priority

}

void RB3E_ArtNetSend(int state1, int state2)
{    

    if (ArtNetInitFailed)
        return;
      
    // if we don't have a socket, initialise one
    if (ArtNetSocket == -1)
    {
        RB3E_InitArtNet();
        if (ArtNetSocket == -1)
            return;
    }
    
    // Translate
    if (state2 == 1 ) // Fog on
    {
        ArtNetPacket.Data[config.ArtNetFogChannel] = config.ArtNetFogValue;
    }
    else if (state2 == 2 ) // Fog off
    {
        ArtNetPacket.Data[config.ArtNetFogChannel] = 0;
    }
    else if (state2 < 7) // Strobe on ( 3, 4, 5, 6 = four different rates )
    {
        int strobe_rate = state2 - 3;
        ArtNetPacket.Data[config.ArtNetStrobeRateChannel] = config.ArtNetStrobeRate[strobe_rate];
        ArtNetPacket.Data[config.ArtNetStrobeIntensityChannel] = config.ArtNetStrobeIntensity[strobe_rate];
        ArtNetPacket.Data[config.ArtNetStrobeDurationChannel] = config.ArtNetStrobeDuration[strobe_rate];
    }
    else if (state2 == 7) // Strobe off
    {
        ArtNetPacket.Data[config.ArtNetStrobeRateChannel] = 0;
        ArtNetPacket.Data[config.ArtNetStrobeIntensityChannel] = 0;
        ArtNetPacket.Data[config.ArtNetStrobeDurationChannel] = 0;
    }
    else if (state2 == 255) // all off
    {
        memset(ArtNetPacket.Data, 0, 513 * sizeof(ArtNetPacket.Data[0]));
        // Pre-Fill the global brightness
        ArtNetPacket.Data[ config.ArtNetGlobalBrightnessChannel ] = config.ArtNetGlobalBrightnessValue;
    }
    else if ( (state2 && 0xE0) != 0x00) // Led colour state
    {
        int colour_id = ( state2 / 32 ) - 1;  // 0 - 3 ( Blue, Green, Yellow, Red )
        int led_number = 0;
        int led_id = 1;
        
        // 2 different loops for off & on means that different led numbers of the same colour can use the same channel number.
        
        // Turn them all off.
        for (; led_number<8; led_number++)
        {
            if( (state1 & led_id) != led_id )
            {
                ArtNetPacket.Data[config.ArtNetColourTranslationChannel[colour_id][led_number]] = 0;
            }

            led_id *= 2;
        }
        
        // Turn on the needed items.
        if (config.ArtNetColourAdditive[colour_id])
        {  // Additive.  All values for 'on' led colours for same channel are added together.
            led_id = 1;
            for (led_number = 0; led_number<8; led_number++)
            {
                if( (state1 & led_id) == led_id )
                {
                    ArtNetPacket.Data[config.ArtNetColourTranslationChannel[colour_id][led_number]] += config.ArtNetColourTranslationValue[colour_id][led_number];
                }

                led_id *= 2;
            }
        }
        else
        { // Highest 'on' LED numbers value wins.
            led_id = 1;
            for (led_number = 0; led_number<8; led_number++)
            {
                if( (state1 & led_id) == led_id )
                {
                    ArtNetPacket.Data[config.ArtNetColourTranslationChannel[colour_id][led_number]] = config.ArtNetColourTranslationValue[colour_id][led_number];
                }

                led_id *= 2;
            }
        }
    }
    else
    {
        RB3E_DEBUG("ArtNet unhandled stage kit data : %i:%i", state1, state2);
        return;
    }
    
    // Send : 18 header + 512 DMX array
    RB3E_UDP_SendTo(ArtNetSocket, ArtNetIP, ArtNetPort, &ArtNetPacket, 18+512);
  
    // Move the sequence on.
    ArtNetPacket.Sequence++;
    if (!ArtNetPacket.Sequence)
        ArtNetPacket.Sequence = 1;

}

// It is a requirement of Art-Net that all controllers broadcast
// an ArtPoll every 2.5 to 3 seconds. This ensures that any network 
// devices can easily detect a disconnect.
void RB3E_ArtNetPoll()
{
    // No socket, no sendy
    if (ArtNetSocket == -1)
        return;

    RB3E_UDP_SendTo(ArtNetSocket, ArtNetIP, ArtNetPort, &ArtNetPollPacket, 14);
}