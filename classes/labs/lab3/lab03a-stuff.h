#ifndef LAB03A_STUFF_H_SEEN
#define LAB03A_STUFF_H_SEEN

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#define STR(x) #x  /* stringisation */
#define XSTR(x) STR(x)

#define PI 3.14

#ifndef NDEBUG
#define ASSERT(EXPR) do { \
    if (!(EXPR)) { \
        fprintf(stderr, "Assertion " #EXPR " failed in "\
            __FILE__ ":" XSTR(__LINE__) "\n"); \
        exit(1); \
    } \
} while(0)
#else
#define ASSERT(EXPR) do { } while(0)
#endif


double sum(int n);


#endif
