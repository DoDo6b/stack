#include "logger/logger.h"
#include "stack/stack.h"

static void normalWork ()
{
    log_string ("<blu><b>Normal work test</b><dft>\n");

    StackHandler stack = stackInitH (3, sizeof(int));

    stackDumpH (stack);

    int a = 0X11111111;
    stackPushH (stack, &a);
    a = 0X22222222;
    stackPushH (stack, &a);
    a = 0X33333333;
    stackPushH (stack, &a);
    a = 0X44444444;
    stackPushH (stack, &a);

    stackDumpH (stack);

    stackPopH (stack)
    stackDumpH (stack);

    stackFreeH (stack);
    log_string ("<grn>TEST PASSED<dft>\n");
}

static void randHandler ()
{
    log_string ("<blu><b>Random handler fault test</b><dft>\n");

    log_string("<ylw>Verify testing<dft>\n");
    stackVerifyH (rand());

    log_string("<ylw>Free testing<dft>\n");
    stackFreeH (rand());

    log_string("<ylw>Top testing<dft>\n");
    stackTopH (rand(), NULL);
    
    log_string("<ylw>Push testing<dft>\n");
    stackPushH (rand(), (int*)rand());

    log_string("<ylw>Pop testing<dft>\n");
    stackPopH (rand());

    log_string("<ylw>Len testing<dft>\n");
    stackLenH (rand());

    log_string("<ylw>Dump testing<dft>\n");
    stackDumpH (rand());

    log_string ("<grn>TEST PASSED<dft>\n");
}

int main ()
{
    log_start ("stdout");

    normalWork ();

    randHandler ();
}