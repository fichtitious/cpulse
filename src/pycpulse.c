/*
  This is a Python extension module that wraps cpulse.  cpulse is started
  when you import the module in Python and stopped when the module gets
  garbage-collected.

  As in cpulse.c, there is no threading.  You simply call cpulse.pulse()
  continuously to get continuous beat tracking.
*/

#include <python2.6/Python.h>
#include "cpulse.h"

float *_beats;

static PyObject * pulse(PyObject *self, PyObject *args) {
    _beats = cpulse_beats();
    return Py_BuildValue("i", (int) _beats[0] - 2);
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
