#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>


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
