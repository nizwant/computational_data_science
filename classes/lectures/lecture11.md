# Lecture 11 (2026-05-26)

## OpenMP

Open Multiprocessing

API for a portable multithreaded shared memory parallelism
implementing the fork-join model.

MPI - another library


```c
gcc lab10-openmp1.c -fopenmp && OMP_NUM_THREADS=10 ./a.out
omp_get_max_threads()  // maximal possible number of threads (OMP_NUM_THREADS)
omp_get_thread_num()   // current thread ID, where 0=master thread
omp_get_num_threads()  // actual number of threads spawned

#ifdef _OPENMP
#include <omp.h>
#endif

#pragma omp parallel
{
    // this block will be run in parallel

    #pragma omp critical
    {
        // critical section – one thread at a time
    }

    #pragma omp atomic
    a += b;  // "simple" instructions only - lightweight version of critical

    #pragma omp barrier
    // wait for all the threads
}
```



Quicksort, mergesort:
#pragma omp task


OpenMP support for GPUs

