#ifndef POLICY_H
#define POLICY_H


#define TOLERANCEPOLICY_LVL 9
#define SECURITY_LVL 9


#if SECURITY_LVL >= 1
    #define S1(...) __VA_ARGS__
#else
    #define S1(...)
#endif

#if TOLERANCEPOLICY_LVL >= 1
    #define T1(...) __VA_ARGS__
#else
    #define T1(...)
#endif

#if TOLERANCEPOLICY_LVL >= 2
    #define T2(...) __VA_ARGS__
#else
    #define T2(...)
#endif


#endif