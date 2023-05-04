#ifdef RB3E_WII
#include "rb3/BinStream.h"

extern void GetUserDataWriteHook(BinStream *this, void *data, int size);
extern void GetUserDataWriteEndianHook(BinStream *this, int size);
extern void GetUserDataReadHook(BinStream *this, void *data, int size);
#endif