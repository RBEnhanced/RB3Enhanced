/*
    RB3Enhanced - net_events.h
    Local network events API for sending out game state.
*/

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

// events API
// maximum packet size
#define RB3E_EVENTS_MAXPACKET 0xFF
// protocol version
#define RB3E_EVENTS_PROTOCOL 0
// protocol magic ('RB3E')
#define RB3E_EVENTS_MAGIC 0x52423345

// types of packets that can be sent via the local socket
typedef enum _RB3E_PacketTypes
{
    RB3E_PACKET_ALIVE,          // content is a string of the RB3E_BUILDTAG value
    RB3E_PACKET_STATE,          // content is a char - 00=menus, 01=ingame
    RB3E_PACKET_SONG_NAME,      // content is a string of the current song name
    RB3E_PACKET_SONG_ARTIST,    // content is a string of the current song artist
    RB3E_PACKET_SONG_SHORTNAME, // content is a string of the current shortname
    RB3E_PACKET_SCORE,          // content is a RB3E_EventScore struct with score info
    RB3E_PACKET_STAGEKIT        // content is a RB3E_EventStagekit struct with stagekit info
} RB3E_EventTypes;

PACKSTART
// header for all local packets
typedef struct _RB3E_EventHeader
{
    int ProtocolMagic;
    char ProtocolVersion;
    char PacketType;
    char PacketSize;
    char Unused;
} PACKED RB3E_EventHeader;
// local socket packet
typedef struct _RB3E_EventPacket
{
    RB3E_EventHeader Header;
    char Data[RB3E_EVENTS_MAXPACKET];
} PACKED RB3E_EventPacket;
// packet that contains scoring information
typedef struct _RB3E_EventScore
{
    int TotalScore;
    int MemberScores[4];
    char Stars;
} PACKED RB3E_EventScore;
// packet that contains stagekit rumble data
typedef struct _RB3E_EventStagekit
{
    char LeftChannel;
    char RightChannel;
} PACKED RB3E_EventStagekit;
PACKEND

// start up the events socket
void RB3E_InitEvents();
// send an event, maximum size is 0xFF
void RB3E_SendEvent(int type, void *packet, int size);
