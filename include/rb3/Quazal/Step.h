#ifndef _STEP_H
#define _STEP_H

typedef struct _Step
{
    void *m_pStepMethod; // the function that gets called when the job is ran
    int unk;
#ifdef RB3E_WII
    int unk2; // field only appears to exist on Wii
#endif
    char *m_szStepDescription;
} Step;

#endif // _STEP_H