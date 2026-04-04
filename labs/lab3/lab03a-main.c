#include "lab03a-stuff.h"


int main()
{
    double v;
    volatile int n = 10000000;  // volatile prevents the compiler from replacing sum(n) with the computed result

    printf("%s" ":" "%d\n", __FILE__, __LINE__);

    ASSERT(n > 0);

    double t_min  = INFINITY;
    double t_mean = 0.0;
    double t_max  = -INFINITY;

    const int niter = 100;
    for (int i=0; i<niter; ++i) {
        clock_t c0 = clock();
        v = sum(n);
        clock_t c1 = clock();
        double t = (c1-c0)/(double)CLOCKS_PER_SEC*1000.0;

        t_mean += t;
        if (t < t_min) t_min = t;
        if (t > t_max) t_max = t;
    }
    t_mean /= niter;  // t_mean = t_mean / (double)niter;

    printf("%s:%d\n", __FILE__, __LINE__);
    printf("%f\n", v);
    printf("min  time = %.3f ms\n", t_min);
    printf("mean time = %.3f ms\n", t_mean);
    printf("max  time = %.3f ms\n", t_max);

    ASSERT(t_mean < 4);

    return 0;  // EXIT_SUCCESS
}
