#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>

SEXP asum(SEXP x, SEXP y)
{
    if (!Rf_isReal(x) || !Rf_isReal(y))
        Rf_error("x and y should be real");
    size_t nx = XLENGTH(x), ny = XLENGTH(y);
    if (nx == 0 || ny == 0)
        return Rf_allocVector(REALSXP, 0);
    size_t nz = (nx>ny)?nx:ny;  // max
    SEXP z;
    PROTECT(z = Rf_allocVector(REALSXP, nz));
    double *px = REAL(x), *py = REAL(y), *pz = REAL(z);
    for (size_t i=0; i<nz; ++i) {
        if (ISNA(px[i%nx]) || ISNA(py[i%ny]))
            pz[i] = NA_REAL;
        else
            pz[i] = px[i%nx] + py[i%ny];
    }
    UNPROTECT(1);
    return z;
}

SEXP amean(SEXP x, SEXP na_rm)
{
    int na_rm_bool = NA_LOGICAL;
    if (Rf_isLogical(na_rm) && XLENGTH(na_rm) == 1)
        na_rm_bool = LOGICAL(na_rm)[0];
    if (na_rm_bool == NA_LOGICAL)
        Rf_error("na_rm should be either TRUE or FALSE");

    double v = 0.0;  // sum
    size_t m = 0;  // # non missing
    if (Rf_isInteger(x)) {
        const int* px = INTEGER(x);
        size_t n = XLENGTH(x);
        for (size_t i=0; i<n; ++i) {
            if (px[i] == NA_INTEGER) {
                if (!na_rm_bool)
                    return Rf_ScalarReal(NA_REAL);
            }
            else {
                v += px[i];
                m++;
            }
        }
    }
    else if (Rf_isReal(x)) {
        const double* px = REAL(x);
        size_t n = XLENGTH(x);
        for (size_t i=0; i<n; ++i) {
            if (ISNA(px[i])) {
                if (!na_rm_bool)
                    return Rf_ScalarReal(NA_REAL);
            }
            else {
                v += px[i];
                m++;
            }
        }
    }
    else
        Rf_error("x should be a numeric vector");

    return Rf_ScalarReal(v/m);
    // x can be INTSXP or REALSXP
    // there can be missing values in x
    // na_rm a single TRUE or FALSE
    // x[i] == NA_INTEGER or ISNA(x[i]) for reals
}
