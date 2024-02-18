/*
    RB3Enhanced - QuazalHooks.h
    Hooks for Quazal-related functions.
    Quazal is the creator of the networking middleware used by RB3.
*/

#include "rb3/Quazal/Step.h"

extern void OperatorEqualsFmt(char *thisString, char *szString);
void OperatorEqualsFmtHook(char *thisString, char *szString);
extern int StepSequenceJobSetStep(int *thisStepSequenceJob, Step *oStep);
int StepSequenceJobSetStepHook(int *thisStepSequenceJob, Step *oStep);
