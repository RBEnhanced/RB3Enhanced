#ifndef _DATETIME_H
#define _DATETIME_H

typedef struct _DateTime
{
    unsigned char sec;
    unsigned char min;
    unsigned char hour;
    unsigned char day;
    unsigned char month;
    unsigned char year; // years since 1900
} DateTime;

#endif // _DATETIME_H
