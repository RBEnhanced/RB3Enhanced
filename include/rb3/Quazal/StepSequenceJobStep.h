#ifndef _STEPSEQUENCEJOBSTEP_H
#define _STEPSEQUENCEJOBSTEP_H

typedef struct _StepSequenceJobStep
{
    void *jobFunc; // the function that gets called when the job is ran (?)
    int unk;
#ifdef RB3E_WII
    int unk2; // field only appears to exist on Wii
#endif
    char *jobName;
} StepSequenceJobStep;

#endif // _STEPSEQUENCEJOBSTEP_H