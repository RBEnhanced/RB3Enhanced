/*
    RB3Enhanced - net_liveless_online.c
    Code to manage connection to the Liveless online service, provide UI, etc
*/

#ifdef RB3E_XBOX
#include <xtl.h>
#include <string.h>
#include "net_liveless_online.h"
#include "config.h"
#include "net.h"
#include "rb3enhanced.h"
#include "ports.h"
#include "quazal/InetAddress.h"
#include "quazal/QuazalSocket.h"
#include "rb3/PassiveMessagesPanel.h"
#include "rb3/RockCentralGateway.h"
#include "xbox360.h"
#include "version.h"

// done in LocaleHooks.c
extern char RB3E_ActiveLocale[RB3E_LANG_LEN + 1];

// done in xbox360_liveless.c
extern QuazalSocket *game_socket;

static int Liveless_Socket = -1;

// state regarding our connection
char Liveless_Connected;
unsigned int Liveless_MyExternalIPv4;
char Liveless_MyUserCode[8];

// state regarding external users
char Liveless_PendingJoin;
unsigned int Liveless_PendingExternalIPv4;
unsigned int Liveless_PendingInternalIPv4;

static int last_player_num = 0;

typedef enum _LastDialogShown
{
    Dialog_JustAnOkay,
    Dialog_NotConnected,
    Dialog_Connected,
    Dialog_EnterUserCode
} LastDialogShown;
static int last_dialog_shown = Dialog_JustAnOkay;
static char login_initiated_from_dialog = 0;

static char connection_error;
static char connecting = 0;
static char logged_in = 0;

static int frames_since_last_packet;

static wchar_t entered_code[9];
static char entered_code_converted[8];

static XOVERLAPPED overlapped;
static MESSAGEBOX_RESULT result;

void Liveless_DoClientHello()
{
    LLMM_Packet packet = {0};
    LLMM_ClientHello *hello = (LLMM_ClientHello *)packet.data;

    packet.header.packet_magic = LLMM_MAGIC;
    packet.header.packet_size = sizeof(LLMM_ClientHello);
    packet.header.packet_type = LL_ClientHello;

    hello->protocol_version = LLMM_PROTOCOL;
    hello->using_xenia = RB3E_IsEmulator();
    strncpy(hello->language, RB3E_ActiveLocale, 3);
    strncpy(hello->rb3e_version, RB3E_BUILDTAG, sizeof(hello->rb3e_version));

    RB3E_TCP_Send(Liveless_Socket, &packet, sizeof(LLMM_PacketHeader) + sizeof(LLMM_ClientHello));
}

void Liveless_DoPong()
{
    LLMM_Packet packet = {0};
    packet.header.packet_magic = LLMM_MAGIC;
    packet.header.packet_size = 0;
    packet.header.packet_type = LL_Pong;
    RB3E_TCP_Send(Liveless_Socket, &packet, sizeof(LLMM_PacketHeader));
}

void Liveless_DoJoinUser(char *user_code)
{
    LLMM_Packet packet = {0};
    LLMM_UserJoinRequest *request = (LLMM_UserJoinRequest *)packet.data;

    packet.header.packet_magic = LLMM_MAGIC;
    packet.header.packet_size = sizeof(LLMM_UserJoinRequest);
    packet.header.packet_type = LL_UserJoinRequest;

    strncpy(request->user_code, user_code, 8);

    RB3E_TCP_Send(Liveless_Socket, &packet, sizeof(LLMM_PacketHeader) + sizeof(LLMM_UserJoinRequest));
}

void Liveless_DoClientLogin(int player_num, char make_proof, unsigned char *proof_key)
{
    XNADDR xna = {0};
    unsigned char hash[0x14] = {0};
    LLMM_Packet packet = {0};
    LLMM_ClientLogin *login = (LLMM_ClientLogin *)packet.data;
    packet.header.packet_magic = LLMM_MAGIC;
    packet.header.packet_size = sizeof(LLMM_ClientLogin);
    packet.header.packet_type = LL_ClientLogin;

    // get username and xuid of primary profile
    XUserGetXUID(player_num, &login->xuid);
    XUserGetName(player_num, login->gamertag, sizeof(login->gamertag));

    // get the local IPv4 for connecting across the same NAT
    XNetGetTitleXnAddr(&xna);
    login->local_ipv4 = xna.ina.S_un.S_addr;

    // make console signature proof
    if (make_proof)
    {
        XeCryptHmacSha(proof_key, 0x10, config.LivelessAddress, strlen(config.LivelessAddress), &login->xuid, sizeof(XUID), NULL, 0, hash, 0x14);
        if (RB3E_IsEmulator() == 0)
            XeKeysConsolePrivateKeySign(hash, login->proof);
        else // can't console sign on Xenia, TODO: passcode system as proof
            memcpy(login->proof, hash, 0x14);
    }

    RB3E_TCP_Send(Liveless_Socket, &packet, sizeof(LLMM_PacketHeader) + sizeof(LLMM_ClientLogin));
}

void Liveless_Disconnect()
{
    Liveless_Connected = 0;
    Liveless_Socket = -1;
    logged_in = 0;
    frames_since_last_packet = 0;
    login_initiated_from_dialog = 0;
    RB3E_DisposeSocket(Liveless_Socket);
}

// starts a connection to the liveless server
void Liveless_StartConnect(void *arg)
{
    InetAddress liveless_address;
    int r = 0;

    connecting = 1;
    if (Liveless_Socket != -1) // dispose of the socket if we already have a connection
        Liveless_Disconnect();

    // resolve the Liveless server domain
    RB3E_DEBUG("Resolving %s for Liveless...", config.LivelessAddress);
    memset(&liveless_address, 0, sizeof(InetAddress));
    SetAddress(&liveless_address, config.LivelessAddress);

    // verify that we've actually got it resolved
    if (liveless_address.address == 0)
    {
        RB3E_DEBUG("Failed to resolve Liveless domain", NULL);
        connection_error = 1;
        connecting = 0;
        return;
    }
    else
        RB3E_DEBUG("Resolved Liveless server domain to %08x", liveless_address.address);

    // create the socket
    Liveless_Socket = RB3E_CreateSocket(RB3E_TYPE_TCP);
    // set the socket to timeout after 4 seconds
    RB3E_SetTimeout(Liveless_Socket, 4000);

    // try connect to the server
    r = RB3E_TCP_Connect(Liveless_Socket, liveless_address.address, LLMM_PORT);
    if (r < 0)
    {
        RB3E_DEBUG("Connection failed %i %i", r, WSAGetLastError());
        connection_error = 2;
        connecting = 0;
        return;
    }
    // set non-blocking
    RB3E_SetNonBlocking(Liveless_Socket, 1);
    Liveless_Connected = 1;
    connecting = 0;
    // server expects us to say hi first
    Liveless_DoClientHello();
    return;
}

static wchar_t dialog_text_buffer[256];
static char delayed_task = 0;
void Liveless_DelayedAction(void *arg)
{
    RB3E_Sleep(300);
    if (delayed_task == 0)
    {
        XShowKeyboardUI(last_player_num, VKBD_LATIN_GAMERTAG, NULL, L"RB3Enhanced Liveless", L"Enter the user code of the user you would like to join. (Case insensitive)", entered_code, 9, &overlapped);
        last_dialog_shown = Dialog_EnterUserCode;
    }
    else if (delayed_task == 1)
    {
        LPWSTR buttons[1] = {L"Close"};
        wsprintfW(dialog_text_buffer, L"Your user code is %.8S.\n\nAnyone with this code and on your server can join you.\n\nServer: %S", Liveless_MyUserCode, config.LivelessAddress);
        XShowMessageBoxUI(last_player_num, L"RB3Enhanced Liveless", dialog_text_buffer, 1, buttons, 0, XMB_ALERTICON, &result, &overlapped);
        last_dialog_shown = Dialog_JustAnOkay;
    }
}

void Liveless_ShowConnectedUI(player_num)
{
    LPWSTR buttons[3] = {L"Join User Code", L"Show My User Code", L"Close"};
    wsprintfW(dialog_text_buffer, L"Connected to the Liveless server! Select 'Join User Code' to join another player, or 'Show My User Code' to show your own user code. (Be careful when streaming - anyone with this code can join you!)");
    XShowMessageBoxUI(player_num, L"RB3Enhanced Liveless", dialog_text_buffer, 3, buttons, 2, XMB_ALERTICON, &result, &overlapped);
    last_dialog_shown = Dialog_Connected;
}

static LLMM_Packet recv_buf;
void Liveless_Poll()
{
    int r = 0;

    // handle
    if (XHasOverlappedIoCompleted(&overlapped) && last_dialog_shown != Dialog_JustAnOkay)
    {
        // reset the overlapped state
        if (last_dialog_shown == Dialog_NotConnected)
        {
            if (result.dwButtonPressed == 0)
            { // 'Connect to Server' button
                login_initiated_from_dialog = 1;
                RB3E_CreateThread(Liveless_StartConnect, NULL, 0x1000);
            }
        }
        else if (last_dialog_shown == Dialog_Connected)
        {
            if (result.dwButtonPressed == 0)
            { // 'Join User Code' button
                RB3E_CreateThread(Liveless_DelayedAction, NULL, 0x1000);
                delayed_task = 0;
            }
            else if (result.dwButtonPressed == 1)
            { // 'Show My User Code' button
                RB3E_CreateThread(Liveless_DelayedAction, NULL, 0x1000);
                delayed_task = 1;
            }
        }
        else if (last_dialog_shown == Dialog_EnterUserCode)
        {
            if (overlapped.dwExtendedError == ERROR_SUCCESS)
            {
                RB3E_DEBUG("Joining user code %.8S", entered_code);
                wcstombs(entered_code_converted, entered_code, 8);
                Liveless_DoJoinUser(entered_code_converted);
            }
        }
        last_dialog_shown = 0;
    }

    // if we're not connected, don't do anything
    if (Liveless_Connected == 0)
        return;

    // 30 second timeout since last packet?
    // server should keep sending LL_Ping
    if (frames_since_last_packet > (60 * 30))
    {
        RB3E_DEBUG("Connection with Liveless server timed out, disconnecting.", NULL);
        connection_error = 4;
        Liveless_Disconnect();
        return;
    }

    // zero out the header part of the recv buf, this is in the mainloop so don't waste time 0ing everything
    memset(&recv_buf, 0, sizeof(LLMM_PacketHeader));
    // read from the socket
    r = RB3E_TCP_Recv(Liveless_Socket, &recv_buf, sizeof(LLMM_Packet));
    if (r < 0) // packet error
    {
        int err = RB3E_LastError();
        // if the error wasn't cause we were blocking, and the error wasn't zero, AND
        // we aren't on xenia OR, if on xenia, the error isn't WSAENOTSOCK(?) and is over 10000 (so a socket error)
        // i don't know, i'm just trying to get it to work, lol -Emma
        if (err != 10035 && err != 0 && (!RB3E_IsEmulator() || (err != 10038 && err > 10000)))
        {
            // connection error
            RB3E_DEBUG("Connection to Liveless socket severed, ret %i err %i", r, err);
            connection_error = 3;
            Liveless_Disconnect();
            return;
        }
    }
    else if (r > 0) // got a packet, do something about it
    {
        if (r < sizeof(LLMM_PacketHeader))
            RB3E_DEBUG("Recieved invalid packet size %i from Liveless server.", r);
        else if (recv_buf.header.packet_magic != LLMM_MAGIC)
            RB3E_DEBUG("Recieved invalid packet magic %04x from Liveless server.", recv_buf.header.packet_magic);
        else
        {
            frames_since_last_packet = 0;
            // after ClientHello, server should give us a ServerHello letting us login
            if (recv_buf.header.packet_type == LL_ServerHello)
            {
                LLMM_ServerHello *packet = (LLMM_ServerHello *)recv_buf.data;
                if (!packet->connection_allowed) // server doesn't want us in, so disconnect
                    Liveless_Disconnect();
                else // do a client login
                    Liveless_DoClientLogin(last_player_num, packet->requires_proof, packet->proof_key);
            }
            else if (recv_buf.header.packet_type == LL_ServerLoggedIn)
            {
                LLMM_ServerLoggedIn *packet = (LLMM_ServerLoggedIn *)recv_buf.data;
                RB3E_DEBUG("Liveless server logged in - IP: %08x, user code %.8s", packet->public_ipv4, packet->user_code);
                Liveless_MyExternalIPv4 = packet->public_ipv4;
                memcpy(Liveless_MyUserCode, packet->user_code, 8);
                logged_in = 1;
                if (login_initiated_from_dialog)
                { // only show an alert if this was user-initiated
                    login_initiated_from_dialog = 0;
                    DisplayMessage("Logged into Liveless!");
                }
            }
            else if (recv_buf.header.packet_type == LL_JoinResponse)
            {
                LLMM_JoinResponse *packet = (LLMM_JoinResponse *)recv_buf.data;
                RB3E_DEBUG("Got a join response from the Liveless server, public/private %08x %08x, username %.16s", packet->public_ipv4, packet->private_ipv4, packet->joining_user);
                Liveless_PendingJoin = 1;
                Liveless_PendingInternalIPv4 = packet->private_ipv4;
                Liveless_PendingExternalIPv4 = packet->public_ipv4;
                // hacky trick to join an invitation
                ExecuteDTA(PORT_ROCKCENTRALGATEWAY, "{do {join_invite_panel set_joining_user {user_mgr get_user_from_pad_num 0}} {ui_event_mgr trigger_event invite_accepted}}");
            }
            else if (recv_buf.header.packet_type == LL_Ping)
            {
                Liveless_DoPong();
            }
            else if (recv_buf.header.packet_type == LL_NATPunchRequest)
            {
                LLMM_NATPunchRequest *packet = (LLMM_NATPunchRequest *)recv_buf.data;
                RB3E_DEBUG("Server is asking us to punch through to public IP %08x", packet->public_ipv4);
                if (game_socket != NULL)
                {
                    RB3E_DEBUG("We've got a game socket so send it", NULL);
                    RB3E_UDP_SendTo((int)game_socket->native_socket, packet->public_ipv4, 9103, "NAT!", 4);
                }
            }
            else
            {
                RB3E_DEBUG("Got unknown packet type %i from the Liveless server", recv_buf.header.packet_type);
            }
        }
    }
    else
    {
        frames_since_last_packet++;
    }
}

int Liveless_ShowUI(int player_num)
{
    last_player_num = player_num;
    if (Liveless_Connected == 0 && connecting == 0)
    {
        LPWSTR buttons[2] = {L"Connect to Server", L"Close"};
        last_dialog_shown = Dialog_NotConnected;
        XShowMessageBoxUI(player_num, L"RB3Enhanced Liveless", L"You are not connected to the Liveless server. If you are using direct IP connections, go to 'Play Now' -> 'Find Online Players'.", 2, buttons, 0, XMB_ALERTICON, &result, &overlapped);
    }
    else if (Liveless_Connected == 0 && connecting == 1)
    {
        last_dialog_shown = Dialog_JustAnOkay;
        DisplayMessage("Connecting, please wait...");
    }
    else if (Liveless_Connected && logged_in == 0)
    {
        last_dialog_shown = Dialog_JustAnOkay;
        DisplayMessage("Waiting for log in, please wait...");
        logged_in = 1;
    }
    else if (Liveless_Connected && logged_in)
    {
        Liveless_ShowConnectedUI();
    }
    else
    {
        RB3E_MSG("WTF liveless state, blame Emma", NULL);
    }
    return 0;
}

#endif
