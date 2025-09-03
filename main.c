#include "logger/logger.h"
#include "stack/stack.h"

static void normalWork ()
{
    log_string ("<blu><b>Normal work test</b><dft>\n");

    StackHandler stack = stackInit (3, sizeof(int));

    stackDump (stack);

    int a = 0X11111111;
    stackPush (stack, &a);
    stackDump (stack);
    a = 0X22222222;
    stackPush (stack, &a);
    stackDump (stack);
    a = 0X33333333;
    stackPush (stack, &a);
    stackDump (stack);
    a = 0X44444444;
    stackPush (stack, &a);

    stackDump (stack);

    stackPop (stack, NULL);
    stackDump (stack);

    stackFree (stack);
    log_string ("<grn>TEST PASSED<dft>\n");
}

T3
(
static void randHandler ()
{
    log_string ("<blu><b>Random handler fault test</b><dft>\n");

    log_string("<ylw>Verify testing<dft>\n");
    stackVerify (rand());

    log_string("<ylw>Free testing<dft>\n");
    stackFree (rand());

    log_string("<ylw>Top testing<dft>\n");
    stackTop (rand(), NULL);
    
    log_string("<ylw>Push testing<dft>\n");
    stackPush (rand(), (int*)rand());

    log_string("<ylw>Pop testing<dft>\n");
    stackPop (rand(), NULL);

    log_string("<ylw>Len testing<dft>\n");
    stackLen (rand());

    log_string("<ylw>Dump testing<dft>\n");
    stackDump (rand());

    log_string ("<grn>TEST PASSED<dft>\n");
}
)

int main ()
{
    log_start ("stdout");

    normalWork ();

T3( randHandler (); )
}