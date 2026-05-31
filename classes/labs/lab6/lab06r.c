#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>



SEXP mysquare(SEXP x)
{
    if (!Rf_isReal(x) || XLENGTH(x) != 1)
        Rf_error("expecting a numeric vector of length 1");

    printf("%d", (int)(((char*)x)[0]));

    double v = REAL(x)[0];
    double ret = v*v;

    return Rf_ScalarReal(ret);
}

// R CMD SHLIB lab06r.c
