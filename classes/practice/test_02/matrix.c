#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void print_matrix(double* A, int n, int m){
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            printf("%f ", A[n*j + i]);
        }

        printf("\n");
    }
}

double min_in_col(double *A, int n, int m, int col_num){
    if(col_num>= m) return NAN;

    double min = INFINITY;
    for(int i=0;i<n;i++){
        if(A[col_num*n + i] < min){
            min = A[col_num*n + i];
        }
    }

    return min;
}

double max_in_row(double *A, int n, int m, int row_num){
    if(row_num>n) return NAN;

    double max = - INFINITY;
    for (int i=0; i<m; i++){
        if(A[n*i + row_num] > max){
            max = A[n*i + row_num];
        }
    }

    return max;
}

void non_zero(double *A, int n, int m, int **rows, int **cols, int *size){
    int counter = 0;

    for(int i=0; i<(n*m);i++){
        if(A[i] != 0) counter++;
    }

    *rows = (int*)malloc(sizeof(int)*counter);
    *cols = (int*)malloc(sizeof(int)*counter);
    *size = counter;
    counter = 0;

    for (int j=0; j<m; j++){
        for(int i=0; i<n; i++){
            if(A[j*n + i] != 0){
                (*rows)[counter] = i;
                (*cols)[counter] = j;
                counter ++;
            }
        }
    }
}

void mat_mul(double *A, double *B, double **C, int size){
    *C = (double*)malloc(sizeof(double) * size * size);

    for (size_t i=0; i<size; i++){
        for (size_t j=0; j<size; j++){
            double sum = 0;
            for(size_t k=0; k<size; k++){
                sum += A[i + k*size] * B[j*size + k];
            }
            (*C)[i + j*size] = sum;
        }
    }
}


int main(){

    double *A = (double *)malloc(sizeof(double) * 4 * 3);
    for(int i=0; i<12; i++){
        A[i] = 2 * i - 1;
    }

    int n = 3;
    int m = 4;

    print_matrix(A, n, m);

    printf("%f, %f, %f\n", min_in_col(A,n,m,0), min_in_col(A,n,m,1), min_in_col(A,n,m,2));
    printf("%f, %f, %f\n", max_in_row(A,n,m,0), max_in_row(A,n,m,1), max_in_row(A,n,m,2));

    A[3] = 0;
    A[5] = 0;
    A[7] = 0;
    print_matrix(A, n, m);

    return 0;
}