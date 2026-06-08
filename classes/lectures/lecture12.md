# Lecture 12 (2026-06-02)

## C++

C += 1

C can be considered a subset of C++ (in some sense)

double *x = (double*)malloc(sizeof(double)*n);  // (double*) necessary in C++

int n = 5;
int x[n]; // fine in C

https://en.wikipedia.org/wiki/Compatibility_of_C_and_C%2B%2B

bool in C23




g++ prog.cpp   # gcc -x c++ prog.cpp
clang++



In R packages, just put .cpp files into the src/ directory

In Python,
`setuptools.Extension('spam', sources=['module.cpp'], language='c++')`.



In C:
int test(int x);
this will be compiled as 'test'

In C++ we have name mangling:
int test(int x);
this will be called like a3785698w347terg_test_int

```c
#ifdef __cplusplus
extern "C" {
#endif

int test(int x);  // don't use name mangling

#ifdef __cplusplus
}
#endif
```




##  `bool` type (present also in C23)

```c++
bool x;
x = true;
x = false;
```


##  new and delete (constructors, destructors!)

```c++
int *x = new int[n];  // like malloc
double *u = new double;   // a single double allocated

delete [] x;   // it was an array
delete u;
```

deleting is necessary - no garbage collection




##  function and operator overloading

```c++
int square(int x) { return x*x; }  // _square_int
double square(double x) { return x*x; } // _square_double
```

square(3);  // first
square(3.0); // second


they must differ in the number or type of arg list


MAKE_GENERIC(test, int)... -> test_int


```c++
struct test { int x; }
test operator+(test x, test y) {
    return x*y;
}
```


struct test { int data; };

test operator+(test x, test y) {
    test z;
    z.data = x.data + y.data;
    return z;
}

// z = x+y;

operator<<

operator()


Python:

z = x + y # type(x).__add__(x, y)

def __add__(self, other):  # class method
    return ...


operator()



##  default arguments

```c++
int pow(int x, int p=2);
```



##  object references


```c
int swap(int* a, int* b) {
    int c = *a;
    *a = *b;
    *b = c;
}

int a, b;
swap(&a, &b);
list_push_front(&head, val);
```


```c++
int swap(int &a, int &b) {
    int c = a;
    a = b;
    b = c;
}

int a, b;
swap(a, b);

struct test { int data; };
int& getdata(test& t) { return t.data; }

test z;
getdata(z) = 6;
```



##  templates, generic programming + specialisation for faster code

```c++
//template <class T>
template <typename T>
T square(T x) { return x*x; }   // operator*

square(6);
square(6.0);


template double square<double>(double x);


template <typename T, typename S>
T plus(T x, S y) { return x+y; }

template <typename T, int X>
T power(T x) {
    T y = x;
    for (int i=X-1; i>0; --i) y *= x;
    return y;
}

power<double, 2>(2.0);
power<double, 5>(2.0);
```

##  header-only libraries, namespaces (also: modules)

```c++
import file
file.object_from_file

namespace my {
    int test();
};

my::test();


using namespace my;   // from file import *
test();
```


Header-only libs - libraries with function/class defs in .h (.hpp) files


Rcpp:

```c++
#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
double mysum(NumericVector x) {
    size_t n = x.size();
    for (....)
        v += x[i];
    return v;
}
```

##  basic OOP: constructors, destructors, inheritance, virtual functions


```c
struct list {
    int data;
};

void list_insert(list* this, int x);
```

```c++
struct list2 {
public:   // explicit
    int data;
};


class list {
private:  // explicit
    int data;

public:
    void insert(int x);  // method
};

```






##  C++ Standard Library

* Data structures: vector, deque, stack, map (hash table)
* Algorithms: sort, ...

#include <vector>

std::vector x(10);
x[0] = ...
std::sort(x.begin(), x.end());
std::stable_sort(x.begin(), x.end());

https://en.cppreference.com/cpp/algorithm/sort

Rcpp
Boost


