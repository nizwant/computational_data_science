# Lecture 10 (2026-05-19)


## Matrix representations (C/Fortran contiguous), R/C and Python API calls for contiguous matrices

Array of arrays (rarely):
n rows, m columns

```c
double **A = (double**)malloc(sizeof(double*)*n);
for (size_t i=0; i<n; ++i)
    A[i] = (double*)malloc(sizeof(double)*m);

A[i][j]// i-th row and j-column:
```

flat vector


```c
double *A = (double*)malloc(sizeof(double)*n*m);

A[m*i+j] // i-th row and j-column:  // row-major representation (C) -- default in numpy
// or
A[i+n*j] // i-th row and j-column:  // column-major representation (Fortran) -- R
```


R:

```r
> A <-  1:12
> dim(A) <- c(3, 4)
> A
     [,1] [,2] [,3] [,4]
[1,]    1    4    7   10
[2,]    2    5    8   11
[3,]    3    6    9   12
> `dim<-`(A, NULL)
 [1]  1  2  3  4  5  6  7  8  9 10 11 12
```

```c
// testing whether A is a double-typed matrix
int Rf_isReal(SEXP A);
int Rf_isMatrix(SEXP A);  // dim attribute is a vector of length 2

// getting the dimensions:
int Rf_nrows(SEXP A);  // 32-bit int!
int Rf_ncols(SEXP A);

// the following works with n-dimensional arrays, also with n!=2:
SEXP dim = PROTECT(Rf_getAttrib(A, Rf_ScalarString("dim")));
int nrows = INTEGER(dim)[0];
int ncols = INTEGER(dim)[1];

// allocating a new matrix (items are not initialised):
SEXP B = PROTECT(Rf_allocMatrix(REALSXP, nrows, mcols));

// Fortran (column-major) order:
double* a = REAL(A);
a[i+j*n];  // i-th row, j-th column
```


Example 1: Matrix-vector product


```c
void _matvecprod1(const double* A, const double* x, double* y,
    size_t n, size_t m)
{

}


SEXP matvecprod(SEXP A, SEXP x)
{
    // ..._matvecprod1
}
```


Numpy:


```python
In [1]: A = np.arange(12)

In [2]: A
Out[2]: array([ 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11])

In [3]: A.shape = [3,4]

In [4]: A
Out[4]:
array([[ 0,  1,  2,  3],
       [ 4,  5,  6,  7],
       [ 8,  9, 10, 11]])

In [5]: A.data.c_contiguous
Out[5]: True

In [6]: A.data.f_contiguous
Out[6]: False

In [9]: A.T
Out[9]:
array([[ 0,  4,  8],
       [ 1,  5,  9],
       [ 2,  6, 10],
       [ 3,  7, 11]])

In [10]: A.T.data.f_contiguous
Out[10]: True

```

```c
// 1. PyArray_Check(arr) ?
// 2. PyArray_TYPE((const PyArrayObject*)arr) == NPY_DOUBLE ?
int PyArray_NDIM(PyArrayObject* arr);  // 2 for matrices
npy_intp* PyArray_SHAPE(PyArrayObject* arr);  // length-NDIM array, e.g., {nrows, ncols}

int PyArray_IS_C_CONTIGUOUS(PyArrayObject* arr);
int PyArray_IS_F_CONTIGUOUS(PyArrayObject* arr);
npy_intp* PyArray_STRIDES(PyArrayObject* arr);  // length-NDIM array

npy_intp dims[] = {nrows, ncols};
PyObject *PyArray_SimpleNew(int nd, npy_intp const *dims, int typenum);
```




## Locality of reference; cache

## SIMD and auto-vectorisation

Single instruction, multiple data

`-O3 -march=native`

edit CFLAGS in /usr/lib64/R/etc/Makeconf

PKG_CPPFLAGS="-O2" Rscript lab10a.R


## Blocking



Example 2: 1-NN

## OpenMP

Open Multiprocessing

API for a portable multithreaded shared memory parallelism
implementing the fork-join model.

MPI - another library



