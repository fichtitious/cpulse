#include <python2.6/Python.h>
#include "cpulse.h"

float *_pulses;

static PyObject * pulse(PyObject *self, PyObject *args) {
    _pulses = cpulse_pulse();
    return Py_BuildValue("i", (int) _pulses[0] - 2);
}

static PyMethodDef CPulseMethods[] = {
    {"pulse", pulse, METH_VARARGS, "get current pulse"},
    {NULL,    NULL,  0,            NULL}
};

PyMODINIT_FUNC initcpulse(void) {
    (void) Py_InitModule("cpulse", CPulseMethods);
    cpulse_start();
    Py_AtExit(&cpulse_stop);
}
