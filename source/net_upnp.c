/*
    RB3Enhanced - net_upnp.c
    Using the Xbox 360 UPnP APIs to map the port of the current player.
*/

#ifdef RB3E_XBOX

#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "ports.h"
#include "net.h"
#include "rb3enhanced.h"
#include "xbox360.h"
#include "xbox360_upnp.h"

// increase every frame, when it hits 60 then reset to 0 and act like a second has passed
static int ticker = 0;
static int seconds_passed = 0;

static char has_tried_init = 0;
static HANDLE doing_work = INVALID_HANDLE_VALUE;

static UpnpDevice device;
static UpnpServiceDescription service_desc;

// put this somewhere static, it doesn't belong on the stack :v
static UpnpActionOutputParams act_params = {0};
static char xml_buffer[800] = {0};

typedef enum _RB3E_UPNPTask
{
    UPnP_Starting,
    UPnP_SearchingDevicesWANIP,
    UPnP_SearchingDevicesWANPPP,
    UPnP_SearchingDescriptions,
    UPnP_ObtainingPort,
    UPnP_ObtainingExternalIP,
    UPnP_Done,
    UPnP_Fail
} RB3E_UPNPTask;

static int current_upnp_task = UPnP_Starting;

void UPNP_Poll()
{
    // if we're done with it (or it failed), don't waste any cycles doing anything else
    if (current_upnp_task == UPnP_Done || current_upnp_task == UPnP_Fail)
        return;
    // xenia does not have upnp (yet?), don't bother
    if (RB3E_IsEmulator())
    {
        current_upnp_task = UPnP_Fail;
        return;
    }

    // tick up
    ticker++;
    if (ticker > 60)
    {
        ticker = 0;
        seconds_passed++;
    }
    // initialise UPnP if we haven't initialised it before
    if (!has_tried_init)
    {
        int start_ret = NetDll_UpnpStartup(NETDLL_TITLE);
        if (start_ret != 0)
        {
            RB3E_DEBUG("Couldn't start up UPnP, error %i", start_ret);
        }
        has_tried_init = 1;
        return;
    }
    // if we have a handle for doing work, do it for a bit
    if (doing_work != INVALID_HANDLE_VALUE)
    {
        if (ticker == 0)
            RB3E_DEBUG("UPnP is doing work...", NULL);
        NetDll_UpnpDoWork(NETDLL_TITLE, doing_work, 0);
    }

    // weird state machine? go!
    if (current_upnp_task == UPnP_Starting)
    {
        NetDll_UpnpSearchCreate(NETDLL_TITLE, "urn:schemas-upnp-org:service:WANIPConnection:1", 1, 0, 0, NULL, 0, &doing_work);
        current_upnp_task = UPnP_SearchingDevicesWANIP;
        seconds_passed = 0;
    }
    else if (current_upnp_task == UPnP_SearchingDevicesWANIP)
    {
        int devices_returned = 1;
        int ret = NetDll_UpnpSearchGetDevices(NETDLL_TITLE, doing_work, &device, &devices_returned);
        if (ret == 0)
        {
            if (devices_returned >= 1)
            {
                RB3E_DEBUG("Found a WANIPConnection device at IP %08x, relative path %s, UUID %s", device.address.sin_addr.S_un.S_addr, device.relative_path, device.uuid);
                NetDll_UpnpCloseHandle(NETDLL_TITLE, doing_work);
                NetDll_UpnpDescribeCreate(NETDLL_TITLE, &device, 24, 24, NULL, &doing_work);
                current_upnp_task = UPnP_SearchingDescriptions;
                seconds_passed = 0;
            }
            else if (seconds_passed > 2)
            {
                RB3E_DEBUG("Finding WANIPConnection device timed out after 3 seconds, trying WANPPPConnection", NULL);
                NetDll_UpnpCloseHandle(NETDLL_TITLE, doing_work);
                NetDll_UpnpSearchCreate(NETDLL_TITLE, "urn:schemas-upnp-org:service:WANPPPConnection:1", 1, 0, 0, NULL, 0, &doing_work);
                current_upnp_task = UPnP_SearchingDevicesWANPPP;
                seconds_passed = 0;
            }
        }
        else
        {
            RB3E_DEBUG("UpnpSearchGetDevices returned %i, failing", ret);
            current_upnp_task = UPnP_Fail;
        }
    }
    else if (current_upnp_task == UPnP_SearchingDevicesWANPPP)
    {
        int devices_returned = 1;
        int ret = NetDll_UpnpSearchGetDevices(NETDLL_TITLE, doing_work, &device, &devices_returned);
        if (ret == 0)
        {
            if (devices_returned >= 1)
            {
                RB3E_DEBUG("Found a WANPPPConnection device at IP %08x, relative path %s, UUID %s", device.address.sin_addr.S_un.S_addr, device.relative_path, device.uuid);
                NetDll_UpnpCloseHandle(NETDLL_TITLE, doing_work);
                NetDll_UpnpDescribeCreate(NETDLL_TITLE, &device, 24, 24, NULL, &doing_work);
                current_upnp_task = UPnP_SearchingDescriptions;
                seconds_passed = 0;
            }
            else if (seconds_passed > 2)
            {
                RB3E_DEBUG("Finding WANPPPConnection device timed out after 3 seconds, failing", NULL);
                NetDll_UpnpCloseHandle(NETDLL_TITLE, doing_work);
                current_upnp_task = UPnP_Fail;
            }
        }
        else
        {
            RB3E_DEBUG("UpnpSearchGetDevices returned %i, failing", ret);
            current_upnp_task = UPnP_Fail;
        }
    }
    else if (current_upnp_task == UPnP_SearchingDescriptions)
    {
        UpnpDeviceDescription *device_descriptions;
        int num_device_descriptions = 0;
        UpnpServiceDescription *service_descriptions;
        int num_service_descriptions = 0;
        int ret = NetDll_UpnpDescribeGetResults(NETDLL_TITLE, doing_work, &device_descriptions, &num_device_descriptions, &service_descriptions, &num_service_descriptions);
        if (ret == 1)
        {
            if (seconds_passed > 4)
            {
                RB3E_DEBUG("Getting device descriptions from device timed out after 5 seconds, failing", NULL);
                NetDll_UpnpCloseHandle(NETDLL_TITLE, doing_work);
                current_upnp_task = UPnP_Fail;
            }
        }
        else
        {
            int i = 0;
            char found_service = 0;
            RB3E_DEBUG("Got %i devices %i services with ret %i", num_device_descriptions, num_service_descriptions, ret);
            // we don't care about the devices, just enumerate the services for what we want
            for (i = 0; i < num_service_descriptions; i++)
            {
                UpnpServiceDescription *desc = &(service_descriptions[i]);
                if (strcmp(desc->type, "urn:schemas-upnp-org:service:WANIPConnection:1") == 0 ||
                    strcmp(desc->type, "urn:schemas-upnp-org:service:WANPPPConnection:1") == 0)
                {
                    RB3E_DEBUG("Found service type %s with control URL %s", desc->type, desc->control_url);
                    found_service = 1;
                    memcpy(&service_desc, desc, sizeof(UpnpServiceDescription));
                    break;
                }
            }
            NetDll_UpnpCloseHandle(NETDLL_TITLE, doing_work);
            if (!found_service)
            {
                RB3E_DEBUG("No WAN service found, failing", NULL);
                current_upnp_task = UPnP_Fail;
            }
            else // request our external IPv4 address
            {
                act_params.name = "NewExternalIPAddress";
                act_params.type = 2;  // string?
                act_params.size = 16; // maximum size? (4 octet + null terminator = 16 bytes)
                NetDll_UpnpActionCreate(NETDLL_TITLE, &device.address, service_desc.control_url, service_desc.type,
                                        "GetExternalIPAddress", NULL, 0, &act_params, 1, NULL, 0, NULL, &doing_work);
                RB3E_DEBUG("Found WAN service, performing GetExternalIPAddress", NULL);
                current_upnp_task = UPnP_ObtainingExternalIP;
                seconds_passed = 0;
            }
        }
    }
    else if (current_upnp_task == UPnP_ObtainingExternalIP)
    {
        UpnpActionOutputParams *out_params = NULL;
        int num_out_params = 0;
        int ret = NetDll_UpnpActionGetResults(NETDLL_TITLE, doing_work, &out_params, &num_out_params);
        if (ret == 1)
        {
            if (seconds_passed > 2)
            {
                RB3E_DEBUG("Getting external IPv4 from device timed out after 3 seconds, failing", NULL);
                NetDll_UpnpCloseHandle(NETDLL_TITLE, doing_work);
                current_upnp_task = UPnP_Fail;
            }
        }
        else
        {
            int i = 0;
            char got_ip = 0;
            RB3E_DEBUG("Got response from external IPv4 request with ret %i", ret);
            for (i = 0; i < num_out_params; i++)
            {
                if (out_params[i].name != NULL && strcmp(out_params[i].name, "NewExternalIPAddress") == 0)
                {
                    RB3E_DEBUG("UPnP gateway IP: %s", out_params[i].values->string);
                    strncpy(config.ExternalIP, out_params[i].values->string, RB3E_MAX_IP_LEN);
                    got_ip = 1;
                }
            }
            NetDll_UpnpCloseHandle(NETDLL_TITLE, doing_work);
            if (!got_ip)
            {
                RB3E_DEBUG("No IPv4 address found, failing", NULL);
                current_upnp_task = UPnP_Fail;
            }
            else
            {
                unsigned int my_ipv4 = RB3E_GetInternalIP();
                unsigned char *ipv4_bytes = (unsigned char *)&my_ipv4;
                sprintf(xml_buffer,
                        "<NewPortMappingDescription>%s</NewPortMappingDescription>"
                        "<NewLeaseDuration>0</NewLeaseDuration>"
                        "<NewInternalClient>%i.%i.%i.%i</NewInternalClient>"
                        "<NewEnabled>1</NewEnabled>"
                        "<NewExternalPort>%i</NewExternalPort>"
                        "<NewRemoteHost></NewRemoteHost>"
                        "<NewProtocol>UDP</NewProtocol>"
                        "<NewInternalPort>%i</NewInternalPort>",
                        "RB3Enhanced",                                              // NewPortMappingDescription
                        ipv4_bytes[0], ipv4_bytes[1], ipv4_bytes[2], ipv4_bytes[3], // NewInternalClient
                        9103, 9103);                                                // NewExternalPort / NewInternalPort
                RB3E_DEBUG("Making port mapping for 9103 via UPnP", NULL);
                NetDll_UpnpActionCreate(NETDLL_TITLE, &device.address, service_desc.control_url, service_desc.type,
                                        "AddPortMapping", xml_buffer, strlen(xml_buffer), NULL, 0, NULL, 0, NULL, &doing_work);
                current_upnp_task = UPnP_ObtainingPort;
                seconds_passed = 0;
            }
        }
    }
    else if (current_upnp_task == UPnP_ObtainingPort)
    {
        int ret = NetDll_UpnpActionGetResults(NETDLL_TITLE, doing_work, NULL, NULL);
        if (ret == 1)
        {
            if (seconds_passed > 2)
            {
                RB3E_DEBUG("Mapping port from device timed out after 3 seconds, failing", NULL);
                NetDll_UpnpCloseHandle(NETDLL_TITLE, doing_work);
                current_upnp_task = UPnP_Fail;
            }
        }
        else
        {
            NetDll_UpnpCloseHandle(NETDLL_TITLE, doing_work);
            RB3E_DEBUG("Succesfully mapped port 9103 via UPnP!", NULL);
            current_upnp_task = UPnP_Done;
        }
    }
}

#endif
