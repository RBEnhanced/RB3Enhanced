/*
    RB3Enhanced - ps3_usb.c
    USB handling for the PS3, including Wii and Santroller support
*/

#ifdef RB3E_PS3

#include <cell/usbd.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "ports.h"

#define LE16(i) (((((i) & 0xFF) << 8) | (((i) >> 8) & 0xFF)) & 0xFFFF)

static int stagekit_device_id = -1;
static int stagekit_pipe = -1;
void SantrollerSendStagekit(uint8_t l, uint8_t r)
{
    if (stagekit_device_id == -1 || stagekit_pipe == -1)
        return;
    uint8_t report[] = {0x01, 0x5A, l, r, 0x00, 0x00, 0x00, 0x00};
    UsbDeviceRequest req;
    req.bmRequestType = USB_REQTYPE_TYPE_CLASS | USB_REQTYPE_RECIP_INTERFACE | USB_REQTYPE_DIR_TO_DEVICE;
    req.bRequest = 9; // SET_REPORT
    req.wValue = 0x200;
    req.wIndex = 0;
    req.wLength = sizeof(report);
    cellUsbdControlTransfer(stagekit_pipe, &req, report, NULL, 0);
}

static int santrollerpipe = 0;
void SantrollerKickToNormal(void *ctrl_pipe)
{
    int pipe = santrollerpipe;
    uint8_t rep[8];
    UsbDeviceRequest req;
    req.bmRequestType = USB_REQTYPE_TYPE_CLASS | USB_REQTYPE_RECIP_INTERFACE | USB_REQTYPE_DIR_TO_HOST;
    req.bRequest = 0x01;
    req.wValue = 0x0303;
    req.wIndex = 0;
    req.wLength = 8;
    cellUsbdControlTransfer(pipe, &req, rep, NULL, 0);
}

int SantrollerProbe(int device)
{
    UsbDeviceDescriptor *descriptor = cellUsbdScanStaticDescriptor(device, NULL, USB_DESCRIPTOR_TYPE_DEVICE);
    if (descriptor != NULL && // santroller detection
        descriptor->idVendor == LE16(0x1209) && descriptor->idProduct == LE16(0x2882))
    {
        return CELL_USBD_PROBE_SUCCEEDED;
    }
    return CELL_USBD_PROBE_FAILED;
}

int SantrollerAttach(int device)
{
    UsbDeviceDescriptor *desc = cellUsbdScanStaticDescriptor(device, NULL, USB_DESCRIPTOR_TYPE_DEVICE);
    if (desc != NULL)
    {
        int ctrl_pipe = cellUsbdOpenPipe(device, NULL);
        if (ctrl_pipe >= CELL_OK)
        {
            UsbConfigurationDescriptor *config = cellUsbdScanStaticDescriptor(device, NULL, USB_DESCRIPTOR_TYPE_CONFIGURATION);
            santrollerpipe = ctrl_pipe;
            cellUsbdSetConfiguration(ctrl_pipe, config->bConfigurationValue, SantrollerKickToNormal, &santrollerpipe);
            if (desc->bcdDevice == LE16(0x0900))
            { // stagekit
                stagekit_device_id = device;
                stagekit_pipe = ctrl_pipe;
            }
            return CELL_USBD_ATTACH_SUCCEEDED;
        }
        else
        {
            RB3E_DEBUG("cellUsbdOpenPipe returned %08x for santroller", ctrl_pipe);
        }
    }
    return CELL_USBD_ATTACH_FAILED;
}

int SantrollerDetach(int device)
{
    if (device == stagekit_device_id)
    {
        stagekit_device_id = -1;
        stagekit_pipe = -1;
    }
    return 0;
}

CellUsbdLddOps santroller_ldd = {
    "Santroller",
    SantrollerProbe,
    SantrollerAttach,
    SantrollerDetach,
};

CellUsbdLddOps wii_ldd;

void RegisterLDDsHook()
{
    // register LDDs for the Wii instruments
    memcpy(&wii_ldd, (CellUsbdLddOps *)PORT_LDDADDRESS, sizeof(CellUsbdLddOps));
    wii_ldd.name = "RB3EWiiInstruments";
    cellUsbdRegisterExtraLdd2(&wii_ldd, 0x1BAD, 0x0000, 0xFFFF);
    // register LDD for santroller
    cellUsbdRegisterExtraLdd(&santroller_ldd, 0x1209, 0x2882);
    RB3E_MSG("Initialised Usbd LDDs!", NULL);
}

int IsUSBDeviceValid(int device, UsbDeviceDescriptor *descriptor);
int IsUSBDeviceValidHook(int device, UsbDeviceDescriptor *descriptor)
{
    RB3E_DEBUG("USB Instrument - VID: %02x - PID: %02x", LE16(descriptor->idVendor), LE16(descriptor->idProduct));
    if (descriptor->idVendor == LE16(0x1BAD)) // Harmonix Music Systems
    {
        // don't patch it everywhere, instead just change the descriptor in-memory
        // all real sneaky style :eye:
        descriptor->idVendor = LE16(0x12BA); // Sony Computer Entertainment

#define SWAP_INSTRUMENT(wii_pid, ps3_pid)       \
    if (descriptor->idProduct == LE16(wii_pid)) \
    descriptor->idProduct = LE16(ps3_pid)

        SWAP_INSTRUMENT(0x0004, 0x0200); // RB1 Guitar
        SWAP_INSTRUMENT(0x3010, 0x0200); // RB2 Guitar
        SWAP_INSTRUMENT(0x0005, 0x0210); // RB1 Drums
        SWAP_INSTRUMENT(0x3110, 0x0210); // RB2 Drums
        SWAP_INSTRUMENT(0x3138, 0x0218); // MPA Drums
        SWAP_INSTRUMENT(0x3330, 0x2330); // Pro Keyboard
        SWAP_INSTRUMENT(0x3338, 0x2338); // MPA Keyboard
        SWAP_INSTRUMENT(0x3430, 0x2430); // Pro Mustang
        SWAP_INSTRUMENT(0x3438, 0x2438); // MPA Mustang
        SWAP_INSTRUMENT(0x3530, 0x2530); // Pro Squire
        SWAP_INSTRUMENT(0x3538, 0x2538); // MPA Squire
    }
    return IsUSBDeviceValid(device, descriptor);
}

#endif // RB3E_PS3
