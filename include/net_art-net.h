/*
    RB3Enhanced - net_art-net.h
    Art-Net protocol : https://art-net.org.uk/

    Art-Net™ is a trademark of Artistic Licence Engineering Ltd.
*/


#ifndef PACKSTART
#ifdef RB3E_XBOX
// MSVC notation
#define PACKSTART __pragma(pack(push, 1))
#define PACKEND __pragma(pack(pop))
#define PACKED
#else
// GCC notation
#define PACKSTART
#define PACKEND
#define PACKED __attribute__((packed))
#endif
#endif

PACKSTART
// art-net packet structure for DMX512
typedef struct _RB3E_ArtNetPacket
{
  char ID[8];              // "Art-Net"
  char OpCode[2];          // 0x5000 : This is an ArtDmx data packet. It contains DMX512 information for a single Universe.
  char ProtocolHi;         // 0
  char ProtocolLo;         // 14 (Version 2)
  unsigned char Sequence;  // monotonic counter
  char Physical;           // 0x00
  char UniverseLo;         // low universe
  char UniverseHi;         // high universe
  short Length;            // data length (Must be even number for channel,data pairs)
  unsigned char Data[513]; // DMX512 data
} RB3E_ArtNetPacket;

// Consumers of ArtPoll shall accept as valid a packet of length 14 bytes or larger (Page 17)
typedef struct _RB3E_ArtNetPollPacket
{
  char ID[8];             // "Art-Net"
  char OpCode[2];         // 0x2000 This is an ArtPoll packet, no other data is contained in this UDP packet.
  char ProtocolHi;        // 0
  char ProtocolLo;        // 14 (Version 2)
  char Flags;             // 0x00 - Disable receiving diagnostics, etc & only send ArtPollReply in response to an ArtPoll.
  char DiagPriority;      // 0x10 Low priority message.
} RB3E_ArtNetPollPacket;

PACKEND

// Start up the socket & build default packet
void RB3E_InitArtNet();

// Decode weight data to art-net & broadcast
void RB3E_ArtNetSend(int state1, int state2);

// Send keep alive
void RB3E_ArtNetPoll();
