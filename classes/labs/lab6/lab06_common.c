#include "lab06.h"

double square(double x)
{
    return x*x;
}

double mysum(const double* x, size_t n)
{
    double t = 0.0;
    for (size_t i=0; i<n; ++i)
        t += x[i];
    return t;
}
