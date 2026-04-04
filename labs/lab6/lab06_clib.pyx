import numpy as np
cimport numpy as np  # .pxd
np.import_array()

cdef extern from "lab06.h":
    double square(double x)
    double mysum(const double* x, size_t n)

cpdef double sumpy(double[::1] x):
    return mysum(&x[0], x.shape[0])

cpdef double sumpy2(x):
    cdef np.ndarray[double] x2 = \
        np.asarray(x, dtype=np.double, order="C")
    return mysum(&x2[0], x2.shape[0])


# cython lab06_clib.pyx
# gcc -shared -pthread -fPIC -fwrapv -O2 -Wall -fno-strict-aliasing -I/usr/include/python3.13 -I/home/gagolews/.local/lib/python3.13/site-packages/numpy/_core/include lab06_common.c lab06_clib.c -o lab06_clib.so

# >>> import numpy as np
# >>> np.get_include()
# '/home/gagolews/.local/lib/python3.13/site-packages/numpy/_core/include'


cpdef double squarepy(double x):
    return square(x)

def squarepy2(x):
    return square(x)

