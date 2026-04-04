# Lecture 5

## Array pointers

double x[5];  // static array

x[0]...x[4]

contiguous array

for (i...) printf("%llu", (unsigned long long)&x[i]);

100
108
116
124
132

C-contiguous array

&x[0] - pointer to the start of the array

*(&x[0]+i)   + sizeof(double)*i bytes
x[i]

&x[i] == &x[0] + i == x+i

x == &x[0]

x[i] == i[x] == *(x+i) == *(i+x)

x[0] == *x

```c
double sum(const double* x, const size_t n)
{
    double t = 0.0;
    for (size_t i=0; i<n; ++i)
        // t += x[i];
        t += *(x++);
    return t;
}

double sum2(const double* xstart, const double* xstop)
{
    double t = 0.0;
    // n = (xstop-xstart)/sizeof(double);
    while (xstart != xstop)
        t += *(xstart++);
    return t;
}

//  const double* x   -- x[i] -- const double
//  double* const x   -- x[i] -- double
//  const double* const x

int main()
{
    double x[5];
    sum(x, 5);
    sum2(x, x+5);
    return 0;
}


```

## Dynamic memory allocation


<stdlib.h> has:

void* malloc(size_t size);  // allocates `size` bytes and returns the start address

allocates on the heap, which persists between function calls

void* - any pointer

int n = 1000;
double* x = (double*)malloc(n*sizeof(double));

x[0]....x[n-1]
sum(x, n)

// heap

// void free(void* ptr);

free(x);

// avoid memory leaks!




int x = (int)3.14;




```python
x = np.zeros(34343)
...
x = "gdsgS"
```


# `typedef`

typedef int myint;
myint x = 4;

#ifndef HAS_SSIZE_T
typdef ssize_t unsigned long long;
#endif


# Record types (structures)

```c
struct twoints {
    int a;
    int b;
};

struct twoints t;
t.a = 1;
t.b = 2;

struct twoints r = t;
r.a = ..;
```


```c
typedef struct  {
    double* data;
    size_t size;
} vector;

double sum3(const vector v)
{
    double t = 0.0;
    for (size_t i=0; i<v.size; ++i)
        t += v.data[i];
    return t;
}

double sum4(const vector* v)
{
    double t = 0.0;
    for (size_t i=0; i<(*v).size; ++i)   // v->size
        t += (*v).data[i];  // v->data[i]
    return t;
}

int main()
{
    vector x;
    x.size = 100;
    x.data = (double*)malloc(x.size*sizeof(double));
    sum3(x);
    sum4(&x);
    free(x.data);
    return 0;
}

```

gcc --std=c99 file.c

CHECK: C has anonymous structs from C11 (?) but C++ does not allow them (?)


// write a function that takes a numeric vector and computes the square of each element.

// write a function that implements vectorised (numpy-stype) addition of two numeric vectors /elementwise/

// + - sqrt sum mean std cumsum diff min argmin np.nonzero
// functions taking 1-2 vectors and returning scalars or modify given vectors in place or return malloc'd vector

double *x;
double n;
double *y = malloc(...)
test(x, n, y);


y = test2(x, n);
free(y);


// nonzero:
size_t m = nonzero(x, n, y);
nonzero(x, n, y, &m);
vector_ints v = nonzero(x, n);


# Example: implementing linked lists in C

see lab05.c

