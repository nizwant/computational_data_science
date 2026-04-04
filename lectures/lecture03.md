1. Assembly cont'd

`gcc  -S -fomit-frame-pointer -O1 file.c`

https://godbolt.org/ - MIPS64, ARM, SPARC, ...


2. Benchmarking Python vs Cython vs Numba revisited

* PEP 659 – Specializing Adaptive Interpreter
* PEP 744 – JIT Compilation


3. Preprocessor cont'd

```c
#include "file.h"
#include <file.h>  // gcc -Ipath -I.
#define PI 3.14
```

#define x y

// function-like macro

#define F(x) x*x
F(3) -> 3*3
F(x+y) -> x+y*x+y


#define F(x) (x)*(x)
F(3) -> (3)*(3)
F(x+y) -> (x+y)*(x+y)


#define MAX(A, B) ((A)>(B))?(A):(B)

MAX(sum(x), sum(y))  :/
sx = sum(x)
sy = sum(y)
MAX(sx, sy)

#undef PI
#define PI 3.1415


```c
#define A 1
printf("%d", A);  // prints 1
#undef A
#define A 2
printf("%d", A);  // prints 2
```



```c
#define ASSERT(EXPR) do { \
    if (!(EXPR)) { \
        fprintf(stderr, "Assertion failed"); \
        exit(1); \
    } \
} while(0)
```

// do statement while(expression);

if (something)
    ASSERT(x > 0);
else
    ASSERT(y > 0);


```c
#define ASSERT(EXPR) \
    if (!(EXPR)) { \
        fprintf(stderr, "Assertion failed"); \
        exit(1); \
    }
```

if (something)
    ASSERT(x > 0)
else
    ASSERT(y > 0)

ASSERT(x > 0)
;

```c
#define STR(x) #x  /* stringisation */
#define XSTR(x) STR(x)

#define ASSERT(EXPR) do { \
    if (!(EXPR)) { \
        fprintf(stderr, "Assertion " #EXPR " failed in "\
            __FILE__ ":" XSTR(__LINE__) "\n"); \
        exit(1); \
    } \
} while(0)
```

"a " \
"string"

fprintf(stderr, "Assertion " "n > 1000")

fprintf(stderr, "Assertion ", "n > 1000")

fprintf(stderr, "Assertion %s failed", "n > 1000")

printf(...)
fprintf(stdout, ...)


//
#if ...
...
#endif


#if ...
...
#elif ..
....
#else
...
#endif

#define PI. ...

#if !defined(PI)
#define PI 3.1415
#elif PI == 3.14
#undef PI
#define PI 3.1415
#else
#error "WTF?"
#endif


#define OPENMP_IS_ENABLED 0

```c
int Comp_get_max_threads()
{
#if OPENMP_IS_ENABLED
    return omp_get_max_threads();
#else
    return 1;
#endif
}
```


typedef int liczba
liczba i=0;



#define HAVE_SSIZE_T 1
...
#define HAVE_SSIZE_T 0

```c
#if !defined(HAVE_SSIZE_T) || !HAVE_SSIZE_T
typedef ptrdiff_t ssize_t;
#endif
```


typedef ptrdiff_t ssize_t;



