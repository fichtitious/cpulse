#include <python2.6/Python.h>
#include "../include/cpulse.h"

char _started = 0;
float *_pulses;

static PyObject * pulse(PyObject *self, PyObject *args) {

    if (!_started) {
        cpulse_start();
        _started = 1;
    }

    _pulses = cpulse_pulse();
    return Py_BuildValue("f", _pulses[0]);

}

static PyMethodDef CPulseMethods[] = {
    {"pulse", pulse, METH_VARARGS, "get current pulse"},
    {NULL,    NULL,  0,            NULL}
};

PyMODINIT_FUNC initcpulse(void) {
    (void) Py_InitModule("cpulse", CPulseMethods);
    Py_AtExit(&cpulse_stop);
}
