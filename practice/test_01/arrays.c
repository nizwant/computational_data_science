#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int* create_array(int size){
    int* array = (int *)malloc(size * sizeof(int));
    for (int i=0; i<size; i++){
        array[i] = rand();
    }
    return array;
}

void repeat_each(int* base_array, int base_array_size, int k, int** return_array, int* final_size){
    *final_size = k * base_array_size;
    *return_array = (int *)malloc(*final_size * sizeof(int));
    for (int i = 0; i < *final_size; i++) {
        (*return_array)[i] = base_array[i / k];
    }
}

void ordering_permutation(int* base_array, int** ordering_permutation, int array_size){
    *ordering_permutation = malloc(array_size * sizeof(int));
    for (int i=0;i<array_size; i++) *ordering_permutation[i] = i;
    for (int i=0;i<array_size; i++){
        int biggest = base_array[i];
        int biggest_index = i;
        for (int j=i;j<array_size; j++){
            if (base_array[j] > biggest) {
                biggest = base_array[j];
                biggest_index = j;
            }
        }
        *ordering_permutation[i] = biggest_index;
        *ordering_permutation[biggest_index] = i;
    }
}


int main(){
    srand(time(NULL));
    int array_size = 5;
    int* array = create_array(array_size);

    for (int i=0; i<array_size; i++){
        printf("%d, ", array[i]);
    }

    int final_size;
    int* return_array;

    int myNumbers[] = {1,2,3};
    repeat_each(myNumbers, 3, 4, &return_array, &final_size);

    printf("\n\n%d\n", final_size);
    for (int i=0; i<final_size; i++){
        printf("%d, ", return_array[i]);
    }

    printf("\n\n\n");

    int to_sort[] = {2,1,3,7,27,21,-1,-20,3};
    int size = *(&to_sort + 1) - to_sort;
    int* ordering_permutation_array;
    ordering_permutation(to_sort, &ordering_permutation_array, size);

    for (int i=0; i<size; i++){
        printf("%d, ", to_sort[i]);
    }

    printf("\n");

    for (int i=0; i<size; i++){
        printf("%d, ", ordering_permutation_array[i]);
    }

    free(array);
    free(return_array);
    free(ordering_permutation_array);


    return 0;
}