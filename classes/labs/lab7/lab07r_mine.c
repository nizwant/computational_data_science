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

SEXP aadd(SEXP x, SEXP y){
    if (!Rf_isReal(x) || !Rf_isReal(y)){
        Rf_error("expecting a numeric vectors of doubles");
    }

    size_t length_x = XLENGTH(x);
    size_t length_y = XLENGTH(y);

    if (length_x == 0 || length_y == 0) return Rf_alloccVector(REALSXP,0);

    int max_length = (length_x > length_y)?length_x:length_y;

    SEXP answer = Rf_alloccVector(REALSXP,max_length);
    PROTECT(answer);

    double* x_ptr = REAL(x);
    double* y_ptr = REAL(y);
    double* answer_ptr = REAL(answer);
    
    for(size_t i=0; i<max_length; i++){
        if (ISNA(x[i%length_x]) || ISNA(y[i%length_y])){
            answer_ptr[i] = NA_REAL;
        }
        else{
            answer_ptr[i] = x[i%length_x] + y[i%length_y];
        }
    }

    UNPROTECT(1);
    return answer;
}


SEXP mysquare(SEXP x)
{
    double v = REAL(x)[0];
    double ret = v*v;

    return Rf_ScalarReal(ret);
}

// R CMD SHLIB mine_lab07r.c
