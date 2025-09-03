#include "stack_.h"


static size_t ceil (size_t a, size_t b)
{
    assertStrict (b != 0, "IDIOT division by zero");
    
    return (a + b - 1) / b;
}

T2
(
static void updateT2Hashes (Stack* dst)
{
    assertStrict (dst, "recived a NULL");
    assertStrict (dst->data, "stack wasnt initialized");

    dst->crc32Data  = crc32Calculate ((unsigned char*)dst->data, dst->capacity * dst->sizeOfElem);
    dst->crc32      = crc32Calculate ((unsigned char*)dst, &dst->crc32 - &dst->frontCanary);
}
)


Stack* stackInitD (size_t numOfElem, size_t sizeOfElem)
{
    Stack* dst = (Stack*)calloc(1, sizeof(Stack));

    assertStrict (dst, "calloc returned NULL");
    if (!dst) return NULL;
    assertStrict (numOfElem > 0 && sizeOfElem > 0, "cant allocate stack with capacity 0 or element size equal 0");

    size_t reservedMemory = 0 T1 ( + ceil (2 * sizeof (uintptr_t), sizeOfElem) + (numOfElem * sizeOfElem % sizeof (uintptr_t)) );
    dst->data = (char*) calloc (numOfElem + reservedMemory, sizeOfElem) T1 ( + sizeof (uintptr_t));
    assertStrict (dst->data, "calloc returned NUL");

    if (dst->data)
    {
        dst->top        = dst->data;
        dst->sizeOfElem = sizeOfElem;
        dst->capacity   = numOfElem;

T1      (
        dst->frontCanary = FRONTCANARY;
        dst->tailCanary  = TAILCANARY;

        uintptr_t* frontOffset = (uintptr_t*)dst->data - 1;
        *frontOffset = (uintptr_t) dst->data ^ HEXSPEAK;

        uintptr_t* tailOffset = (uintptr_t*)(dst->data + numOfElem * sizeOfElem + (numOfElem * sizeOfElem % sizeof (uintptr_t)) );
        *tailOffset = (uintptr_t)(dst->data + numOfElem * sizeOfElem) ^ HEXSPEAK;
        )
T2(     updateT2Hashes (dst); )

        assertStrict (stackVerifyD (dst) == 0, "verification failed, cant continue");

        return dst;
    }

    else
    {
        free (dst);
        return NULL;
    }
}

static void stackReallocD (Stack* stack)
{
    assertStrict (stackVerifyD (stack) == 0, "verification failed, cant continue");
    
    size_t reservedMemory = 0 T1 ( + ceil (2 * sizeof (uintptr_t), stack->sizeOfElem) + (stack->capacity * stack->sizeOfElem % sizeof (uintptr_t)) );
    size_t newSize = (stack->capacity * (GROWTHRATE) + reservedMemory) * stack->sizeOfElem;
    size_t topOffset = stack->top - stack->data;

    char* newBlock = (char*)realloc (stack->data T1 ( - sizeof (uintptr_t) ), newSize);
    assertStrict (newBlock, "realloc returned NULL");
    if (!newBlock) return;

    stack->data = newBlock T1 ( + sizeof (uintptr_t) );
    memset (stack->data + stack->capacity * stack->sizeOfElem, 0, ((GROWTHRATE) - 1) * stack->capacity * stack->sizeOfElem);
    stack->capacity *= (GROWTHRATE);
    stack->top = stack->data + topOffset;

T1  (
    uintptr_t* frontOffset = (uintptr_t*)stack->data - 1;
    *frontOffset = (uintptr_t) stack->data ^ HEXSPEAK;

    uintptr_t* tailOffset = (uintptr_t*)(stack->data + stack->capacity * stack->sizeOfElem + (stack->capacity * stack->sizeOfElem % sizeof (uintptr_t)) );
    *tailOffset = (uintptr_t)(stack->data + stack->capacity * stack->sizeOfElem) ^ HEXSPEAK;
    )
T2( updateT2Hashes (stack); )

    assertStrict (stackVerifyD (stack) == 0, "verification failed, cant continue");
}

void stackFreeD (Stack* stack)
{
    assertStrict (stackVerifyD (stack) == 0, "verification failed, cant continue");
T1( stack->data -= sizeof (uintptr_t); )

S1( memset (stack->data, 0XCC, stack->capacity * stack->sizeOfElem); )

    free (stack->data);

    memset (stack, 0xCC, sizeof (Stack));
    free (stack);
}


void stackTopD (const Stack* stack, void* dst)
{
    assertStrict (stackVerifyD (stack) == 0, "verification failed, cant continue");
    
    memcpy (dst, stack->top - stack->sizeOfElem, stack->sizeOfElem);
}

void stackPushD (Stack* stack, const void* src)
{
    assertStrict (stackVerifyD (stack) == 0, "verification failed, cant continue");
    assertStrict (src, "received a NULL");

    if ((size_t)(stack->top - stack->data) >= stack->capacity * stack->sizeOfElem) stackReallocD (stack);
    if ((size_t)(stack->top - stack->data) <  stack->capacity * stack->sizeOfElem)
    {
        memcpy (stack->top, src, stack->sizeOfElem);
        stack->top += stack->sizeOfElem;

T2(     updateT2Hashes (stack); )

        assertStrict (stackVerifyD (stack) == 0, "verification failed, cant continue");
    }
}

void stackPopD_ (Stack* stack, void* dst /* = NULL */)
{
    assertStrict (stackVerifyD (stack) == 0, "verification failed, cant continue");

    if (stack->top - stack->sizeOfElem >= stack->data)
    {
        stack->top -= stack->sizeOfElem;

        if (dst) memcpy (dst, stack->top, stack->sizeOfElem);

S1(     memset (stack->top, 0XCC, stack->sizeOfElem); )
T2(     updateT2Hashes (stack); )

        assertStrict (stackVerifyD (stack) == 0, "verification failed, cant continue");
    }
}

size_t stackLenD (const Stack* stack)
{
    assertStrict (stackVerifyD (stack) == 0, "verification failed, cant continue");
    return (stack->top - stack->data) / stack->sizeOfElem;
}


void stackDumpD_ (const char* name, const Stack* stack, void (*print)(const void* obj))
{
    if (stackVerifyD (stack) != 0)
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


uint64_t stackVerifyD_ (const char* callerFile, unsigned int callerLine, const Stack* stack)
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

T2  ( 
    if (stack->crc32 != crc32Calculate ((const unsigned char*)stack, &stack->crc32 - &stack->frontCanary))
    {
        log_string
        (
            "%s:%d: %s: <b><red>verification error:<dft> crc32 hash of struct has changed</b>\n",
            callerFile,
            callerLine,
            __func__
        );
        selfTestingCode |= CRCMAIN_HASCHANGED;
    }
    )

T1  (
    uintptr_t* frontOffset = (uintptr_t*)stack->data - 1;
    uintptr_t* tailOffset = (uintptr_t*)(stack->data + stack->capacity * stack->sizeOfElem + (stack->capacity * stack->sizeOfElem % sizeof (uintptr_t)) );

    if ( *frontOffset != ((uintptr_t) stack->data ^ HEXSPEAK) || *tailOffset != ((uintptr_t)(stack->data + stack->capacity * stack->sizeOfElem) ^ HEXSPEAK) )
    {
        log_string
        (
            "%s:%d: %s: <b><red>verification error:<dft> data signes was corrupted</b>\n",
            callerFile,
            callerLine,
            __func__
        );
        selfTestingCode |= DATABLOCK_SIGNES_CORRUPTED;
    }
    )

T2  (
    if (stack->crc32Data != crc32Calculate ((const unsigned char*)stack->data, stack->capacity * stack->sizeOfElem))
    {
        log_string
        (
            "%s:%d: %s: <b><red>verification error:<dft> crc32 hash of data has changed</b>\n",
            callerFile,
            callerLine,
            __func__
        );
        selfTestingCode |= CRCDATA_HASHCHANGED;
    }
    )


    if (selfTestingCode != 0)
    {
        log_err ("verification error", "failed with code: %llu", selfTestingCode);
    }
    return selfTestingCode;
}