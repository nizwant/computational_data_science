#include <stdio.h>
#include <omp.h>

int main()
{
    int max_threads = omp_get_max_threads();
    printf("max_threads=%d\n", max_threads);

    //omp_get_num_threads

    int data[max_threads];
    for (int i=0; i<max_threads; ++i)
        data[i] = -1;

    double u = 0.0;
    int v=0;

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        data[thread_id] = thread_id;

        v = thread_id;

        //#pragma omp atomic

        #pragma omp critical
        {
            // only one thread allowed inside
            u = u+thread_id*thread_id;
        }
    }

    for (int i=0; i<max_threads; ++i)
        printf("%d", data[i]);
    printf("\nu=%f,v=%d\n", u, v);
    return 0;
}

// gcc lab10-openmp2.c -fopenmp && OMP_NUM_THREADS=10 ./a.out
