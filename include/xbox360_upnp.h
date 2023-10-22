/*
    RB3Enhanced - xbox360_upnp.h
    Miscellaneous Xbox 360 imports from Xam to use UPnP.
    Definitions from xkelib, adjusted for cleanliness/coding style.
*/

#ifdef RB3E_XBOX

#include <xtl.h>

typedef struct _UpnpDeviceDescription_t UpnpDeviceDescription;
typedef struct _UpnpServiceDescription_t UpnpServiceDescription;

typedef struct _UpnpDevice_t
{
    char uuid[0x80];
    struct sockaddr_in address;
    char relative_path[0x100];
    int last_update;
} UpnpDevice;

typedef struct _UpnpDeviceDescription_t
{
    char uuid[0x80];
    char type[0x80];
    char friendly_name[0x41];
    char manufacturer[0x41];
    char manufacturer_url[0x100];
    char model_name[0x21];
    char model_description[0x81];
    char model_number[0x21];
    char model_url[0x100];
    char serial_number[0x41];
    char upc[0xD];
    UpnpDeviceDescription *pParent;
    UpnpDeviceDescription *pNextSibling;
    UpnpDeviceDescription *pFirstChild;
    UpnpServiceDescription *pFirstService;
} UpnpDeviceDescription;

typedef struct _UpnpServiceDescription_t
{
    char type[0x80];
    char id[0x81];
    unsigned short description_port;
    char description_url[0x100];
    unsigned short control_port;
    char control_url[0x100];
    unsigned short event_port;
    char event_url[0x100];
    UpnpDeviceDescription *device;
    UpnpServiceDescription *next_service;
} UpnpServiceDescription;

typedef union _UpnpValue_t
{
    unsigned int integer;
    char *string;
} UpnpValue;

typedef struct _UpnpActionOutputParams_t
{
    char *name;
    int type;
    int size;
    UpnpValue values[1]; // size value makes me think there's more...
} UpnpActionOutputParams;

#define NETDLL_TITLE 1

int NetDll_UpnpStartup(int caller);
int NetDll_UpnpCleanup(int caller);

int NetDll_UpnpSearchCreate(int caller, char *search_target, int max_devices, int try_count, int send_interval, void *event, int flags, HANDLE *out_handle);
int NetDll_UpnpDescribeCreate(int caller, UpnpDevice *device, int max_devices, int max_services, void *event, HANDLE *out_handle);
int NetDll_UpnpActionCreate(int caller, struct sockaddr_in *host, char *control_url, char *service_type, char *action, char *input, int input_len, UpnpActionOutputParams *out_params,
                            int num_out_params, void *work_buffer, int work_size, void *event, HANDLE *out_handle);

// "get" functions return 0 when done, and return 1 when busy
int NetDll_UpnpSearchGetDevices(int caller, HANDLE handle, UpnpDevice *devices, int *device_count);
int NetDll_UpnpDescribeGetResults(int caller, HANDLE handle, UpnpDeviceDescription **devices, int *device_count, UpnpServiceDescription **services, int *service_count);
int NetDll_UpnpActionGetResults(int caller, HANDLE handle, UpnpActionOutputParams **out_params, int *out_param_count);

int NetDll_UpnpDoWork(int caller, HANDLE handle, int sleep);

int NetDll_UpnpCloseHandle(int caller, HANDLE handle);

#endif // RB3E_XBOX
