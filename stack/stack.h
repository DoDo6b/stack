#ifndef STACK_H
#define STACK_H


#include "Policy.h"
#if TOLERANCEPOLICY_LVL < 3
    #include "src/stack_.h"
#else
    #include "src/handlers.h"
#endif


#endif