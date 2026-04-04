# Computational Data Science - Lecture 06 - 2026-03-31

## Written Assignment 1 - 2026-04-21

Test prep - exercises:

1. Implement the following operations on C arrays:

    * arithmetic mean, standard deviation, and other simple aggregates
    * index of the maximal element
    * vectorised (elementwise) addition of two arrays
        (like numpy's `+` on two vectors)
    * check if a given array is a palindrome
    * repeat with recycling `k` times, e.g., `rep_times([1,2,3], 2) == [1,2,3,1,2,3]`
    * repeat each `k` times, e.g., `rep_each([1,2,3], 2) == [1,1,2,2,3,3]`
    * remove elements < 0
    * count the number of increasing contiguous subsequences in a given array
    * locate the longest increasing contiguous subsequence in a given array
    * bubble, selection, insertion sort
    * bubble, selection, insertion argsort (return an ordering permutation,
        i.e., the indexes that yield a sorted version of the input array)
    * located the index of a given element in a sorted array (search)
    * remove duplicates in a sorted array
    * merge two sorted arrays into a single sorted array
    * partition an array `x` based on a given pivot element `v`;
        return two arrays `[x[i]: x[i]<=v]` and `[x[i]: x[i]>v]`

2. Implement the following operations on singly- [head; next]
    and on doubly-linked lists [head, tail; next, prev]:

    * is empty?
    * print list
    * push front, push back, pop front, pop back
    * delete all (clear)
    * join two lists
    * get i-th element
    * remove i-th element
    * bubble sort
    * selection sort
    * merge two sorted lists (and yield a sorted list in result)
    * insert an element into a sorted list (maintain order)

    Practice by implementing two kinds of APIs (where applicable):

    ```c
    head = modify(head);  // new head returned
    modify(&head);  // head modified 'in-place'
    ```

3. Implement the following operations on binary search trees:

    * is empty?
    * print all elements in increasing order
    * get the number of elements in the tree
    * get max height
    * search for a specific value
    * insert a new value, ignore duplicates
    * delete all (clear)
    * delete a specific key
    * left, right, double rotations (like in AVL trees)
    * convert to a linked list (return a new singly-linked list)



## Cython cont'd

### Calling C functions from Cython

<see codes lab06*>


### Accessing **numpy** from Cython

<see codes lab06*>


## Enums

```c
typedef enum choices {
    choice_zero,   // starts at 0
    choice_one,    // advances by 1
    choice_two,
    choice_seven=7,
    choice_eight,  // advances by 1
    choice_ten=10
} Choices;

//
Choices c = choice_eight;
printf("%d", c);
```


```c
#define CHOICE_ZERO 0
#define CHOICE_ONE  1
#define CHOICE_TWO  2
// ...
typedef int Choices;

Choices c = CHOICE_ZERO;
```


## Integers as bit sequences

unsigned 32-bit integers

signed 32-bit integers = two's complement

overflow

0111 == 7
1000 == -8

-ftrapv/-fwrapv

size_t n = 100000000000;
for (int i=0; i<n; ++i)



bitwise operators:

* << left shift
* >> right shift
* & bitwise AND
* | bitwise OR
* ~ bitwise NOT
* ^ bitwise XOR

32 bits:
7 << 1 == 14
0000111 << 1 == 0001110

6 & 14 ==
0110
1110
0110 == 6



ICU:

```c
#define UREGEX_UNIX_LINES       1  // 0001
#define UREGEX_CASE_INSENSITIVE 2  // 0010
#define UREGEX_COMMENTS         4  // 0100
// ...


uint32_t flags = UREGEX_CASE_INSENSITIVE | UREGEX_COMMENTS;


if (flags & UREGEX_CASE_INSENSITIVE) ...


what's this?

if (x != y) {
    x ^= y;
    y ^= x;
    x ^= y;
}
```

Bit fields in structs:

```c
struct abc {
    unsigned int a : 1;  // 1 (named) bit
    unsigned int b : 1;
    unsigned int c : 1;
};
```

## Unions

```c
typedef union int_or_double {
    // alternative declarations:
    int i;     // either
    double f;  // or
} IntOrDouble;

typedef struct value {
    int is_int;
    IntOrDouble datum;
} Value;

Value z;
z.is_int = 1;
z.datum.i = 42;

if (z.is_int) printf("%d", z.datum.i);
else  printf("%f", z.datum.f);

```


## ASCII strings

char - 8-bit integer

0..127
printf("%c", (char)127);

'a' == (char)97;

C strings - arrays of the type char, NUL-terminated (0)

const char* s = "pupa";
s[4] == '\0' == (char)0;

printf("%s", s);


int strlen(const char* s) {
    int l = 0;
    //while (*(s++)) l++;
    while (s[l] != '\0') l++;
    return l;
}


32-bit  UTF-32 int
UTF-8 - 8-bit variable-length between 1 and 6 bytes per character




#include <string.h>

* `strlen(const char* s)` fetches the number of bytes in a string `s`,

* `strncpy(char* to, const char* from, size_t count)` copies a most `count` bytes
    from the string `from`, including the null character,
    and stores it in the array `to`,

* `strcmp(const char* s1, const char* s2)` returns 0 if two strings are identical,
    a value less than 0 if `s1` is lexicographically bytewise *less than*
    `s2`, and something greater than 0 otherwise,

```c
int atoi(const char *str);  // alpha to integer
double atof(const char *str);  // alpha to float (double)
```

"3.14"


void* memcpy(void *dest, const void *src, size_t n);

double *x = ... // array of size n
double *y = ... // array of size n

memcpy(y, x, n*sizeof(double));



## R/C API

Further reading:

* [*Deep R Programming – Chapter 14: Interfacing compiled code*](https://deepr.gagolewski.com/chapter/310-compiled.html)
* [*Writing R Extensions*](https://cran.r-project.org/doc/manuals/R-exts.html), Sec. 5 and Sec. 6
* [*R Internals*](https://cran.r-project.org/doc/manuals/r-release/R-ints.htm)


### R's SEXPRECs

SEXP == SEXPREC* (a struct)

https://cran.r-project.org/doc/manuals/r-release/R-ints.html

### Functions taking vectors and returning scalars in R


Rboolean Rf_isLogical(SEXP s);
Rboolean Rf_isInteger(SEXP s);
Rboolean Rf_isReal(SEXP s);
int* LOGICAL(SEXP x);
int* INTEGER(SEXP x);
double* REAL(SEXP x);
R_xlen_t XLENGTH(SEXP x);
SEXP Rf_ScalarReal(double x);

probs ptrdiff_t (64-bit signed int)

int LENGTH(SEXP x); -- doesn't support long vectors introduced in
>= R 3.0.0


NA, NA_integer_, NA_real_

At the C level: NA_LOGICAL, NA_INTEGER, NA_REAL
-2**31, -2**31, special NaN

if (x[i] == NA_LOGICAL) ..
else ....


x[i] == NA_REAL is always false

NA_REAL == NA_REAL is false
ISNA(x[i]) can be used


// Implement: sum, mean, sd, min, which.min
// with NA handling


