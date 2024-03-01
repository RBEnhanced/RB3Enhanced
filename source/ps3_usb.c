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

int SantrollerProbe(int device)
{
    RB3E_DEBUG("Santroller probed (device=%08x)", device);
    return CELL_USBD_PROBE_FAILED;
}

int SantrollerAttach(int device)
{
    RB3E_DEBUG("Santroller attached (device=%08x)", device);
    return 0;
}

int SantrollerDetach(int device)
{
    RB3E_DEBUG("Santroller detached (device=%08x)", device);
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
    RB3E_DEBUG("Device valid check (device=%08x, descriptor=%p)", device, descriptor);
    RB3E_DEBUG("Device: %02x %02x", descriptor->idVendor, descriptor->idProduct);
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
