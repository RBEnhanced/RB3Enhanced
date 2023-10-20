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
typedef enum _RB3E_Events_PacketTypes
{
    RB3E_EVENT_ALIVE,          // content is a string of the RB3E_BUILDTAG value
    RB3E_EVENT_STATE,          // content is a char - 00=menus, 01=ingame
    RB3E_EVENT_SONG_NAME,      // content is a string of the current song name
    RB3E_EVENT_SONG_ARTIST,    // content is a string of the current song artist
    RB3E_EVENT_SONG_SHORTNAME, // content is a string of the current shortname
    RB3E_EVENT_SCORE,          // content is a RB3E_EventScore struct with score info
    RB3E_EVENT_STAGEKIT,       // content is a RB3E_EventStagekit struct with stagekit info
    RB3E_EVENT_BAND_INFO,      // content is a RB3E_EventBandInfo struct with band info
    RB3E_EVENT_VENUE_NAME,     // content is a string of the current venue name
    RB3E_EVENT_SCREEN_NAME,    // content is a string of the current screen name
    RB3E_EVENT_DX_DATA,        // content is a RB3E_EventModData struct with info given by DTA
} RB3E_Events_EventTypes;
// identifiers for each platform RB3E can or will run on
typedef enum _RB3E_Events_PlatformIDs
{
    RB3E_PLATFORM_XBOX,          // RB3E is running on a physical Xbox 360
    RB3E_PLATFORM_XENIA,         // RB3E is running on Xenia emulator
    RB3E_PLATFORM_WII,           // RB3E is running on a physical Wii
    RB3E_PLATFORM_DOLPHIN,       // RB3E is running on Dolphin emulator
    RB3E_PLATFORM_PS3,           // RB3E is running on a physical PS3
    RB3E_PLATFORM_RPCS3,         // RB3E is running on RPCS3 emulator
    RB3E_PLATFORM_UNKNOWN = 0xFF // just a nice placeholder
} RB3E_Events_PlatformIDs;

PACKSTART
// header for all local packets
typedef struct _RB3E_EventHeader
{
    int ProtocolMagic;
    char ProtocolVersion;
    char PacketType;
    char PacketSize;
    char Platform;
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
// packet that contains band member data
typedef struct _RB3E_EventBandInfo
{
    char MemberExists[4];
    char Difficulty[4];
    char TrackType[4];
} PACKED RB3E_EventBandInfo;
// packet meant for mods (rb3dx) to send out arbitrary information
typedef struct _RB3E_EventModData
{
    char IdentifyValue[50];
    char String[200];
} PACKED RB3E_EventModData;
PACKEND

// start up the events socket
void RB3E_InitEvents();
// send an event, maximum size is 0xFF
void RB3E_SendEvent(int type, void *packet, int size);
