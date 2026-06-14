# Lecture 13 (2026-06-09)


## Representing data frames

R - array of m arrays of length n (normally)
[a list of m vectors]
class = data.frame

but these can also be matrices
aggregate(iris[,1:2], iris[,5,drop=FALSE], function(x) c(MIN=min(x), MAX=max(x)))

R:  dplyr, data.table



pandas,

x.__dict__['_mgr'].blocks[0].get_values()


polars


Apache Arrow



## Representing sparse vectors and matrices

k << n non-zero elements

vectors:
v = [0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 3]
     0  1  2  3  4  5  6  7  8  9  A

2 vectors of length k

indexes = 2, 5, 10
data    = 1, 2,  3
n, k


v[i]

np.mean

A*v


sparse matrices:

scipy.sparse

https://docs.scipy.org/doc/scipy/reference/sparse.html


Dictionary of keys:

x = dict()
x[(2, 3)] = 4
x[(1, 2)] = 9

csc_array(arg1[, shape, dtype, copy, maxprint])


Compressed Sparse Row array     (CSR)

three vectors:

data    - length k     [1 2 3 4 5 6]
indexes - length k     [0 2 2 0 1 2]
indptr  - length n+1   [0 2 3 6]

number of non-zero elements in the i-row = indptr[i+1]-indptr[i]

for j in indexes[indptr[i]:indptr[i+1]]

k = 6
n = 3
m = 3
      ([[1, 0, 2],
        [0, 0, 3],
        [4, 5, 6]])





Compressed Sparse Column array  (CSC)





## Numerical libraries

https://www.gnu.org/software/gsl/doc/html/specfunc.html


GNU GSL - GNU Scientific Library

- math funs and algos



Maxima, Mathematica, Maple - computer algebra systems
Sages


~numerical~ symbolic computing

https://dlmf.nist.gov/




BLAS = basic linear algebra system 1979, 1988, 1990 /Fortan/
BLAS is used by LAPACK, GNU GSL, NumPy, SciPy, SymPy, R, MATLAB, ...


1. vector operations ($O(n)$), e.g.,

    * `axpy`: $\mathbf{y}=\mathbf{y}+\alpha\mathbf{x}$
    * `scal`: $\mathbf{x}=\alpha\mathbf{x}$
    * `copy`: $\mathbf{y}=\mathbf{x}$
    * `swap`: $\mathbf{y} \leftrightarrow \mathbf{x}$
    * `dot`:  $\mathbf{x}^T \mathbf{y}$ (dot product)
    * `nrm2`: $\|\mathbf{x}\|_2$ (Euclidean norm)
    * `asum`: $\|\mathbf{x}\|_1$ (Manhattan norm)


2. matrix-vector operations ($O(n^2)$), e.g.,

    * `gemv`: $\mathbf{y}=\alpha\mathbf{A}^*\mathbf{x}+\beta\mathbf{y}$, where $*$ is the matrix transpose or no-op (general matrix-vector multiply)
    * `ger`: $\mathbf{A}=\mathbf{A}+\alpha\mathbf{x}\mathbf{y}^T$ (general rank-1 update)

3. matrix-matrix operations ($O(n^3)$), e.g.,

    * `gemm`: $\mathbf{C}=\beta\mathbf{C}+\alpha \mathbf{A}^* \mathbf{B}^*$ (general matrix-matrix multiply)
    * `dtrsm`... TODO - triangular solve .. see below


LAPACK = linear algebra package





