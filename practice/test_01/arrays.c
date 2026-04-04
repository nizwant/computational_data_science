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
    int i = 0;
    while (i<*final_size){
        for (int num_repeats=0;num_repeats<k;num_repeats++){
            (*return_array)[i] = base_array[i/k];
            i++;
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

    return 0;
}