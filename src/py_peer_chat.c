#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "client_lib.h"

PyObject *py_connect(PyObject *self, PyObject *args)
{
    const char *username;
    const char *password;
    if (!PyArg_ParseTuple(args, "ss", &username, &password))
        return NULL;

    int ret = peer_connect(username, password);
    if (ret < 0)
    {
        PyErr_SetString(PyExc_ConnectionError, "Failed to connect to server");
        return NULL;
    }
    Py_RETURN_NONE;
}

PyObject *py_get_user(PyObject *self, PyObject *args)
{
    const char *username;
    const char *password;
    if (!PyArg_ParseTuple(args, "ss", &username, &password))
        return NULL;

    int ret = peer_get_user(username, password);
    if (ret < 0)
    {
        PyErr_SetString(PyExc_RuntimeError, "Failed to request peer");
        return NULL;
    }
    Py_RETURN_NONE;
}

PyObject *py_send_message(PyObject *self, PyObject *args)
{
    const char *recipient;
    const char *message;
    if (!PyArg_ParseTuple(args, "ss", &recipient, &message))
        return NULL;

    int ret = peer_send_message(recipient, message);
    if (ret < 0)
    {
        PyErr_Format(PyExc_RuntimeError, "Failed to send message to '%s'", recipient);
        return NULL;
    }
    Py_RETURN_NONE;
}

PyObject *py_send_ping(PyObject *self, PyObject *args)
{
    const char *recipient;
    if (!PyArg_ParseTuple(args, "s", &recipient))
        return NULL;

    int ret = peer_send_ping(recipient);
    if (ret < 0)
    {
        PyErr_Format(PyExc_RuntimeError, "Failed to ping '%s'", recipient);
        return NULL;
    }
    Py_RETURN_NONE;
}

PyObject *py_receive(PyObject *self, PyObject *args)
{
    int timeout_ms = 100;
    if (!PyArg_ParseTuple(args, "|i", &timeout_ms))
        return NULL;

    int ret = peer_receive(timeout_ms);
    return PyLong_FromLong(ret);
}

PyObject *py_disconnect(PyObject *self, PyObject *args)
{
    peer_disconnect();
    Py_RETURN_NONE;
}

PyObject *py_get_fd(PyObject *self, PyObject *args)
{
    int fd = peer_get_fd();
    return PyLong_FromLong(fd);
}

static PyMethodDef peer_chat_methods[] = {
    {"connect", py_connect, METH_VARARGS, "Connect to the server and register with username/password."},
    {"get_user", py_get_user, METH_VARARGS, "Request a peer's address from the server."},
    {"send_message", py_send_message, METH_VARARGS, "Send a message to a previously discovered peer."},
    {"send_ping", py_send_ping, METH_VARARGS, "Send a ping to a previously discovered peer."},
    {"receive", py_receive, METH_VARARGS, "Receive one incoming packet (timeout_ms, default 100)."},
    {"disconnect", py_disconnect, METH_NOARGS, "Close connection and free all internal state."},
    {"get_fd", py_get_fd, METH_NOARGS, "Get the internal socket file descriptor."},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef peer_chat_module = {
    PyModuleDef_HEAD_INIT,
    "peer_chat",
    "PeerChat C extension: UDP peer-to-peer messaging",
    -1,
    peer_chat_methods};

PyMODINIT_FUNC PyInit_peer_chat(void)
{
    return PyModule_Create(&peer_chat_module);
}
