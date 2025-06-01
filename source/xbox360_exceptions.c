/*
    RB3Enhanced - xbox360_exceptions.c
    Low-level exception handling for RB3Enhanced on Xbox.
*/
#ifdef RB3E_XBOX

#include <xtl.h>
#include <stdio.h>
#include <string.h>
#include "exceptions.h"
#include "ppcasm.h"
#include "ports.h"
#include "rb3enhanced.h"
#include "xbox360.h"
#include "version.h"

static int hadException = 0;

static EXCEPTION_RECORD exceptionRecord;
static CONTEXT exceptionContext;
static BYTE exceptionHandlerStack[0x2000];

static LPWSTR buttons[1] = {L"Return to Dashboard"};
static MESSAGEBOX_RESULT result;
static XOVERLAPPED overlapped;
static void MessageBoxAndWait(wchar_t *text)
{
    if (XShowMessageBoxUI(XUSER_INDEX_ANY, L"RB3Enhanced Exception", text, 1, buttons, 0, XMB_ERRORICON, &result, &overlapped) == ERROR_IO_PENDING)
    {
        while (!XHasOverlappedIoCompleted(&overlapped))
            Sleep(50);
    }
}

static int didSuccessfullyWriteFile = 0;
static char exceptionOutputFilename[64];

static char *dataReadWriteNames[2] = { "read", "write" };

void DxRndSuspend(void *theDxRnd);

static void GraphicalExceptionDisplay()
{
    wchar_t exceptionText[1024] = {0};
    wchar_t exceptionFmtBuffer[128];
    wcscat(exceptionText, L"Rock Band 3 has crashed. If this happens again, report this to the RB3Enhanced developers!\n\n");
#define fmtExc(...) { wsprintfW(exceptionFmtBuffer, __VA_ARGS__); wcscat(exceptionText, exceptionFmtBuffer); }

    fmtExc(L"RB3Enhanced %S\n", RB3E_BUILDTAG);

    // print info about the exception
    if (exceptionRecord.ExceptionCode == STATUS_ACCESS_VIOLATION) {
        fmtExc(L"Data Error: %08X %08X %S\n", exceptionRecord.ExceptionAddress, exceptionRecord.ExceptionInformation[1], dataReadWriteNames[exceptionRecord.ExceptionInformation[0] & 1]);
    } else if (exceptionRecord.ExceptionCode == STATUS_ILLEGAL_INSTRUCTION) {
        fmtExc(L"Instruction Error: %08X\n", exceptionRecord.ExceptionAddress);
    } else {
        fmtExc(L"Error %X: %08X\n", exceptionRecord.ExceptionCode, exceptionRecord.ExceptionAddress);
    }

    // print some core registers - not worth having them all...
    fmtExc(L"R1:%08X R12:%08X R13:%08X\n", exceptionContext.Gpr1, exceptionContext.Gpr12, exceptionContext.Gpr13);
    fmtExc(L"PC:%08X LR:%08X MSR:%08X\n", exceptionContext.Iar, exceptionContext.Lr, exceptionContext.Msr);

    // walk the stack
    fmtExc(L"Stack: ");
    {
        int i = 0;
        DWORD *stackPtr = (DWORD *)exceptionContext.Gpr1;
        // sanity check: make sure our stack pointer is valid
        if (MmIsAddressValid((DWORD)stackPtr))
        {
            do {
                DWORD lr;
                stackPtr = (DWORD *)stackPtr[0];
                // break if the stack pointer isn't valid
                if (!MmIsAddressValid((DWORD)stackPtr))
                    break;
                lr = stackPtr[-2];
                // break if the LR isn't valid
                if (!MmIsAddressValid((DWORD)lr))
                    break;
                // print it out
                fmtExc(L"%08X ", lr);
                i++;
            } while (i < 7);
        }
    }
    fmtExc(L"\n");

    if (didSuccessfullyWriteFile)
    {
        char *exceptionOutputFilenameCut = exceptionOutputFilename;
        if (exceptionOutputFilenameCut[0] == 'R') // starts with RB3 so skip it
            exceptionOutputFilenameCut += 3;
        fmtExc(L"\nCrash dump saved to\n%S", exceptionOutputFilenameCut);
    }
    else
    {
        fmtExc(L"\nFailed to save crash dump.");
    }
#undef fmtExc
    // suspend TheDxRnd so XAM can render our error message
    DxRndSuspend((void *)PORT_DXRND);
    MessageBoxAndWait(exceptionText);
}

static int openExceptionFile = -1;
static uint16_t totalStackwalks = 0;
static uint16_t totalMemchunks = 0;
static uint32_t totalMemchunkData = 0;

static void WriteHeaderToFile(uint16_t num_stackwalk, uint16_t num_memchunks)
{
    rb3e_exception_header header;
    header.magic = EXCEPTIONPACK_HEADER_XBOX;
    header.version = 0;
    strncpy(header.rb3e_buildtag, RB3E_BUILDTAG, sizeof(header.rb3e_buildtag));
    strncpy(header.rb3e_commit, RB3E_BUILDCOMMIT, sizeof(header.rb3e_commit));
    header.num_stackwalk = num_stackwalk;
    header.num_memchunks = num_memchunks;
    // always write to the start of the file
    RB3E_WriteFile(openExceptionFile, 0, &header, sizeof(rb3e_exception_header));
}

static void WriteExceptionInfoToFile()
{
    // always comes after the header
    RB3E_WriteFile(openExceptionFile, sizeof(rb3e_exception_header) + 0, &exceptionRecord, sizeof(exceptionRecord));
    RB3E_WriteFile(openExceptionFile, sizeof(rb3e_exception_header) + sizeof(EXCEPTION_RECORD), &exceptionContext, EXCEPTION_CONTEXT_SIZE);
}

static void WriteMemChunkToFile(void *buffer, uint32_t length)
{
    rb3e_exception_memchunk chunkheader;
    // make sure the address is valid as well as the total range
    if (!MmIsAddressValid((DWORD)buffer) || !MmIsAddressValid((DWORD)buffer + length))
        return;
    // craft the header
    chunkheader.address = (uint32_t)buffer;
    chunkheader.length = length;
    // write the header to the file
    // oh geez that offset calculation is getting long im starting to regret not just having fwrite equivalent but we stay silly
    RB3E_WriteFile(openExceptionFile, sizeof(rb3e_exception_header) + sizeof(EXCEPTION_RECORD) + EXCEPTION_CONTEXT_SIZE +
        (totalStackwalks * sizeof(DWORD)) + (totalMemchunks * sizeof(rb3e_exception_memchunk)) + totalMemchunkData, &chunkheader, sizeof(rb3e_exception_memchunk));
    // increment our pointers
    totalMemchunks++;
    // for the sake of simplicity write data here
    RB3E_WriteFile(openExceptionFile, sizeof(rb3e_exception_header) + sizeof(EXCEPTION_RECORD) + EXCEPTION_CONTEXT_SIZE +
        (totalStackwalks * sizeof(DWORD)) + (totalMemchunks * sizeof(rb3e_exception_memchunk)) + totalMemchunkData, buffer, length);
    totalMemchunkData += length;
}

static void WriteStackWalkToFile()
{
    DWORD *stackPtr = (DWORD *)exceptionContext.Gpr1;
    // sanity check: make sure our stack pointer is valid
    if (MmIsAddressValid((DWORD)stackPtr))
    {
        do {
            DWORD lr;
            stackPtr = (DWORD *)stackPtr[0];
            // break if the stack pointer isn't valid
            if (!MmIsAddressValid((DWORD)stackPtr))
                break;
            lr = stackPtr[-2];
            // break if the LR isn't valid
            if (!MmIsAddressValid((DWORD)lr))
                break;
            // write the address out to the file
            RB3E_WriteFile(openExceptionFile, sizeof(rb3e_exception_header) + sizeof(EXCEPTION_RECORD) + EXCEPTION_CONTEXT_SIZE + (totalStackwalks * sizeof(DWORD)), &lr, sizeof(DWORD));
            totalStackwalks++;
        } while (TRUE);
    }
    // this code is ugly, but do that all again so we can actually dump out the contents of the stack as memchunks
    stackPtr = (DWORD *)exceptionContext.Gpr1;
    if (MmIsAddressValid((DWORD)stackPtr))
    {
        DWORD totalStackBytes = 0;
        DWORD lastPtr = (DWORD)stackPtr;
        do {
            DWORD length;
            // set an upper limit for the amount of stack we can dump
            if (totalStackBytes > 0x4000)
                break;
            stackPtr = (DWORD *)stackPtr[0];
            // break if the stack pointer isn't valid
            if (!MmIsAddressValid((DWORD)stackPtr))
                break;
            // calculate the length of this chunk of the stack and if its too long then dont bother
            length = (DWORD)stackPtr - lastPtr;
            if (length > 0x4000)
                break;
            // write the chunk out to the file
            WriteMemChunkToFile((void *)lastPtr, length);
            lastPtr = (DWORD)stackPtr;
            totalStackBytes += length;
        } while (TRUE);
    }
}

static void ExceptionWriteToFile()
{
    char exceptionFolder[32];
    char exceptionFilename[32];
    // get the path of rb3.ini, this is the folder we'll write the crash dump to
    char *configFilePath = RB3E_GetRawfilePath("rb3.ini", 1);
    // create the filename for the execption
    SYSTEMTIME sysTime;
    GetSystemTime(&sysTime);
    sprintf(exceptionFilename, "crash_%04d%02d%02d_%02d%02d%02d.exc", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
    // try to open the file
    if (configFilePath == NULL)
    {
        // try saving to the game directory first
        sprintf(exceptionOutputFilename, "GAME:\\%s", exceptionFilename);
        openExceptionFile = RB3E_OpenFile(exceptionOutputFilename, 1);
        // if that fails, try saving to the root of the HDD
        if (openExceptionFile == -1 && RB3E_Mounted)
        {
            sprintf(exceptionOutputFilename, "RB3HDD:\\%s", exceptionFilename);
            openExceptionFile = RB3E_OpenFile(exceptionOutputFilename, 1);
        }
    }
    else
    {
        // save to the path of rb3.ini, so remove it from the filename path
        char *rb3inipos = strstr(configFilePath, "rb3.ini");
        strncpy(exceptionFolder, configFilePath, rb3inipos - configFilePath);
        sprintf(exceptionOutputFilename, "%s%s", exceptionFolder, exceptionFilename);
        openExceptionFile = RB3E_OpenFile(exceptionOutputFilename, 1);
    }
    // if we couldn't create the new file then bail out
    if (openExceptionFile == -1)
    {
        didSuccessfullyWriteFile = 0;
        return;
    }
    // write the header and exception info
    WriteHeaderToFile(0, 0);
    WriteExceptionInfoToFile();
    // write the contents of the stack
    WriteStackWalkToFile();
    // write memory chunks for valid memory addresses we have in GPRs
    // TODO(Emma): dedupe
    {
        ULONGLONG *registers = &exceptionContext.Gpr0;
        int i = 0;
        for (i = 0; i < 32; i++)
        {
            DWORD reg32 = (DWORD)registers[i];
            if ((reg32 & 0xF0000000) == 0x40000000 || // heap memory
                (reg32 & 0xF0000000) == 0x70000000 || // stack memory
                (reg32 & 0xF0000000) == 0x80000000 || // 64K executable memory
                (reg32 & 0xF0000000) == 0x90000000) // 4K executable memory
            {
                if (MmIsAddressValid(reg32 & 0xFFFFFF00))
                {
                    WriteMemChunkToFile((void *)(reg32 & 0xFFFFFF00), 0x200);
                }
            }
        }
    }
    // TODO(Emma): write heap debugging information, and other useful trinkets
    // rewrite the header with the total stack walked
    WriteHeaderToFile(totalStackwalks, totalMemchunks);
    didSuccessfullyWriteFile = 1;
    // close the file and flush to disk
    RB3E_CloseFile(openExceptionFile);
}

static void ExceptionHandler()
{
    ExceptionWriteToFile();
    GraphicalExceptionDisplay();
    // XamLoaderTerminateTitle
    ((void(*)(void))PORT_XAMLOADERTERMINATETITLE)();
}

int RB3E_ExceptionHandler(EXCEPTION_RECORD *ExceptionRecord, void *EstablisherFrame, CONTEXT *ContextRecord, struct _DISPATCHER_CONTEXT *DispatcherContext)
{
    POKE_B(&DxRndSuspend, PORT_DXRND_SUSPEND);

    // if we've already had an exception, try to terminate title without displaying to the user
    if (hadException)
    {
        ContextRecord->Iar = PORT_XAMLOADERTERMINATETITLE;
        return 0;
    }

    // back up our exception record and stack
    memcpy(&exceptionRecord, ExceptionRecord, sizeof(EXCEPTION_RECORD));
    memcpy(&exceptionContext, ContextRecord, sizeof(CONTEXT));
    // change the thread to go to our exception handler
    ContextRecord->Iar = (DWORD)ExceptionHandler;
    // we change the stack too to avoid clobbering the original stack and make it viable to dump info from
    ContextRecord->Gpr1 = (DWORD)exceptionHandlerStack + sizeof(exceptionHandlerStack);
    hadException = 1;
    return 0; // continue execution
}

#endif
