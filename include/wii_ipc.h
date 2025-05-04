/*
    RB3Enhanced - wii_ipc.h
    IPC structures for interacting with the IOS processor.
*/
#ifdef RB3E_WII
#ifndef _WII_IPC_H
#define _WII_IPC_H
// For IPCCommandType, IOSOpenMode and SeekMode:
// Copyright 2008 Dolphin Emulator Project
// Licensed under GPLv2+

#include <stdint.h>

typedef int32_t s32;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef enum _IPCCommandType
{
    IPC_CMD_OPEN = 1,
    IPC_CMD_CLOSE = 2,
    IPC_CMD_READ = 3,
    IPC_CMD_WRITE = 4,
    IPC_CMD_SEEK = 5,
    IPC_CMD_IOCTL = 6,
    IPC_CMD_IOCTLV = 7,
    IPC_REPLY = 8,
    IPC_INTERRUPT = 9,
} IPCCommandType;

typedef enum _IOSOpenMode
{
    IOS_OPEN_READ = 1,
    IOS_OPEN_WRITE = 2,
    IOS_OPEN_RW = (IOS_OPEN_READ | IOS_OPEN_WRITE)
} IOSOpenMode;

typedef struct _IOSResourceOpenRequest
{
    const u8 *path;
    IOSOpenMode flags;
    u32 uid; // some sort of ID (UID/PID?)
    u16 gid; // probably gid based on the SetGid syscall
} IOSResourceOpenRequest;

typedef struct _IOSResourceCloseRequest
{
} IOSResourceCloseRequest;

typedef struct _IOSResourceReadWriteRequest
{
    u8 *data;
    u32 length;
} IOSResourceReadWriteRequest;

typedef enum _SeekMode
{
    IOS_SEEK_SET = 0,
    IOS_SEEK_CUR = 1,
    IOS_SEEK_END = 2,
} SeekMode;

typedef struct _IOSResourceSeekRequest
{
    u32 offset;
    SeekMode mode;
} IOSResourceSeekRequest;

typedef struct _IOSResourceIOCtlRequest
{
    u32 request;
    u8 *in;
    u32 in_size;
    u8 *out;
    u32 out_size;
} IOSResourceIOCtlRequest;

typedef struct _IOVector
{
    u8 *base;
    u32 length;
} IOVector;

typedef struct _IOSResourceIOCtlVRequest
{
    u32 request;
    u32 in_count;
    u32 io_count;
    IOVector *vectors;
} IOSResourceIOCtlVRequest;

typedef union _IOSResourceArgs
{
    IOSResourceOpenRequest open;
    IOSResourceCloseRequest close;
    IOSResourceReadWriteRequest read;
    IOSResourceReadWriteRequest write;
    IOSResourceSeekRequest seek;
    IOSResourceIOCtlRequest ioctl;
    IOSResourceIOCtlVRequest ioctlv;
} IOSResourceArgs;

typedef struct _IOSRequest
{
    IPCCommandType cmd;
    s32 ret;
    u32 fd;
    IOSResourceArgs args;
} IOSRequest;

typedef struct _IOSMessage
{
    IOSRequest request;
    void *callback;
    u32 caller_data;
    u32 relaunch;
} IOSMessage;

#endif // _WII_IPC_H
#endif // RB3E_WII
