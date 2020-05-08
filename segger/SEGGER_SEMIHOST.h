/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************
*                                                                    *
*            (c) 2019 SEGGER Microcontroller GmbH                    *
*                                                                    *
*       www.segger.com     Support: support@segger.com               *
*                                                                    *
**********************************************************************
*                                                                    *
* All rights reserved.                                               *
*                                                                    *
* Redistribution and use in source and binary forms, with or         *
* without modification, are permitted provided that the following    *
* condition is met:                                                  *
*                                                                    *
* o Redistributions of source code must retain the above copyright   *
*   notice, this condition and the following disclaimer.             *
*                                                                    *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND             *
* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,        *
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF           *
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE           *
* DISCLAIMED. IN NO EVENT SHALL SEGGER Microcontroller BE LIABLE FOR *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR           *
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT  *
* OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;    *
* OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF      *
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT          *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE  *
* USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH   *
* DAMAGE.                                                            *
*                                                                    *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : SEGGER_SEMIHOST.h
Purpose : Semihosting implementation header file.
*/
#include "stdarg.h"

#ifndef SEGGER_SEMIHOST_H // Avoid multiple inclusion.
#define SEGGER_SEMIHOST_H

#if defined(__cplusplus)
extern "C" { // Make sure we have C-declarations in C++ programs.
#endif

/*********************************************************************
 *
 *       Types, global
 *
 **********************************************************************
 */
typedef union {
    void *pV;
    const void *cpV;
    char *pC;
    const char *cpC;
    int I;
} SEGGER_SEMIHOST_PARA;

/*********************************************************************
 *
 *       Defines, fixed
 *
 **********************************************************************
 */
//
// File handles for standard streams
//
#define SEGGER_SEMIHOST_STDIN 0
#define SEGGER_SEMIHOST_STDOUT 1
#define SEGGER_SEMIHOST_ERROUT 2
//
// File modes for SYS_OPEN
//
#define SYS_FILE_MODE_READ 0       // Open the file for reading "r"
#define SYS_FILE_MODE_READBINARY 1 // Open the file for reading "rb"
#define SYS_FILE_MODE_READWRITE 2  // Open the file for reading and writing "r+"
#define SYS_FILE_MODE_READWRITEBINARY                                          \
    3 // Open the file for reading and writing "r+"
#define SYS_FILE_MODE_WRITE                                                    \
    4 // Open and truncate or create the file for writing "w"
#define SYS_FILE_MODE_WRITEBINARY                                              \
    5 // Open and truncate or create the file for writing "wb"
#define SYS_FILE_MODE_WRITEREAD                                                \
    6 // Open and truncate or create the file for writing and reading "w+"
#define SYS_FILE_MODE_WRITEREADBINARY                                          \
    7 // Open and truncate or create the file for writing and reading "w+b"
#define SYS_FILE_MODE_APPEND 8       // Open or create the file for writing "a"
#define SYS_FILE_MODE_APPENDBINARY 9 // Open or create the file for writing "ab"
#define SYS_FILE_MODE_APPENDREAD                                               \
    10 // Open or create the file for writing and reading "a+"
#define SYS_FILE_MODE_APPENDREADBINARY                                         \
    11 // Open or create the file for writing and reading "a+b"

//
// Semihosting operations,
// compatible to operations defined by ARM
//
#define SYS_OPEN 0x01
#define SYS_CLOSE 0x02
#define SYS_WRITEC 0x03
#define SYS_WRITE0 0x04
#define SYS_WRITE 0x05
#define SYS_READ 0x06
#define SYS_READC 0x07
#define SYS_ISERROR 0x08
#define SYS_ISTTY 0x09
#define SYS_SEEK 0x0A

#define SYS_FLEN 0x0C
#define SYS_TMPNAME 0x0D
#define SYS_REMOVE 0x0E
#define SYS_RENAME 0x0F
#define SYS_CLOCK 0x10
#define SYS_TIME 0x11
#define SYS_SYSTEM 0x12
#define SYS_ERRNO 0x13

#define SYS_GET_CMDLINE 0x15
#define SYS_HEAPINFO 0x16

#define SYS_EXIT 0x18 // ARM's operation name: angel_SWIreason_ReportException

#define SYS_ELAPSED 0x30
#define SYS_TICKFREQ 0x31
//
// SEGGER Extensions
//
#define SYS_IS_CONNECTED 0x00
#define SYS_WRITEF 0x40

/*********************************************************************
 *
 *       API functions / Function prototypes
 *
 **********************************************************************
 */
int SEGGER_SEMIHOST_Open(const char *sFilename, int Mode, int LenFilename);
int SEGGER_SEMIHOST_Close(int hFile);
int SEGGER_SEMIHOST_WriteC(char c);
int SEGGER_SEMIHOST_Write0(const char *s);
int SEGGER_SEMIHOST_Write(int hFile, const char *pBuffer, int NumBytesToWrite);
int SEGGER_SEMIHOST_Writef(const char *pFormat, va_list *pArg);
int SEGGER_SEMIHOST_Read(int hFile, char *pBuffer, int NumBytesToRead);
int SEGGER_SEMIHOST_ReadC(void);
int SEGGER_SEMIHOST_IsTTY(int hFile);
int SEGGER_SEMIHOST_Seek(int hFile, int Pos);
int SEGGER_SEMIHOST_FLen(int hFile);
int SEGGER_SEMIHOST_TmpName(char *pBuffer, int hFile, int pNumBytesName);
int SEGGER_SEMIHOST_Remove(const char *pPath, int NumBytesPath);
int SEGGER_SEMIHOST_Rename(const char *pFileName, int NumBytesFileName,
                           const char *pNewName, int NumBytesNewName);
int SEGGER_SEMIHOST_Clock(void);
int SEGGER_SEMIHOST_Time(void);
int SEGGER_SEMIHOST_System(const char *pCommand, int NumBytesCommand);
int SEGGER_SEMIHOST_Errno(void);
int SEGGER_SEMIHOST_GetCmdLine(char *pBuffer, char **psCmdLine,
                               int *pNumBytesCmdLine);
int SEGGER_SEMIHOST_Elapsed(unsigned long long *pTicks);
int SEGGER_SEMIHOST_HeapInfo(char *pDataBlock);
int SEGGER_SEMIHOST_TickFreq(void);
int SEGGER_SEMIHOST_Exit(int ExitCode);
int SEGGER_SEMIHOST_IsConnected(void);

int SEGGER_SEMIHOST_X_Request(int Op, SEGGER_SEMIHOST_PARA *pPara);

#if defined(__cplusplus)
} // Make sure we have C-declarations in C++ programs.
#endif

#endif // Avoid multiple inclusion.

/*************************** End of file ****************************/
