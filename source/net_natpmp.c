/*
    RB3Enhanced - net_natpmp.c
    Using the NAT-PMP and PCP protocols to open ports and get the external IP address of the user.
*/

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "ports.h"
#include "net.h"
#include "net_natpmp.h"
#include "rb3/Random.h"

unsigned char NATPMP_Success = 0;
unsigned int NATPMP_ExternalIP = 0;
unsigned short NATPMP_ExternalPort = 0;

static int natpmp_socket = -1;

// keep track of whether we get "invalid version" responses or the gateway doesn't support it
static char failed_pcp = 0;
static char failed_natpmp = 0;

// the epoch value expected from the server in a response/announce
static int expected_server_epoch = 0;
// seconds remaining until the port opening expires
static int time_until_expiry = -1;

// increase every frame, when it hits 60 then reset to 0 and act like a second has passed
static int ticker = 0;

static unsigned int gateway_ipv4 = 0;

// nonce for pcp mappings - random upon startup
static unsigned char mapping_nonce[12];

// 1 hour request lifetime, should be good enough
#define REQUEST_LIFETIME 3600

static char has_tried_init = 0;

// silly helper function
static void fill_ipv4_mapped_ipv6(IPv4MappedIPv6 *map, unsigned int ipv4)
{
    memset(map->all_zero, 0, sizeof(map->all_zero));
    map->max_uint16 = 0xFFFF;
    map->ipv4 = ipv4;
}

void NATPMP_RequestExternalIP()
{
    PCP_PacketHeader req = {0};
    // fill the request
    req.version = NATPMP_VERSION;
    req.opcode = NATPMP_OP_GET_IP;
    // fire it off to the gateway
    RB3E_UDP_SendTo(natpmp_socket, gateway_ipv4, PCP_COMMAND_PORT, &req, sizeof(PCP_PacketHeader));
    RB3E_DEBUG("Requested external IP from NATPMP", NULL);
}

void NATPMP_RequestOpenPort(unsigned short port)
{
    NATPMP_MappingRequest req = {0};
    // fill the packet header
    req.header.version = NATPMP_VERSION;
    req.header.opcode = NATPMP_OP_UDP_MAP;
    // fill the request
    req.mapping_lifetime = REQUEST_LIFETIME;
    req.external_port = port;
    req.internal_port = port;
    // fire it off to the gateway
    RB3E_UDP_SendTo(natpmp_socket, gateway_ipv4, PCP_COMMAND_PORT, &req, sizeof(NATPMP_MappingRequest));
    RB3E_DEBUG("Requested port %i from NATPMP", port);
}

void PCP_RequestOpenPort(unsigned short port)
{
    PCP_MAPRequest req = {0};
    // fill the packet header
    req.header.packet.version = PCP_VERSION;
    req.header.packet.opcode = PCP_OP_MAP;
    req.header.requested_lifetime = REQUEST_LIFETIME;
    fill_ipv4_mapped_ipv6(&req.header.local_ipv6, RB3E_GetInternalIP());
    // fill the mapping request
    req.protocol = 17; // IP_PROTO_UDP;
    req.external_port = port;
    req.internal_port = port;
    memcpy(req.nonce, mapping_nonce, sizeof(mapping_nonce));
    fill_ipv4_mapped_ipv6(&req.suggested_address, 0);
    // fire it off to the gateway
    RB3E_UDP_SendTo(natpmp_socket, gateway_ipv4, PCP_COMMAND_PORT, &req, sizeof(NATPMP_MappingRequest));
    RB3E_DEBUG("Requested port %i from PCP", port);
}

void NATPMP_Init()
{
    int i = 0;
    has_tried_init = 1;
    // create a socket, bind to the broadcast port and then set non-blocking
    natpmp_socket = RB3E_CreateSocket(RB3E_TYPE_UDP);
    // do we need to bind? don't think so
    // RB3E_BindPort(natpmp_socket, PCP_LISTEN_PORT);
    RB3E_SetNonBlocking(natpmp_socket, 1);
    //  get the address of the gateway
    gateway_ipv4 = RB3E_GetGatewayIP();
    // set a random nonce to use for pcp mappings
    for (i = 0; i < sizeof(mapping_nonce); i++)
        mapping_nonce[i] = RandomInt(0, 255);
    RB3E_DEBUG("Set up NATPMP socket for gateway %08x", gateway_ipv4);
}

void NATPMP_Poll()
{
    // tick up
    ticker++;
    if (ticker > 60)
    {
        expected_server_epoch++;
        if (time_until_expiry > 0)
            time_until_expiry--;
        ticker = 0;
    }
    if (natpmp_socket == -1 && !has_tried_init)
    {
        // TODO(Emma): only initialise this when the game actually tries setting up a socket
        NATPMP_Init();
        PCP_RequestOpenPort(9103);
    }
    // we can't do anything else without a socket
    if (natpmp_socket != -1)
    {
        unsigned int recv_ip;
        unsigned short recv_port;
        char recv_buffer[128];
        int r = RB3E_UDP_RecvFrom(natpmp_socket, &recv_ip, &recv_port, recv_buffer, sizeof(recv_buffer));
        if (r >= 0)
        {
            PCP_PacketHeader *header = (PCP_PacketHeader *)recv_buffer;
            if (recv_ip != gateway_ipv4)
            {
                RB3E_DEBUG("Recieved packet from %08x, but that isn't gateway %08x, ignoring.", recv_ip, gateway_ipv4);
                return;
            }
            // handle falling back upon an unsupported version
            // initially i expected the version in the header to correspond with the request packet
            // but certain routers (tp-link is one of them) doesn't and IDK correct behaviour
            if (header->response_code == 1)
            {
                if (failed_pcp == 0)
                {
                    RB3E_DEBUG("Gateway told us it doesn't understand PCP, fallback to NAT-PMP.", NULL);
                    failed_pcp = 1;
                    NATPMP_RequestExternalIP();
                }
                else
                {
                    failed_natpmp = 1;
                    RB3E_DEBUG("Gateway told us it doesn't understand NATPMP, give up.", NULL);
                }
                return;
            }
            // handle a NATPMP response
            if (header->version == NATPMP_VERSION)
            {
                if (header->opcode == (0x80 | NATPMP_OP_GET_IP) && header->response_code == 0)
                {
                    NATPMP_ExternalIPResponse *resp = (NATPMP_ExternalIPResponse *)recv_buffer;
                    expected_server_epoch = resp->header.server_timestamp;
                    RB3E_DEBUG("Gateway gave us external IPv4 %08x", resp->external_ipv4);
                    NATPMP_ExternalIP = resp->external_ipv4;
                    NATPMP_RequestOpenPort(9103);
                }
                else if (header->opcode == (0x80 | NATPMP_OP_UDP_MAP) && header->response_code == 0)
                {
                    NATPMP_MappingResponse *resp = (NATPMP_MappingResponse *)recv_buffer;
                    expected_server_epoch = resp->header.server_timestamp;
                    time_until_expiry = resp->mapping_lifetime;
                    RB3E_DEBUG("Gateway assigned us port %i for internal %i, for %i seconds", resp->external_port, resp->internal_port, resp->mapping_lifetime);
                    NATPMP_ExternalPort = resp->external_port;
                    // only mark as success if it's marked the correct port
                    if (NATPMP_ExternalPort == 9103)
                        NATPMP_Success = 1;
                }
                else
                {
                    RB3E_DEBUG("Gateway responded with unknown NATPMP opcode %02X, response code %i", header->opcode, header->response_code);
                }
            }
            // handle a PCP response
            else if (header->version == PCP_VERSION)
            {
                if (header->opcode == (0x80 | PCP_OP_MAP) && header->response_code == 0)
                {
                    PCP_MAPResponse *resp = (PCP_MAPResponse *)recv_buffer;
                    expected_server_epoch = resp->header.epoch_time;
                    time_until_expiry = resp->header.lifetime;
                    RB3E_DEBUG("Gateway assigned us external %08x port %i for internal %i, for %i seconds", resp->assigned_address.ipv4, resp->external_port, resp->internal_port, resp->header.lifetime);
                    NATPMP_ExternalIP = resp->assigned_address.ipv4;
                    NATPMP_ExternalPort = resp->external_port;
                    // only mark as success if it's marked the correct port
                    if (NATPMP_ExternalPort == 9103)
                        NATPMP_Success = 1;
                }
                else
                {
                    RB3E_DEBUG("Gateway responded with unknown PCP opcode %02X, response code %i", header->opcode, header->response_code);
                }
            }
            //
            else
            {
                RB3E_DEBUG("Gateway replied with unknown version %02x, opcode %02x, response code %i", header->version, header->opcode, header->response_code);
            }
        }
        else if (r < 0 && RB3E_LastError() != 10035)
        {
            RB3E_DEBUG("NATPMP socket error %i %i", r, RB3E_LastError());
            RB3E_DisposeSocket(natpmp_socket);
            natpmp_socket = -1;
        }
        if (time_until_expiry == 0)
        {
            RB3E_DEBUG("Expiry timer exceeded, renewing port mapping...", NULL);
            time_until_expiry = -1;
            if (failed_pcp)
                NATPMP_RequestOpenPort(9103);
            else
                PCP_RequestOpenPort(9103);
        }
    }
}
