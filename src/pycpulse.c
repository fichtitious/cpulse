/*
  This is a Python extension module that wraps cpulse.  cpulse is started
  when you import the module in Python and stopped when the module gets
  garbage-collected.

  cpulse.pulse() returns a number between -1 and 1 indicating how much
  of a peak the latest sound coming through your speakers was.

  As in cpulse.c, there is no threading.  You simply call cpulse.pulse()
  continuously to get continuous beat tracking.
*/

#include <python2.6/Python.h>
#include "cpulse.h"

static PyObject * pulse(PyObject *self, PyObject *args) {
    return Py_BuildValue("f", *cpulse_pulse());
}

static PyMethodDef CPulseMethods[] = {
    {"pulse", pulse, METH_VARARGS, "returns a value between -1 and 1 "
        "indicating how much of a peak the latest audio level going "
        "out through your speakers was"},
    {NULL,    NULL,  0,            NULL}
};

PyMODINIT_FUNC initcpulse(void) {
    (void) Py_InitModule("cpulse", CPulseMethods);
    cpulse_start();
    Py_AtExit(&cpulse_stop);
}
