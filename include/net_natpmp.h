/*
    RB3Enhanced - net_natpmp.h
    Using the NAT-PMP and PCP protocols to open ports and get the external IP address of the user.
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

#define PCP_COMMAND_PORT 5351
#define PCP_LISTEN_PORT 5350

#define NATPMP_VERSION 0
#define PCP_VERSION 2

#define NATPMP_OP_GET_IP 0
#define NATPMP_OP_UDP_MAP 1
#define NATPMP_OP_TCP_MAP 2

#define PCP_OP_ANNOUNCE 0
#define PCP_OP_MAP 1
#define PCP_OP_PEER 2

PACKSTART
// both NATPMP and PCP share the same header
typedef struct _PCP_PacketHeader
{
    unsigned char version;
    unsigned char opcode;
    unsigned short response_code;
} PACKED PCP_PacketHeader;

// start NATPMP packets
typedef struct _NATPMP_ResponseHeader
{
    PCP_PacketHeader packet;
    unsigned int server_timestamp;
} PACKED NATPMP_ResponseHeader;

typedef struct _NATPMP_ExternalIPResponse
{
    NATPMP_ResponseHeader header;
    unsigned int external_ipv4;
} PACKED NATPMP_ExternalIPResponse;

typedef struct _NATPMP_MappingResponse
{
    NATPMP_ResponseHeader header;
    unsigned short internal_port;
    unsigned short external_port;
    unsigned int mapping_lifetime;
} PACKED NATPMP_MappingResponse;

typedef struct _NATPMP_MappingRequest
{
    PCP_PacketHeader header;
    unsigned short internal_port;
    unsigned short external_port;
    unsigned int mapping_lifetime;
} PACKED NATPMP_MappingRequest;
// end NATPMP packets

// PCP was designed for a post-IPv4 world
// Wii SO and Xbox 360 kernel does not support IPv6
typedef struct _IPv4MappedIPv6
{
    char all_zero[10];
    unsigned short max_uint16;
    unsigned int ipv4;
} PACKED IPv4MappedIPv6;

// start PCP packets
typedef struct _PCP_RequestHeader
{
    PCP_PacketHeader packet;
    unsigned int requested_lifetime;
    IPv4MappedIPv6 local_ipv6;
} PACKED PCP_RequestHeader;

typedef struct _PCP_ResponseHeader
{
    PCP_PacketHeader packet;
    unsigned int lifetime;
    unsigned int epoch_time;
    char reserved[12];
} PACKED PCP_ResponseHeader;

typedef struct _PCP_MAPRequest
{
    PCP_RequestHeader header;
    char nonce[12];
    char protocol;
    char reserved[3];
    unsigned short internal_port;
    unsigned short external_port;
    IPv4MappedIPv6 suggested_address;
} PACKED PCP_MAPRequest;

typedef struct _PCP_MAPResponse
{
    PCP_ResponseHeader header;
    char nonce[12];
    char protocol;
    char reserved[3];
    unsigned short internal_port;
    unsigned short external_port;
    IPv4MappedIPv6 assigned_address;
} PACKED PCP_MAPResponse;
// end PCP packets

PACKEND

extern unsigned char NATPMP_Success;
extern unsigned int NATPMP_ExternalIP;
extern unsigned short NATPMP_ExternalPort;

void NATPMP_RequestExternalIP();
void NATPMP_RequestOpenPort(unsigned short port);
void PCP_RequestOpenPort(unsigned short port);
void NATPMP_Init();
void NATPMP_Poll();
void UPNP_Poll();
