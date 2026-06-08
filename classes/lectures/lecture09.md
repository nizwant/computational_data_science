# Lecture 9 (2026-04-28)


TODO: selection sort on lists :)



https://docs.python.org/3/c-api/arg.html#c.PyArg_ParseTuple


python3 setup.py sdist
- builds a source distribution of the package (tar.gz)

pip3 install dist/pypackage-0.0.1.tar.gz

or

pip3 install .



    SEXP z = Rf_scalarReal(y);
    PROTECT(z);
    // ..
    UNPROTECT(1);
    return z;


    PyObject* z = PyFloat_FromDouble(y);   // Py_INCREF(z);
    // ...
    return z;  // passes over the ownership along the pointer



https://numpy.org/doc/stable/reference/c-api/array.html



PyObject* retval = PyArray_SimpleNew(1, &n, NPY_DOUBLE);
double* y = PyArray_DATA((const PyArrayObject*)retval);
// ...
return retval;
