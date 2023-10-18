/*
    RB3Enhanced - net_liveless_online.h
    Protocol and function definition for liveless online multiplayer.
*/

#ifdef RB3E_XBOX

#include <xtl.h>
#include <stdint.h>

// MSVC notation
#define PACKSTART __pragma(pack(push, 1))
#define PACKEND __pragma(pack(pop))
#define PACKED

// maximum packet size
#define LLMM_MAXPACKET 0x300
// protocol version
#define LLMM_PROTOCOL 0
// protocol magic ('LL')
#define LLMM_MAGIC 0x4C4C
// default port
#define LLMM_PORT 19532

// types of packets that can be sent through C->S
typedef enum _LLMM_PacketType_CtoS
{
    LL_ClientHello,
    LL_ClientLogin,
    LL_Pong,
    LL_UserJoinRequest
} LLMM_PacketType_CtoS;

// types of packets that can be sent through S->C
typedef enum _LLMM_PacketType_StoC
{
    LL_ServerHello,
    LL_ServerLoggedIn,
    LL_Ping,
    LL_JoinResponse,
    LL_JoinDenied,
    LL_NATPunchRequest
} LLMM_PacketType_StoC;

// join types for JoinResponse
typedef enum _LLMM_JoinType
{
    LL_JoinType_UserCode, // user entered a code
    LL_JoinType_External  // user joined this lobby thru website/discord/irc/matrix/xmpp/etc
} LLMM_JoinType;

// deny types for JoinDenied
typedef enum _LLMM_JoinDenyType
{
    LL_Denied_CodeNotFound // user with code didn't exist
} LLMM_JoinDenyType;

PACKSTART
typedef struct _LLMM_PacketHeader
{
    unsigned short packet_magic;
    unsigned char reserved;
    unsigned char packet_type;
    unsigned short packet_size;
} PACKED LLMM_PacketHeader;

typedef struct _LLMM_Packet
{
    LLMM_PacketHeader header;
    char data[LLMM_MAXPACKET];
} PACKED LLMM_Packet;

typedef struct _LLMM_ClientHello
{
    unsigned char protocol_version;
    unsigned char using_xenia;
    char language[3];
    char rb3e_version[0x30];
} PACKED LLMM_ClientHello;

typedef struct _LLMM_ServerHello
{
    unsigned char connection_allowed;
    unsigned char requires_proof;
    unsigned char proof_key[0x10];
} PACKED LLMM_ServerHello;

typedef struct _LLMM_ClientLogin
{
    XUID xuid;
    char gamertag[16];
    unsigned int local_ipv4;
    unsigned char proof[0x228];
} PACKED LLMM_ClientLogin;

typedef struct _LLMM_ServerLoggedIn
{
    unsigned int public_ipv4;
    char user_code[8];
} PACKED LLMM_ServerLoggedIn;

typedef struct _LLMM_UserJoinRequest
{
    char user_code[8];
} PACKED LLMM_UserJoinRequest;

typedef struct _LLMM_JoinResponse
{
    unsigned char join_type;
    char joining_user[16];
    XUID joining_xuid;
    unsigned int public_ipv4;
    unsigned int private_ipv4;
} PACKED LLMM_JoinResponse;

typedef struct _LLMM_JoinDenied
{
    unsigned char deny_reason;
} PACKED LLMM_JoinDenied;

typedef struct _LLMM_NATPunchRequest
{
    unsigned int public_ipv4;
} PACKED LLMM_NATPunchRequest;
PACKEND

// poll the liveless socket for new data
void Liveless_Poll();
// tell the liveless state we want to join a room
void Liveless_DoJoinUser(char *user_code);
// show message-box based UI
int Liveless_ShowUI(int player_num);

// state regarding our connection
extern char Liveless_Connected;
extern unsigned int Liveless_MyExternalIPv4;
extern char Liveless_MyUserCode[8];

// state regarding external users
extern char Liveless_PendingJoin;
extern unsigned int Liveless_PendingExternalIPv4;
extern unsigned int Liveless_PendingInternalIPv4;

#endif
