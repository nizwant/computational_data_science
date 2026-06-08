#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>

void _nn1(const double* X, const double* Y, double* o,
    size_t n, size_t m, size_t p)
{
    for (size_t i=0; i<m; ++i) {
        double min_dij = INFINITY;
        for (size_t j=0; j<n; ++j) {
            double d = 0.0;
            for (size_t k=0; k<p; ++k)
                d += (Y[i+m*k]-X[j+n*k])*(Y[i+m*k]-X[j+n*k]);
            if (d < min_dij) {
                min_dij = d;
                o[i] = (double)j;
            }
        }
    }
}

void _nn2(const double* X, const double* Y, double* o,
    size_t n, size_t m, size_t p)
{
    double cy[p];

    for (size_t i=0; i<m; ++i) {
        for (size_t k=0; k<p; ++k)
            cy[k] = Y[i+m*k];

        double min_dij = INFINITY;
        for (size_t j=0; j<n; ++j) {
            double d = 0.0;
            for (size_t k=0; k<p; ++k)
                d += (cy[k]-X[j+n*k])*(cy[k]-X[j+n*k]);
            if (d < min_dij) {
                min_dij = d;
                o[i] = (double)j;
            }
        }
    }
}

void _nn3(const double* X, const double* Y, double* o,
    size_t n, size_t m, size_t p)
{
    double cy[p];
    double d[n];
    for (size_t i=0; i<m; ++i) {
        for (size_t k=0; k<p; ++k)
            cy[k] = Y[i+m*k];

        for (size_t j=0; j<n; ++j)
            d[j] = 0.0;

        for (size_t k=0; k<p; ++k) {
            for (size_t j=0; j<n; ++j) {
                d[j] += (cy[k]-X[j+n*k])*(cy[k]-X[j+n*k]);
            }
        }

        double min_dij = INFINITY;
        for (size_t j=0; j<n; ++j) {
            if (d[j] < min_dij) {
                min_dij = d[j];
                o[i] = (double)j;
            }
        }
    }
}

#define MIN(a, b) ((a)<(b)?(a):(b))
#define BLOCK_SIZE 12
void _nn4(const double* X, const double* Y, double* o,
    size_t n, size_t m, size_t p)
{
    double cy[p];
    double d[BLOCK_SIZE];
    for (size_t i=0; i<m; ++i) {
        for (size_t k=0; k<p; ++k)
            cy[k] = Y[i+m*k];

        double min_dij = INFINITY;
        for (size_t j=0; j<n; j+=BLOCK_SIZE) {
            for (size_t jj=j; jj<MIN(n, j+BLOCK_SIZE); ++jj)
                d[jj-j] = 0.0;
            for (size_t k=0; k<p; ++k) {
                for (size_t jj=j; jj<MIN(n, j+BLOCK_SIZE); ++jj) {
                    d[jj-j] += (cy[k]-X[jj+n*k])*(cy[k]-X[jj+n*k]);
                }
            }
            for (size_t jj=j; jj<MIN(n, j+BLOCK_SIZE); ++jj) {
                if (d[jj-j] < min_dij) {
                    min_dij = d[jj-j];
                    o[i] = (double)jj;
                }
            }
        }
    }
}

SEXP nn(SEXP X, SEXP Y)
{
    if (!Rf_isReal(X) || !Rf_isMatrix(X)) Rf_error("X not a real matrix");
    if (!Rf_isReal(Y) || !Rf_isMatrix(Y)) Rf_error("Y not a real matrix");
    const size_t n = Rf_nrows(X);
    const size_t p = Rf_ncols(X);
    const size_t m = Rf_nrows(Y);
    if (p != Rf_ncols(Y)) Rf_error("wrong ncol of Y");

    SEXP o = PROTECT(Rf_allocVector(REALSXP, m));
    _nn4(REAL(X), REAL(Y), REAL(o), n, m, p);
    UNPROTECT(1);
    return o;
}
