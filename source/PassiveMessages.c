/*
    RB3Enhanced - PassiveMessages.c
    Functions for displaying a passive message.
*/

#include "ports.h"
#include "rb3/BandUI.h"
#include "rb3/PassiveMessagesPanel.h"

static DataArray messageArray = {0};
static DataNode messageNode = {0};

// Shows a passive message which provides information to the entire local band.
// Not recommended to call frequently or use during a song as each passive message will
// stay on screen for 3-4 seconds and could obscure the note track!
void DisplayMessage(char *message)
{
    messageNode.type = SYMBOL;
    messageNode.value.string = message;
    messageArray.mNodeCount = 1;
    messageArray.mNodes = (DataNodes *)&messageNode; // set this right for what it actually is
    messageArray.mRefCount = 1;
    messageArray.mFile = *(Symbol *)PORT_NULLSYMBOL;
    QueueMessage((*(BandUI *)PORT_THEBANDUI).passiveMessagesPanel, &messageArray, 0, (*(Symbol *)PORT_NULLSYMBOL).sym, 0xFFFFFFFF);
}