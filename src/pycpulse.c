#include <python2.6/Python.h>
#include "../include/cpulse.h"

static PyObject * cpulse_start(PyObject *self, PyObject *args) {
    start();
    Py_RETURN_NONE;
}

static PyObject * cpulse_stop(PyObject *self, PyObject *args) {
    stop();
    Py_RETURN_NONE;
}

float *pulses;

static PyObject * cpulse_pulse(PyObject *self, PyObject *args) {
    pulses = pulse();
    return Py_BuildValue("(ff)", pulses[0], pulses[1]);
}

static PyMethodDef CPulseMethods[] = {
    {"start", cpulse_start, METH_VARARGS, "start processing audio"},
    {"stop",  cpulse_stop,  METH_VARARGS, "stop processing audio"},
    {"pulse", cpulse_pulse, METH_VARARGS, "get a tuple of current pulses"},
    {NULL,    NULL,         0,            NULL}
};

PyMODINIT_FUNC init_cpulse(void) {
    (void) Py_InitModule("_cpulse", CPulseMethods);
}
