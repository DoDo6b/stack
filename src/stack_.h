#ifndef STACK__H
#define STACK__H

#include <stdio.h>
#include "../logger/logger.h"
#include "../kassert/kassert.h"
#include "../useful/Macro.h"


#define STACSECURE 1
#define FRONTCANARY 0X8BADF00D0BADFEE1
#define TAILCANARY  0XDEADFA11DEFEC10D
#define HEXSPEAK    0XDEADDEADFA11B002

typedef struct
{
    IF_DBG (uintptr_t frontCanary;)
    char* top;
    char* data;
    size_t capacity;
    size_t sizeOfElem;
    IF_DBG (uintptr_t tailCanary;)
}Stack;

void stackInit (Stack* dst, size_t numOfElem, size_t sizeOfElem);

void stackFree (Stack* stack);

void stackTop (const Stack* stack, void* dst);

void stackPush (Stack* stack, void* src);

void stackPop_ (Stack* stack, void* dst);
#define stackPop(stack) stackPop_ (stack, NULL);

size_t stackLen (const Stack* stack);

void stackDump_ (const char* name, const Stack* stack, void (*print)(const void* obj));
#define stackDump(stack) stackDump_ (#stack, stack, NULL)

#endif