#include "rb3e_include.h"
#include <string.h>

// dirty hack to get the game to print Quazal debug logging out to DbgPrint/OSReport instead
// limited to debug builds as this may introduce crashing or other issues. use with caution
void OperatorEqualsFmtHook(char *r3, char *r4)
{
    RB3E_DEBUG("Quazal: %s", r4);
    return;
}