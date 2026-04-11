#include <stdio.h>

double square(double x){
    return x * x - 2;
}

double cube(double x){
    return x * x * x - 2;
}

double bisection(double (*fun)(double), double start, double stop){
    
    while (stop - start > 0.000001){
        double mid = (start + stop) / 2;
        if (fun(start) * fun(mid) < 0){
            stop = mid;
        }
        else{
            start = mid;
        }
    }
    return start;
}

int main(){
    printf("Hello world\n");
    printf("%f\n", bisection(square, 0 ,10));
    printf("%f\n", bisection(cube, 0 ,10));
    return 0;
}