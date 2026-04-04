#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>

SEXP square(SEXP x)
{
    if (!Rf_isReal(x) || XLENGTH(x) != 1)
        Rf_error("expecting a numeric vector of length 1");

    double v = REAL(x)[0];
    double ret = v*v;

    return Rf_ScalarReal(ret);
}

//  R CMD SHLIB lab04d-squarer.c


//gcc -I"/usr/lib64/R/include" -DNDEBUG -O3 -march=native  -I/usr/local/include    -fpic  -O2 -Wall -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 -fstack-protector-strong -funwind-tables -fasynchronous-unwind-tables -fstack-clash-protection -Werror=return-type -flto=auto -g  -c lab04d-squarer.c -o lab04d-squarer.o
//gcc -shared -L/usr/lib64/R/lib -flto=auto -o lab04d-squarer.so lab04d-squarer.o -L/usr/lib64/R/lib -lR
