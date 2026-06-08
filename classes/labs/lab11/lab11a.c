#ifndef _OPENMP
#error "Compile with -fopenmp"
#endif

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN(a,b) (((a)<(b))?a:b)
void square(const double* x, size_t n, double* y)
{
    // #pragma omp parallel for
    // for (size_t i=0; i<n; ++i)
    //    y[i] = x[i]*x[i];

    // #pragma omp parallel
    // {
    //    #pragma omp for
    //    for (size_t i=0; i<n; ++i)
    //       y[i] = x[i]*x[i];
    //  }

    #pragma omp parallel
    {
        int k = omp_get_num_threads();
        int t = omp_get_thread_num();
        // for (size_t i=t; i<n; i+=k) bad locality of reference ;/

        //for (size_t i=(t*n)/k; i<((t+1)*n)/k; ++i)


        int B = n/k;
        for (size_t i=t*B; i<((t<k-1)?(t+1)*B:n); ++i)
            y[i] = x[i]*x[i];
    }
}
/*
double mean(const double* x, size_t n)
{
    int K = omp_get_max_threads();
    double sums[K];
    for (int i=0; i<K; ++i) sums[i] = 0.0;

    #pragma omp parallel
    {
        int k = omp_get_num_threads();
        int t = omp_get_thread_num();
        #pragma omp for
        for (size_t i=0; i<n; ++i) {
            sums[t] += x[i];
        }
    }

    double sum = 0.0;
    for (int i=0; i<K; ++i) sum += sums[i];
    return sum/(double)n;
}*/


// double mean(const double* x, size_t n)
// {
//     double sum = 0.0;
//
//     #pragma omp parallel
//     {
//         double mysum = 0.0;
//         #pragma omp for
//         for (size_t i=0; i<n; ++i) {
//             mysum += x[i];
//         }
//
//         #pragma omp atomic
//         sum += mysum;
//     }
//
//     return sum/(double)n;
// }


double mean(const double* x, size_t n)
{
    double suma = 0.0;

    #pragma omp parallel for reduction(+:suma)
    for (size_t i=0; i<n; ++i)
        suma += x[i];

    return suma/(double)n;
}

void normalise(const double* x, size_t n, double* y)
{
    //y[i] = (x[i]-min(x))/(max(x)-min(x));
    double cmin = x[n-1];
    double cmax = x[n-1];
    #pragma omp parallel
    {
        #pragma omp for reduction(min:cmin) reduction(max:cmax)
        for (size_t i=0; i<n-1; ++i)
            if (x[i] < cmin) cmin = x[i];
            else if (x[i] > cmax) cmax = x[i];

        //#pragma omp barrier  //redundant

        #pragma omp for
        for (size_t i=0; i<n; ++i)
            y[i] = (x[i]-cmin)/(cmax-cmin);
    }
}

int main()
{
    size_t n = 1000000000;
    double *x = malloc(sizeof(double)*n);
    for (size_t i=0; i<n; ++i) x[i] = i/(double)n;
    double *y = malloc(sizeof(double)*n);
    double t0 = omp_get_wtime();  // "wall" time
    //square(x, n, y);
    normalise(x, n, y);
    //double m = mean(x, n);
    double t1 = omp_get_wtime();
    //printf("mean=%f\n", m);
    printf("time=%f s\n", t1-t0);
    return 0;
}
// gcc lab11a.c -fopenmp -O3 -march=native && OMP_NUM_THREADS=10 ./a.out



