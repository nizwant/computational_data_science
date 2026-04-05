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
    for (int i=0;i<array_size; i++) (*ordering_permutation)[i] = i;
    for (int i=0;i<array_size; i++){
        int biggest = base_array[(*ordering_permutation)[i]];
        int biggest_index = i;
        for (int j=i;j<array_size; j++){
            if (base_array[(*ordering_permutation)[j]] > biggest) {
                biggest = base_array[(*ordering_permutation)[j]];
                biggest_index = j;
            }
        }
        int temp = (*ordering_permutation)[i];
        (*ordering_permutation)[i] = (*ordering_permutation)[biggest_index];
        (*ordering_permutation)[biggest_index] = temp;
    }
}

void partition(int* array, int* array_size, int v, int** smaller, int** larger, int* smaller_size, int* larger_size){
    for (int i=0; i<*array_size; i++){
        if (array[i] > v) (*larger_size)++;
    }
    *smaller_size = *array_size - *larger_size;
    *larger = (int *)malloc(*larger_size * sizeof(int));
    *smaller = (int *)malloc(*smaller_size * sizeof(int));
    int larger_counter = 0;
    int smaller_counter = 0;
    for (int i=0; i<*array_size; i++){
        if (array[i] > v){
            (*larger)[larger_counter] = array[i];
            larger_counter++;
        }
        else {
            (*smaller)[smaller_counter] = array[i];
            smaller_counter++;
        }
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

    int* smaller;
    int* larger;
    int* size_pointer;
    int smaller_size, larger_size;
    size_pointer = &size;
    partition(to_sort, size_pointer, 3, &smaller, &larger, &smaller_size, &larger_size);

    printf("\n");

    for (int i=0; i<smaller_size; i++){
        printf("%d, ", smaller[i]);
    }

    printf("\n");

    for (int i=0; i<larger_size; i++){
        printf("%d, ", larger[i]);
    }

    free(array);
    free(return_array);
    free(ordering_permutation_array);
    free(smaller);
    free(larger);


    return 0;
}