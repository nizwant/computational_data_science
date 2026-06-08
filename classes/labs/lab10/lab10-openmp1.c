#include <stdio.h>
#include <omp.h>

int main()
{
    int max_threads = omp_get_max_threads();
    printf("max_threads=%d\n", max_threads);

    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        //printf("thread_id=%d\n", thread_id);
        for (int i=0; i<100; ++i)
            printf("%c", 'A'+thread_id);
    }
    printf("\n");
    return 0;
}

// gcc lab10-openmp1.c -fopenmp && OMP_NUM_THREADS=10 ./a.out
