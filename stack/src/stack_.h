#ifndef STACK__H
#define STACK__H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "../../logger/logger.h"
#include "../../kassert/kassert.h"
#include "../../useful/Macro.h"

#include "../Policy.h"
#if TOLERANCEPOLICY_LVL >= 2
    #include "crc.h"
#endif

#include "errCodes.h"


const uint32_t FRONTCANARY = 0X8BADF00D;
const uint32_t TAILCANARY  = 0XDEFEC10D;
const uintptr_t HEXSPEAK   = 0XBADA110CBADFEE11;

typedef struct
{
T1( uint32_t frontCanary; )
    char* top;
    char* data;
    size_t capacity;
    size_t sizeOfElem;
T2( uint32_t crc32Data;)
T2( uint32_t crc32;)
T1( uint32_t tailCanary; )
}Stack;

void stackInit (Stack* dst, size_t numOfElem, size_t sizeOfElem);

void stackFree (Stack* stack);

void stackTop (const Stack* stack, void* dst);

void stackPush (Stack* stack, void* src);

void stackPop_ (Stack* stack, void* dst);
#define stackPop(stack) stackPop_ (stack, NULL);

size_t stackLen (const Stack* stack);

void    stackDump_ (const char* name, const Stack* stack, void (*print)(const void* obj));
#define stackDump(stack) stackDump_ (#stack, stack, NULL)

uint64_t stackVerify_ (const char* callerFile, unsigned int callerLine, const Stack* stack);
#define  stackVerify(stack) stackVerify_ (__FILE__, __LINE__, stack)


#endif