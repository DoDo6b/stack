#include "src/stack_.h"
#include "stack.h"

static Stack* Pointers[HANDLERSCAP] = {NULL};
static StackHandler FreeHandler = 0;


static StackHandler handlerAdd (Stack* ptr)
{
    assertStrict (ptr, "NULL received");
    assertStrict (FreeHandler < HANDLERSCAP, "HANDLERSCAP has been reached");
    assertStrict (Pointers[FreeHandler] == NULL, "internal error");

    StackHandler handler = FreeHandler;
    while (Pointers[++FreeHandler]);

    Pointers[handler] = ptr;

    return handler;
}

static Stack* handlerFree (StackHandler handler)
{
    assertStrict (handler < HANDLERSCAP, "invalid handler");

    if (Pointers[handler])
    {
        Stack* ptr = Pointers[handler];
        Pointers[handler] = NULL;
        if (FreeHandler > handler) FreeHandler = handler;
        return ptr;
    }
    else return NULL;
}

StackHandler stackInitH (size_t numOfElem, size_t sizeOfElem)
{
    Stack* ptr = (Stack*)malloc(sizeof(Stack));
    stackInit (ptr, numOfElem, sizeOfElem);
    return handlerAdd (ptr);
}

void stackFreeH (StackHandler handle)
{
    assertStrict (Pointers[handle], "invalid handler");
    stackFree ( handlerFree (handle) );
}

void stackTopH (StackHandler handle, void* dst)
{
    assertStrict (Pointers[handle], "invalid handler");
    stackTop (Pointers[handle], dst);
}

void stackPushH (StackHandler handle, const void* src)
{
    assertStrict (Pointers[handle], "invalid handler");
    stackPush (Pointers[handle], src);
}

void stackPopH_ (StackHandler handle, void* dst)
{
    assertStrict (Pointers[handle], "invalid handler");
    stackPop_ (Pointers[handle], dst);
}

size_t stackLenH (StackHandler handle)
{
    assertStrict (Pointers[handle], "invalid handler");
    return stackLen (Pointers[handle]);
}

void stackDumpH_ (const char* name, StackHandler handle, void (*print)(const void* obj))
{
    assertStrict (Pointers[handle], "invalid handler");
    stackDump_ (name, Pointers[handle], print);
}

uint64_t stackVerifyH_ (const char* callerFile, unsigned int callerLine, StackHandler handle)
{
    assertStrict (Pointers[handle], "invalid handler");
    return stackVerify_ (callerFile, callerLine, Pointers[handle]);
}