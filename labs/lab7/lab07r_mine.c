#define R_NO_REMAP
#include <R.h>
#include <Rinternals.h>
#include <Rmath.h>


SEXP amean(SEXP x, SEXP na_rm){

    int na_rm = LOGICAL(x)[0];
    int length = XLENGTH(x);
    int counter = 0;
    double sum = 0;

    if (Rf_isReal(x)){
        double* list_ptr = REAL(x);
        for (int i=0; i<length; i++){
            if (!ISNA(list_ptr[i])){
                counter++;
                sum += list_ptr[i];
            }

        }
    } else if (Rf_isInteger(x)){
        int* list_ptr = INTEGER(x);

        for (int i=0; i<length; i++){
            if ((list_ptr[i] != NA_INTEGER)){
                counter++;
                sum += list_ptr[i];
            }
        }
    } else{
        Rf_error("expecting a numeric vector of doubles or ints");
    }

    if (!na_rm && (counter != length)){
            return Rf_ScalarReal(NA_REAL);
    }

    return Rf_ScalarReal(sum/counter);
    
}


SEXP mysquare(SEXP x)
{
    double v = REAL(x)[0];
    double ret = v*v;

    return Rf_ScalarReal(ret);
}

// R CMD SHLIB mine_lab07r.c
