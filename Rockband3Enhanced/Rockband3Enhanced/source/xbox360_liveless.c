/*
    RB3Enhanced - xbox360_liveless.c
    Functions that fake the connection to Xbox Live and the XSession API, for online play.
    This also disables packet security for network communications, allowing GoCentral connectivity.
*/

#ifdef RB3E_XBOX

#include <xtl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/rb3/InetAddress.h"
#include "../include/net.h"
#include "../include/gocentral.h"
#include "../include/config.h"
#include "../include/utilities.h"
#include "../include/ppcasm.h"
#include "../include/rb3enhanced.h"
#include "../include/ports.h"

// user's external IP.
IN_ADDR ExternalIP = {0};
USHORT ExternalIPShort[4];

BOOL CanUseGoCentral()
{
    XNQOS *xnqos;
    int res;
    // If we're under Xenia, we can always use GoCentral/Liveless.
    if (RB3E_IsEmulator())
        return TRUE;
    // Detect a connection to Xbox Live. If this is successful, we can't safely use GoCentral or Liveless features.
    res = XNetQosServiceLookup(0, NULL, &xnqos);
    RB3E_DEBUG("XNetQosServiceLookup: %i", res);
    return (res != 0);
}

int ReturnsZero()
{
    return 0;
}
int ReturnsOne()
{
    return 1;
}

// Socket creation hook to allow insecure networking
BOOL optEnable = TRUE;
SOCKET socketHook(int af, int type, int protocol)
{
    SOCKET newSock = socket(af, type, protocol == IPPROTO_VDP ? IPPROTO_UDP : protocol);
    setsockopt(newSock, SOL_SOCKET, 0x5801, (PCSTR)&optEnable, sizeof(BOOL));
    if (type == SOCK_STREAM)
        setsockopt(newSock, SOL_SOCKET, 0x5802, (PCSTR)&optEnable, sizeof(BOOL));
    return newSock;
}

// XSession hook
DWORD XSessionSearchExHook(DWORD dwProcedureIndex,
                           DWORD dwUserIndex,
                           DWORD dwNumResults,
                           DWORD dwNumUsers,
                           WORD wNumProperties,
                           WORD wNumContexts,
                           PXUSER_PROPERTY pSearchProperties,
                           PXUSER_CONTEXT pSearchContexts,
                           DWORD *pcbResultsBuffer,
                           PXSESSION_SEARCHRESULT_HEADER pSearchResults,
                           PXOVERLAPPED pXOverlapped)
{
    XSESSION_SEARCHRESULT result = {0};
    int i = 0;
    int j = 0;
    for (i = 0; i < wNumContexts; i++)
    {
        RB3E_DEBUG("Context %i: %08x=%08x", i, pSearchContexts[i].dwContextId, pSearchContexts[i].dwValue);
    }
    for (i = 0; i < wNumProperties; i++)
    {
        RB3E_DEBUG("Property %i: %08x type %x", i, pSearchProperties[i].dwPropertyId, pSearchProperties[i].value.type);
        if (pSearchProperties[i].value.type == XUSER_DATA_TYPE_INT32)
        {
            RB3E_DEBUG("    Integer data: %08x", pSearchProperties[i].value.nData);
        }
    }
    XSessionSearchEx(dwProcedureIndex, dwUserIndex, dwNumResults, dwNumUsers, wNumProperties, wNumContexts, pSearchProperties, pSearchContexts, pcbResultsBuffer, pSearchResults, pXOverlapped);
    // return a fake session with our redirect IP address
    RB3E_DEBUG("Returning a fake XSession...", NULL);
    result.cContexts = 1;
    result.cProperties = 1;
    result.pProperties = pSearchProperties;
    result.pContexts = pSearchContexts;
    result.info.hostAddress.wPortOnline = 9103;
    result.info.hostAddress.inaOnline.S_un.S_addr = REDIRECT_IP_ADDRESS_INT;
    for (i = 0; i < 16; i++)
        result.info.keyExchangeKey.ab[i] = i + 1;
    for (i = 0; i < 8; i++)
        result.info.sessionID.ab[i] = i + 1;
    result.dwOpenPublicSlots = 4;
    result.dwOpenPrivateSlots = 4;
    result.dwFilledPrivateSlots = 1;
    result.dwFilledPublicSlots = 1;
    if (pSearchResults != NULL)
    {
        pSearchResults->dwSearchResults = 1;
        pSearchResults->pResults = &result;
    }
    return ERROR_SUCCESS;
}

// Stub Xam functions
DWORD XamUserCheckPrivilegeHook(DWORD dwUserIndex, XPRIVILEGE_TYPE PrivilegeType, PBOOL pfResult)
{
    *pfResult = TRUE;
    return 0;
}
DWORD XamUserGetSigninInfoHook(DWORD dwUserIndex, DWORD flags, PXUSER_SIGNIN_INFO xSigninInfo)
{
    DWORD ret = XUserGetSigninInfo(dwUserIndex, flags, xSigninInfo);
    if (xSigninInfo->UserSigninState == eXUserSigninState_SignedInLocally)
        xSigninInfo->UserSigninState = eXUserSigninState_SignedInToLive;
    return ret;
}
XUSER_SIGNIN_STATE XamUserGetSigninStateHook(DWORD dwUserIndex)
{
    XUSER_SIGNIN_STATE state = XUserGetSigninState(dwUserIndex);
    if (state == eXUserSigninState_SignedInLocally)
        state = eXUserSigninState_SignedInToLive;
    return state;
}
// Stub XNet functions
int XNetXnAddrToInAddrHook(XNADDR *pxna, XNKID *pxnkid, IN_ADDR *pina)
{
    RB3E_DEBUG("XNetXnAddrToInAddr called offline %u.%u.%u.%u online %u.%u.%u.%u", pxna->ina.S_un.S_un_b.s_b1, pxna->ina.S_un.S_un_b.s_b2, pxna->ina.S_un.S_un_b.s_b3, pxna->ina.S_un.S_un_b.s_b4, pxna->inaOnline.S_un.S_un_b.s_b1, pxna->inaOnline.S_un.S_un_b.s_b2, pxna->inaOnline.S_un.S_un_b.s_b3, pxna->inaOnline.S_un.S_un_b.s_b4);
    if (pxna->ina.S_un.S_addr == 0)
    {
        RB3E_DEBUG("XnAddr was null - doing direct connect", NULL);
        XNetXnAddrToInAddr(pxna, pxnkid, pina);
        pina->S_un.S_addr = REDIRECT_IP_ADDRESS_INT;
        return ERROR_SUCCESS;
    }
    else
    {
        RB3E_DEBUG("Non-null XnAddr, converting properly", NULL);
        XNetXnAddrToInAddr(pxna, pxnkid, pina);
        pina->S_un.S_addr = pxna->inaOnline.S_un.S_addr;
        return ERROR_SUCCESS;
    }
}
int XNetGetTitleXnAddrHook(XNADDR *pxna)
{
    int ret = XNetGetTitleXnAddr(pxna);
    int i = 0;

    // load external IP
    sscanf(config.ExternalIP, "%hu.%hu.%hu.%hu", &ExternalIPShort[0], &ExternalIPShort[1], &ExternalIPShort[2], &ExternalIPShort[3]);
    ExternalIP.S_un.S_un_b.s_b1 = (BYTE)ExternalIPShort[0];
    ExternalIP.S_un.S_un_b.s_b2 = (BYTE)ExternalIPShort[1];
    ExternalIP.S_un.S_un_b.s_b3 = (BYTE)ExternalIPShort[2];
    ExternalIP.S_un.S_un_b.s_b4 = (BYTE)ExternalIPShort[3];

    pxna->inaOnline.S_un.S_addr = ExternalIP.S_un.S_addr;
    pxna->wPortOnline = 9103;
    for (i = 0; i < 20; i++)
    {
        pxna->abOnline[i] = i;
    }
    RB3E_DEBUG("XNetGetTitleXnAddr called offline %u.%u.%u.%u online %u.%u.%u.%u", pxna->ina.S_un.S_un_b.s_b1, pxna->ina.S_un.S_un_b.s_b2, pxna->ina.S_un.S_un_b.s_b3, pxna->ina.S_un.S_un_b.s_b4, pxna->inaOnline.S_un.S_un_b.s_b1, pxna->inaOnline.S_un.S_un_b.s_b2, pxna->inaOnline.S_un.S_un_b.s_b3, pxna->inaOnline.S_un.S_un_b.s_b4);
    return ret;
}

XNQOS xnqos = {0};
int XNetQosLookupHook(UINT cxna, const XNADDR *apxna[], const XNKID *apxnkid[], const XNKEY *apxnkey[], UINT cina, const IN_ADDR aina[], const DWORD adwServiceId[], UINT cProbes, DWORD dwBitsPerSec, DWORD dwFlags, WSAEVENT hEvent, XNQOS **ppxnqos)
{
    unsigned int i = 0;
    RB3E_DEBUG("XNetQoSLookup called for XnAddr offline %u.%u.%u.%u online %u.%u.%u.%u", apxna[0]->ina.S_un.S_un_b.s_b1, apxna[0]->ina.S_un.S_un_b.s_b2, apxna[0]->ina.S_un.S_un_b.s_b3, apxna[0]->ina.S_un.S_un_b.s_b4, apxna[0]->inaOnline.S_un.S_un_b.s_b1, apxna[0]->inaOnline.S_un.S_un_b.s_b2, apxna[0]->inaOnline.S_un.S_un_b.s_b3, apxna[0]->inaOnline.S_un.S_un_b.s_b4);
    XNetQosLookup(cxna, apxna, apxnkid, apxnkey, cina, aina, adwServiceId, cProbes, dwBitsPerSec, dwFlags, hEvent, ppxnqos);
    if (ppxnqos != NULL)
    {
        if (*ppxnqos == NULL)
        {
            *ppxnqos = &xnqos;
            (*ppxnqos)->cxnqos = 1;
        }
        for (i = 0; i < (*ppxnqos)->cxnqos; i++)
        {
            // falsify the QoS results, making the game think it succeeded
            (*ppxnqos)->axnqosinfo[i].cProbesXmit = 4;
            (*ppxnqos)->axnqosinfo[i].cProbesRecv = 4;
            (*ppxnqos)->axnqosinfo[i].cbData = 1;
            (*ppxnqos)->axnqosinfo[i].pbData = (BYTE *)"A";
            (*ppxnqos)->axnqosinfo[i].wRttMinInMsecs = 4;
            (*ppxnqos)->axnqosinfo[i].wRttMedInMsecs = 10;
            (*ppxnqos)->axnqosinfo[i].dwUpBitsPerSec = 13125;
            (*ppxnqos)->axnqosinfo[i].dwDnBitsPerSec = 21058;
            (*ppxnqos)->axnqosinfo[i].bFlags = XNET_XNQOSINFO_COMPLETE | XNET_XNQOSINFO_TARGET_CONTACTED;
        }
    }
    return ERROR_SUCCESS;
}

// defined in net_stun.c, not worth its own header
int STUN_GetExternalIP(char *stunServerIP, short stunPort, char *returnedIP, int socket_type);

void InitLivelessHooks()
{
    char returnedIP[RB3E_MAX_IP_LEN];

    // if neither are enabled, don't care
    if (config.EnableGoCentral == 0 && config.EnableLiveless == 0)
        return;
    // check that we can actually use it
    if (!CanUseGoCentral())
    {
        MESSAGEBOX_RESULT result;
        LPWSTR buttons[1] = {L"OK"};
        XOVERLAPPED overlapped;
        XShowMessageBoxUI(0, L"RB3Enhanced Warning", L"Your console is currently connected to Xbox Live. You are not able to play on GoCentral or through Liveless. Please enable 'liveblock' in your Dashlaunch settings.", 1, buttons, 0, XMB_ALERTICON, &result, &overlapped);
        return;
    }
    // Bypass SG and allow insecure sockets
    POKE_32(PORT_XL_BYPASS_SG, LI(11, 1));
    POKE_B(PORT_XL_USESECURESOCKETS, &ReturnsZero);
    POKE_B(PORT_SOCKET_STUB, &socketHook);
    POKE_B(PORT_XAMUSERGETSIGNININFO, &XamUserGetSigninInfoHook);
    POKE_B(PORT_XAMUSERGETSIGNINSTATE, &XamUserGetSigninStateHook);
    POKE_B(PORT_XAMUSERCHECKPRIVILEGE, &XamUserCheckPrivilegeHook);
    RB3E_MSG("Applied socket patches!", NULL);
    if (config.EnableLiveless)
    {
        // XSession patches to return success for everything
        POKE_B(PORT_XL_XSESSIONCREATE, &ReturnsZero);
        POKE_B(PORT_XL_XSESSIONJOINREMOTE, &ReturnsZero);
        POKE_B(PORT_XL_XSESSIONMODIFY, &ReturnsZero);
        // XNet patches to return success for everything
        POKE_B(PORT_XNETCONNECT, &ReturnsZero);
        POKE_B(PORT_XNETREGISTERKEY, &ReturnsZero);
        POKE_B(PORT_XNETUNREGISTERKEY, &ReturnsZero);
        POKE_B(PORT_XNETUNREGISTERINADDR, &ReturnsZero);
        // XNet patches for XnAddr handling
        POKE_B(PORT_XNETGETTITLEXNADDR, &XNetGetTitleXnAddrHook);
        POKE_B(PORT_XNETXNADDRTOINADDR, &XNetXnAddrToInAddrHook);
        // XNet patches for QoS
        POKE_B(PORT_XNETQOSLOOKUP, &XNetQosLookupHook);
        // remove session mask check
        POKE_32(PORT_SESSION_MASK_CHECK, NOP);
        // session search hook
        POKE_B(PORT_XL_XSESSIONSEARCHEX, &XSessionSearchExHook);
        // don't write or expect VDP packets (crossplat experiments)
        POKE_32(PORT_VDP_DISABLE1, NOP);
        POKE_32(PORT_VDP_DISABLE2, NOP);
        POKE_32(PORT_VDP_DISABLE3, NOP);
        POKE_32(PORT_VDP_DISABLE4, NOP);
        // we need to patch out checking for xnqos
        // TODO: check if real console needs real xnqos for 3+ multiplayer
        if (RB3E_IsEmulator())
        {
            POKE_32(PORT_XNQOS_PROBE1, NOP);
            POKE_32(PORT_XNQOS_PROBE2, NOP);
            POKE_32(PORT_XNQOS_PROBE3, NOP);
        }
        RB3E_MSG("Applied liveless patches!", NULL);
    }
    if (config.EnableGoCentral)
    {
        // Spoof Rock Central login state
        POKE_32(PORT_RCG_POLL_LOGGEDIN, NOP);
        // Audition Mode patches
        POKE_32(PORT_AUD_PATCH_CRASH, BLR);
        POKE_32(PORT_AUD_PATCH_NET1, NOP);
        POKE_32(PORT_AUD_PATCH_NET2, NOP);
        POKE_32(PORT_AUD_PATCH_NET3, NOP);
        POKE_32(PORT_AUD_PATCH_NET4, LI(3, 0));
        POKE_32(PORT_AUD_PATCH_NET5, LI(3, 1));
        POKE_32(PORT_AUD_PATCH_UNK, LI(11, 1));
        POKE_32(PORT_AUD_PATCH_REPL, LI(11, 1));
        POKE_32(PORT_AUD_INVALID_DATA_CHECK, LI(3, 1));
        POKE_B(PORT_AUD_HANDLE_MESSAGES, PORT_AUD_HANDLE_MSG_JUMP);
        RB3E_MSG("Applied GoCentral patches!", NULL);
    }
    // STUN will not work on Xenia
    if (config.STUNServer[0] != '\0' && !RB3E_IsEmulator())
    {
        if (config.STUNServerPort != 0)
        {
            if (STUN_GetExternalIP(config.STUNServer, config.STUNServerPort, returnedIP, RB3E_TYPE_UDP) == 0)
            {
                strncpy(config.ExternalIP, returnedIP, RB3E_MAX_IP_LEN);
                RB3E_DEBUG("IP: %s", returnedIP);
            }
            else
            {
                RB3E_DEBUG("Could not get IP via STUN!", NULL);
            }
        }
        else
        {
            RB3E_DEBUG("STUN port not set!", NULL);
        }
    }
}

#endif // RB3E_XBOX
