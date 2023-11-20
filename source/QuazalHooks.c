/*
    RB3Enhanced - QuazalHooks.c
    Hooks for Quazal-related functions.
    Quazal is the creator of the networking middleware used by RB3.
*/

#include "rb3e_include.h"
#include <string.h>

// dirty hack to get the game to print Quazal debug logging out to DbgPrint/OSReport instead
// limited to debug builds as this may introduce crashing or other issues. use with caution
void OperatorEqualsFmtHook(char *r3, char *r4)
{
    RB3E_DEBUG("Quazal: %s", r4);
    return;
}

int StepSequenceJobSetStepHook(int *unk, StepSequenceJobStep *step)
{
    // steps can have no name it seems; make sure we are not trying to print a null pointer
    if (step != NULL && step->jobName != NULL)
    {
        RB3E_DEBUG("Quazal Job: %s", step->jobName);
    }
    return StepSequenceJobSetStep(unk, step);
}