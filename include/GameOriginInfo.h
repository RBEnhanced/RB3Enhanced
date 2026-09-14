/*
    RB3Enhanced - GameOriginInfo.h
*/

typedef struct _GameOriginInfo
{
    char *gameOrigin;
    int num;
} GameOriginInfo;

extern int numGameOrigins;
extern GameOriginInfo originInfo[100];