#include "logger/logger.h"
#include "src/stack_.h"

int main ()
{
    log_start ("stdout");

    Stack stack = {0};
    stackInit (&stack, 3, sizeof (int));

    stackDump (&stack);

    int a = 0X11111111;
    stackPush (&stack, &a);
    a = 0X22222222;
    stackPush (&stack, &a);
    a = 0X33333333;
    stackPush (&stack, &a);
    a = 0X44444444;
    stackPush (&stack, &a);

    stackDump (&stack);

    stackPop (&stack);
    stackDump (&stack);

    stackFree (&stack);
}