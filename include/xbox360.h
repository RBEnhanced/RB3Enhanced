/*
    RB3Enhanced - xbox360.h
    Miscellaneous Xbox 360 imports.
*/

#ifdef RB3E_XBOX

#include <xtl.h>

// strings
typedef struct _STRING {
    USHORT Length;
    USHORT MaximumLength;
    PCHAR  Buffer;
} STRING, *PSTRING;
void RtlInitAnsiString(PSTRING DestinationString, char *SourceString);
// drive mounting
int ObCreateSymbolicLink(PSTRING SymbolicLinkName, PSTRING DeviceName);
// XeCrypt
void XeCryptAesKey(PBYTE AesState, PBYTE InKey);
void XeCryptAesCbc(PBYTE AesState, PBYTE InBuffer, DWORD InLength, PBYTE OutBuffer, PBYTE Feed, BOOL Encrypt);

#endif // RB3E_XBOX