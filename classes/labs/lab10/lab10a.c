#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>

void _matvecprod1(const double* A, const double* x, double* y,
    size_t n, size_t m)
{
    // A is Fortran-contiguous (column-major order)
    for (size_t i=0; i<n; ++i) {
        y[i] = 0.0;
        for (size_t j=0; j<m; ++j) {
            y[i] += A[i+j*n]*x[j];
        }
    }
}

void _matvecprod2(const double* A, const double* x, double* y,
    size_t n, size_t m)
{
    // A is Fortran-contiguous (column-major order)
    for (size_t i=0; i<n; ++i)
        y[i] = 0.0;

    for (size_t j=0; j<m; ++j) {
        for (size_t i=0; i<n; ++i) {
            y[i] += A[i+j*n]*x[j];
        }
    }
}

#define MIN(a, b) ((a)<(b)?(a):(b))
#define BLOCK_SIZE 12
void _matvecprod3(const double* A, const double* x, double* y,
    size_t n, size_t m)
{
    // A is Fortran-contiguous (column-major order)
    for (size_t i=0; i<n; ++i)
        y[i] = 0.0;

    for (size_t j=0; j<m; j+=BLOCK_SIZE) {
        for (size_t i=0; i<n; i+=BLOCK_SIZE) {
            for (size_t jj=j; jj<MIN(m,j+BLOCK_SIZE); jj++)
                for (size_t ii=i; ii<MIN(n,i+BLOCK_SIZE); ii++)
                    y[ii] += A[ii+jj*n]*x[jj];
        }
    }
}

SEXP matvecprod(SEXP A, SEXP x)
{
    if (!Rf_isReal(x)) Rf_error("x not a real vector");
    if (!Rf_isReal(A) || !Rf_isMatrix(A)) Rf_error("A not a real matrix");
    const size_t n = Rf_nrows(A);
    const size_t m = Rf_ncols(A);
    if (m != XLENGTH(x)) Rf_error("wrong length of x");

    SEXP y = PROTECT(Rf_allocVector(REALSXP, n));
    _matvecprod3(REAL(A), REAL(x), REAL(y), n, m);
    UNPROTECT(1);
    return y;
}
