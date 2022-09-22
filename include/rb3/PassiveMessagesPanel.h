#ifndef _PASSIVEMESSAGESPANEL_H
#define _PASSIVEMESSAGESPANEL_H

#include "Data.h"

extern void DisplayMessage(char *message);
void QueueMessage(int *thisPassiveMessagesPanel, DataArray *ptr, int type, char *symText, int unk3);

#endif // _PASSIVEMESSAGESPANEL_H