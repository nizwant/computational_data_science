#define PY_SSIZE_T_CLEAN
#include <Python.h>

// PyArray_* functions:
#include <numpy/arrayobject.h>
#include <numpy/ndarrayobject.h>
#include <numpy/ndarraytypes.h>
#include <numpy/arrayscalars.h>
#include <numpy/ufuncobject.h>


double mysum(const double* x, size_t n)
{
    double v = 0.0;
    for (size_t i=0; i<n; ++i) v += x[i];
    return v;
}


PyObject* pysum(PyObject* /*self*/, PyObject* args)
{
    if (PyTuple_Size(args) != 1)
        return PyErr_Format(PyExc_RuntimeError, "expected 1 arg");

    PyObject* args0 = PyTuple_GetItem(args, 0);
    if (!args0) return NULL;

    if (!PyArray_Check(args0))
        return PyErr_Format(PyExc_RuntimeError, "expected a numpy array");

    const PyArrayObject* x = (const PyArrayObject*)args0;
    if (PyArray_TYPE(x) != NPY_DOUBLE)
        return PyErr_Format(PyExc_RuntimeError, "expected a numpy double-typed array");

    if (!PyArray_IS_C_CONTIGUOUS(x))
        return PyErr_Format(PyExc_RuntimeError, "expected a contiguous array");

    const double* xptr = PyArray_DATA(x);
    size_t n = PyArray_SIZE(x);

    double y = mysum(xptr, n);
    return PyFloat_FromDouble(y);
}



double mysquare(double x)
{
    return x*x;
}

PyObject* pysquare(PyObject* /*self*/, PyObject* args)
{
    double _x;
    // if (!PyArg_ParseTuple(args, "d", &_x))
    //     return NULL;
    if (PyTuple_Size(args) != 1)
        return PyErr_Format(PyExc_RuntimeError,
                            "expected 1 arg");

    PyObject* args0 = PyTuple_GetItem(args, 0);
    if (!args0) return NULL;

    _x = PyFloat_AsDouble(args0);
    if (_x == -1 && PyErr_Occurred())
        return NULL;



    double y = mysquare(_x);

    return PyFloat_FromDouble(y);
}



static PyMethodDef my_methods[] = {
    {"pysquare", pysquare, METH_VARARGS, "Pysquare's docstring"},
    {"pysum", pysum, METH_VARARGS, "pysum's docstring"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef my_module = {
    PyModuleDef_HEAD_INIT,
    "cmodule",
    "my module's docstring",
    -1,
    my_methods
};

PyMODINIT_FUNC/* PyObject* */ PyInit_cmodule()
{
    PyObject* mod = PyModule_Create(&my_module);
    if (!mod) return NULL;
    import_array();
    return mod;

}

