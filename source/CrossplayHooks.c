#include "rb3e_include.h"
#include "CrossplayHooks.h"

void MessageBrokerDDLHook(unsigned int doClassID)
{
    RB3E_DEBUG("Initing MessageBroker DOClass with orig ID %i, overriding to 36", doClassID);
    MessageBrokerDDL(36);
    return;
}