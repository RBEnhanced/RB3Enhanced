/*
    RB3Enhanced - xbox360.h
    Miscellaneous Xbox 360 imports.
*/

#ifdef RB3E_XBOX

#include <xtl.h>

// missing imports from xboxkrnl
// strings
typedef struct _STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PCHAR Buffer;
} STRING, *PSTRING;
void RtlInitAnsiString(PSTRING DestinationString, char *SourceString);
// drive mounting
int ObCreateSymbolicLink(PSTRING SymbolicLinkName, PSTRING DeviceName);
// XeCrypt
void XeCryptAesKey(PBYTE AesState, PBYTE InKey);
void XeCryptAesCbc(PBYTE AesState, PBYTE InBuffer, DWORD InLength, PBYTE OutBuffer, PBYTE Feed, BOOL Encrypt);
// XEX import checking
DWORD XexGetModuleHandle(PCSTR ModuleName, PHANDLE OutHandle);
DWORD XexGetProcedureAddress(HANDLE ModuleHandle, DWORD Ordinal, PVOID OutAddress);

// our own defined functions
void InitCryptoHooks();
void InitLivelessHooks();
void InitInputHooks();
#endif // RB3E_XBOX
