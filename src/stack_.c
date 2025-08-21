#include "stack_.h"


void stackInit (Stack* dst, size_t numOfElem, size_t sizeOfElem)
{
    assertStrict (dst, "NULL received");

    dst->data = (char*) calloc (numOfElem, sizeOfElem);

    if (!dst->data)
    {
        IF_DBG
        (
            log_err ("internal error", "calloc returned NULL");
            IF_SAFE (exit (EXIT_FAILURE);)
        )
    }

    dst->top = dst->data;
    dst->sizeOfElem = sizeOfElem;
    dst->capacity = numOfElem;
    IF_DBG
    (
        dst->frontCanary = FRONTCANARY;
        dst->tailCanary = TAILCANARY;
    )
}

void stackFree (Stack* stack)
{
    assertStrict (stack && stack->data && stack->top, "NULL received or stack wasnt initialized");

    free (stack->data);
    stack->data = NULL;
    stack->top = NULL;
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

    if (stack->top - stack->data < stack->capacity * stack->sizeOfElem)
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

        #if STACSECURE
        memset (stack->top, 0XFF, stack->sizeOfElem);
        #endif
    }
}

size_t stackLen (const Stack* stack)
{
    assertStrict (stack && stack->data && stack->top, "NULL received or stack wasnt initialized");
    return (stack->top - stack->data) / stack->sizeOfElem;
}


void stackDump_ (const char* name, const Stack* stack, void (*print)(const void* obj) /* = txDump */)
{
    assertStrict (stack && stack->data && stack->top, "NULL received or stack wasnt initialized");

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