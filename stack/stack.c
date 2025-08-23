#include "src/stack_.h"
#include "stack.h"

static StackHandler Handlers[HANDLERSCAP] = {0};

static bool handlerFind (StackHandler handler)
{
    assertStrict (handler != 0, "NULL received");

    for (size_t i = 0; i < HANDLERSCAP; i++)
    {
        if (Handlers[i] == handler)
        {
            return true;
        }
    }
    return false;
}

static void handlerAdd (StackHandler handler)
{
    assertStrict (handler != 0, "NULL received");

    for (size_t i = 0; i < HANDLERSCAP; i++)
    {
        if (Handlers[i] == 0)
        {
            Handlers[i] = handler;
            return;
        }
    }
}

static bool handlerFree (StackHandler handler)
{
    assertStrict (handler != 0, "NULL received");

    for (size_t i = 0; i < HANDLERSCAP; i++)
    {
        if (Handlers[i] == handler)
        {
            Handlers[i] = 0;
            return true;
        }
    }
    return false;
}

StackHandler stackInitH (size_t numOfElem, size_t sizeOfElem)
{
    Stack* ptr = (Stack*)malloc(sizeof(Stack));
    stackInit (ptr, numOfElem, sizeOfElem);
    handlerAdd ( (StackHandler)ptr );
    return (StackHandler)ptr;
}

void stackFreeH (StackHandler handle)
{
    assertStrict (handlerFree (handle), "handler doesnt exists");
    stackFree ((Stack*)handle);
}

void stackTopH (StackHandler handle, void* dst)
{
    assertStrict (handlerFind (handle), "handler doesnt exists");
    stackTop ((Stack*)handle, dst);
}

void stackPushH (StackHandler handle, void* src)
{
    assertStrict (handlerFind (handle), "handler doesnt exists");
    stackPush ((Stack*)handle, src);
}

void stackPopH_ (StackHandler handle, void* dst)
{
    assertStrict (handlerFind (handle), "handler doesnt exists");
    stackPop_ ((Stack*)handle, dst);
}

size_t stackLenH (StackHandler handle)
{
    assertStrict (handlerFind (handle), "handler doesnt exists");
    return stackLen ((Stack*)handle);
}

void stackDumpH_ (const char* name, StackHandler handle, void (*print)(const void* obj))
{
    assertStrict (handlerFind (handle), "handler doesnt exists");
    stackDump_ (name, (Stack*)handle, print);
}

uint64_t stackVerifyH_ (const char* callerFile, unsigned int callerLine, StackHandler handle)
{
    assertStrict (handlerFind (handle), "handler doesnt exists");
    return stackVerify_ (callerFile, callerLine, (Stack*)handle);
}