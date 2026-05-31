#ifndef _OPENMP
#error "Compile wiht -fopenmp"
#endif

void square(const double* x, size_t n, double* y){
    int num_threads = omp_get_max_threads();

    int partition_size = n / num_threads;

    if(partition_size < 3000) num_threads = 1;

    #pragma omp parallel
    {
        int thread_number = omp_get_thread_num();
        for (size_t i=0; i<partition_size+10; i++){
            if (i + thread_number*partition_size < n){
                y[i + thread_number*partition_size] = x[i + thread_number*partition_size] * x[i + thread_number*partition_size];
            }
           
        }
    }
}

double mean(const double* x, size_t n){
    int num_threads = omp_get_max_threads();
    double sums[num_threads];
    for(int i=0; i<num_threads; i++) sums[i] = 0.0;
    
    #pragma omp parallel
    {
        int thread_number = omp_get_thread_num();

        #pragma omp for
        for (size_t i=0; i<n; i++) sums[thread_number] += x[i];
    }

    double total_sum = 0.0;
    for (int i=0; i<num_threads; i++){
        total_sum += sums[i];
    }

    return total_sum / n;
    
}


void normalise(const double* x, size_t n, double* y){

    double minimum = 1000000000.0;
    double maximum = -1000000000.0;

    #pragma omp parallel for reduction(min:minimum) reduction(max:maximum)
    for (size_t i = 0; i < n; i++) {
        if (minimum > x[i]){
            minimum = x[i];
        }
        if (maximum < x[i]){
            maximum = x[i];
        }
    }

    #pragma omp parallel for
    for (size_t i = 0; i < n; i++) {
        y[i] = (x[i] - minimum) / (maximum - minimum);
    }

}

