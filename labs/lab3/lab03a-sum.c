#include "lab03a-stuff.h"

double sum(int n)  // 0 .. n-1
{
    ASSERT(n > 0);
    double s = 0.0;
    for (int i=0; i<n; ++i)
        s += i;
    return s;
}

