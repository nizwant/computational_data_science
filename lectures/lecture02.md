```c
resource *r1=NULL, ..., *rn=NULL;
r1 = open_r1();
if (!r1) goto cleanup_r1:
// ...
rn = open_rn();
if (!rn) goto cleanup_rn:
// ...

// stack-like cleanup
cleanup_rn:
if (rn) close_rn(rn);
// ...
cleanup_r1:
if (r1) close_r1(r1);
```


```c
for (...) {
    for (...) {
        for (...) {
            if (over)
                goto end;
        }
    }
}
end:
;
```


```c
while (expression) statement
```

the same as `while (expression != 0)`

```c
int i = 5;
while (i) {
    printf("%d", i);
    i--;
}
```

yields 5 4 3 2 1

```c
int i=0;
while (i<4) {
    int j=0;
    while (j<i) {
        printf("%d,%d\n", i, j);
        j++;
    }
    i++;
}
// no j
```
i,j
1,0
2,0
2,1
3,0
3,1
3,2


break, continue

```c
int i = 0;
while (i < 6) {
    i++;
    if (i % 2 == 0)
        continue;
    printf("%d ", i);
}
```

1,3,5

```c
do
    statement
while (expression);   // a semicolon!
```

```c
while (1) {
    statement
    if (!condition) break;
}
```

```c
for (init; expression; thereafter)
    statement
```

more or less equivalent to:

```c
{
    init;
    while (expression) {
        statement
        thereafter;
    }
}
```

```c
int n = 5;
for (int i=0; i<n; ++i)
    printf("%d", i);
```

a for loop that computes the sum of odd integers between 1 and 99:

```c
int sum=0;
for (int i=1; i<=99; i+=2)
    sum += i;
```


pip3 install cython
python3 -m pip install cython


```c
int n = 5;
for (int i=0; i<n; ++i) {
    if (i % 2 == 0) continue;
    printf("%d", i);
}
```


# functions

```c
int main()
{
    // ...
    return 0;  // EXIT_SUCCESS
}
```

```c
return_type fun_name(type1 name1, type2 name2)
{
    // body
}
```


```c
#include <stdio.h>

double my_abs(double x) {
    if (x >= 0.0) return x;
    else return -x;
}

int main() {
    double w = -0.5;
    double z = my_abs(w);
    printf("%f\n", z);
    return 0;
}
```


```c
#include <stdio.h>

int n = 10000;

void f(int i) {
    printf("%d", i);
}

int main() {
    n++;
    printf("%f\n", n);
    return 0;
}
```
// global variables

/// static variables
```c
static int i=0;

static void f() {
    // ;;
}

void counter()
{
    static int i = 0;  // initialised at compile time
    i++;
    printf("%d ", i);
}

int main()
{
    counter();
    counter();
    counter();
    return 0;
}
```


```c
double fun1(double x)
{ ... it cannot call any of the funs below ... }

double fun2(double y)
{ ... can call fun1 ... }

...

int main()
{ ... can call of all the above funs ... }
```

// declare:
```c
double fun1(double x);
double fun2(double y);

int main()
{ ... can call of all the above funs ... }

double fun2(double y)
{ ... can call all the functions ... }

double fun1(double x)
{ ... can call all the functions ... }
```

# The C standard library

`#include <stdio.h>`
printf

`#include <math.h>`
fabs, sqrt, pow, ...


# The C preprocessor

CPP, macro processor

1. preprocessing - textual only
2. compilation
3. linking
4. execution


preprocessing:

1. join lines ending with \

2+\
2

2+2

2. replace all comments by single spaces

3. application of preprocessor #directives and macro expansion

#define PI 3.14
printf("%f", PI*PI)
--->
printf("%f", 3.14*3.14)

raw inclusion of a local file (relative to the current source file)
#include "file.h"

searches for file.h in the INCLUDE PATH:
#include <file.h>
cpp -v /dev/null -o /dev/null

// to add to the input path
gcc -I/home/marek hello.c

