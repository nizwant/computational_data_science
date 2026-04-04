# Lecture 4

## Makefiles


## static arrays

contiguous sequences of scalars of the same type

```c
int x[5];
```

size must be fixed, bc they are stack alloc'd

```c
int x[] = {1, 2, 3, 4, 5};   // size of 5
```

```c
int x[5] = {1, 2, 3};  // two last uninit'd
```


x[0] - first
x[1] - second
...
x[4]  - last


## Pointers

```c
int y = 1;
int *w;
w = &y;   // let w be the address of y
```

```c
int y, *w;
```

w - a pointer to (address of) an integer


```c
int x=0, y=1, *w, z=2;

printf("&x=%llu\n", (unsigned long long)&x);
printf("&y=%llu\n", (unsigned long long)&y);
printf("&w=%llu\n", (unsigned long long)&w);
printf("&z=%llu\n", (unsigned long long)&z);

w = &y;   // address of y
printf(" y=%d\n", y);
printf("*w=%d\n", *w);  // pointer dereference

*w = 6;
printf(" y=%d\n", y);

w = &x;
printf("*w=%d\n", *w);  // pointer dereference

printf("*(w-1)=%d\n", *(w-1));  // pointer dereference

//printf("%d\n", y);
```



pointer is an address in mem -- unsigned integer(!)

double z;
double* x = &z;
then *x is double
char* str;
etc.


(void*) --- any type

double*x;
(int*)((void*)x)

((void*)0) -- NULL pointer

#define NULL ((void*)0)
int* x = NULL;



void* x;
*x -- illegal

*((int*)x)


```c
#include <stdio.h>

void swap(int *a, int* b)
{
    int c = *a;
    *a = *b;
    *b = c;
}


int main()
{
    int x = 1, y = 2;
    swap(&x, &y);
    printf("%d %d\n", x, y); // 2 1
    return 0;
}

```


double pointers

double x;
double *w = &x;
double **p = &w;

useful for passing pointers to functions that can be modified outside

Function pointers

```
int square(int x)
{
    return x*x;
}
```

`square` === `&square`


```c
int (*kwadrat)(int) = square;
kwadrat(2);

double (*power)(double, int);
double z = power(3.4, 3)
```



// Exercise/Homework: implement the bisection method for finding the root of f, i.e., x: f(x)=0
// bisect(my_fun, -1.0, 1.0)
// double (*my_fun)(double)



dynamic shared libraries
calling shared lib funs from R
