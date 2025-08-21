#include "stack_.h"


void stackInit (Stack* dst, size_t numOfElem, size_t sizeOfElem)
{
    assertStrict (dst, "NULL received");
    assertStrict (numOfElem > 0 && sizeOfElem > 0, "cant allocate stack with capacity 0 or element size equal 0");

    dst->data = (char*) calloc (numOfElem T1 ( + (2 * sizeof (uintptr_t) + sizeOfElem - 1) / sizeOfElem ), sizeOfElem) T1 ( + sizeof (uintptr_t));

    if (dst->data)
    {
        dst->top        = dst->data;
        dst->sizeOfElem = sizeOfElem;
        dst->capacity   = numOfElem;

T1      (
        dst->frontCanary = FRONTCANARY;
        dst->tailCanary  = TAILCANARY;

        *((uintptr_t*)dst->data - 1) = (uintptr_t)dst->data ^ HEXSPEAK;
        *(uintptr_t*)(dst->data + numOfElem * sizeOfElem) = (uintptr_t)(dst->data + numOfElem * sizeOfElem) ^ HEXSPEAK;
        )
    }
    else
    {
        IF_DBG
        (
            log_err ("internal error", "calloc returned NULL");
            IF_SAFE (exit (EXIT_FAILURE);)
        )

        memset (dst, 0, sizeof (Stack));
    }
}

void stackFree (Stack* stack)
{
    assertStrict (stack && stack->data && stack->top, "NULL received or stack wasnt initialized");
T2( stack->data -= sizeof (uintptr_t); )

S1( memset (stack->data, 0XCC, stack->capacity * stack->sizeOfElem); )

    free (stack->data);
    memset (stack, 0, sizeof (Stack));
}


void stackTop (const Stack* stack, void* dst)
{
    assertStrict (stack && stack->data && stack->top, "NULL received or stack wasnt initialized");
    assertStrict (dst,                                "NULL received");
    
    memcpy (dst, stack->top - stack->sizeOfElem, stack->sizeOfElem);
}

void stackPush (Stack* stack, void* src)
{
    assertStrict (stack && stack->data && stack->top, "NULL received or stack wasnt initialized");
    assertStrict (src,                                "NULL received");

    if ((size_t)(stack->top - stack->data) < stack->capacity * stack->sizeOfElem)
    {
        memcpy (stack->top, src, stack->sizeOfElem);
        stack->top += stack->sizeOfElem;
    }
}

void stackPop_ (Stack* stack, void* dst /* = NULL */)
{
    assertStrict (stack && stack->data && stack->top, "NULL received or stack wasnt initialized");

    if (stack->top - stack->sizeOfElem >= stack->data)
    {
        stack->top -= stack->sizeOfElem;

        if (dst) memcpy (dst, stack->top, stack->sizeOfElem);

S1(     memset (stack->top, 0XCC, stack->sizeOfElem); )
    }
}

size_t stackLen (const Stack* stack)
{
    assertStrict (stack && stack->data && stack->top, "NULL received or stack wasnt initialized");
    return (stack->top - stack->data) / stack->sizeOfElem;
}


void stackDump_ (const char* name, const Stack* stack, void (*print)(const void* obj))
{
    if(stackVerify (stack) != 0)
    {
        log_err ("internal error", "verification failed, cant continue");
        return;
    }

    log_string
    (
        "<blu>%s dump:<dft>\n"
        "capacity: %zu elem(s),\n"
        "real size %zu byte(s)\n"
        "{\n",
        name,
        stack->capacity,
        stack->top - stack->data
    );
    for (size_t i = 0; stack->data + i <= stack->top; i += stack->sizeOfElem)
    {
        log_string ("  [%zu]: ", i / stack->sizeOfElem);
        if (print) print (stack->data + i);
        else memDump (stack->data + i, stack->sizeOfElem);
    }

    log_string ("}\n");
}


uint64_t stackVerify_ (const char* callerFile, unsigned int callerLine, const Stack* stack)
{
    uint64_t selfTestingCode = 0;

    if (stack == NULL)
    {
        log_string
        (
            "%s:%d: %s: <b><red>verification error:<dft> received a NULL</b>\n",
            callerFile,
            callerLine,
            __func__
        );
        selfTestingCode |= RECIVED_NULL;
        log_err ("verification error", "failed with code: %llu", selfTestingCode);
        return selfTestingCode;
    }

T1  (
    if (stack->frontCanary != FRONTCANARY || stack->tailCanary != TAILCANARY)
    {
        log_string
        (
            "%s:%d: %s: <b><red>verification error:<dft> main signes was corrupted or its not a Stack</b>\n",
            callerFile,
            callerLine,
            __func__
        );
        selfTestingCode |= MAIN_SIGNES_CORRUPTED;
        log_err ("verification error", "failed with code: %llu", selfTestingCode);
        return selfTestingCode;
    }
    )

    if (stack->data == NULL || stack->top == NULL || stack->sizeOfElem == 0 || stack->capacity == 0)
    {
        log_string
        (
            "%s:%d: %s: <b><red>verification error:<dft> stack wasnt initilized</b>\n",
            callerFile,
            callerLine,
            __func__
        );
        selfTestingCode |= NOT_INITILIZED;
        log_err ("verification error", "failed with code: %llu", selfTestingCode);
        return selfTestingCode;
    }

    if (stack->top < stack->data || stack->top > stack->data + stack->capacity * stack->sizeOfElem || (stack->top - stack->data) % stack->sizeOfElem != 0)
    {
        log_string
        (
            "%s:%d: %s: <b><red>verification error:<dft> top pointer is in am impossible position</b>\n",
            callerFile,
            callerLine,
            __func__
        );
        selfTestingCode |= TOPPTR_OUTOFBOUNDS;
    }


    if (selfTestingCode != 0)
    {
        log_err ("verification error", "failed with code: %llu", selfTestingCode);
    }
    return selfTestingCode;
}