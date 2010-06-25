/*
  This is a Python extension module that wraps cpulse.  cpulse is started
  when you import the module in Python and stopped when the module gets
  garbage-collected.

  In Python, cpulse.pulse() returns a tuple of flags indicating whether
  there's a bass peak and whether there's a treble peak in the latest
  sound coming through your speakers, from whatever source.

  As in cpulse.c, there is no threading.  You simply call cpulse.pulse()
  continuously to get continuous beat tracking.
*/

#include <python2.6/Python.h>
#include "cpulse.h"

peakdetector_t *_peakDetector;

static PyObject * pulse(PyObject *self, PyObject *args) {
    _peakDetector = cpulse_pulse();
    return Py_BuildValue( "(ii)", _peakDetector->isBassPeak, _peakDetector->isTreblePeak );
}

static PyMethodDef CPulseMethods[] = {
    {"pulse", pulse, METH_VARARGS, "returns flags indicating whether "
        "the latest sound coming through your speakers was a bass "
        "and/or treble peak"},
    {NULL, NULL, 0, NULL} // sentinel
};

PyMODINIT_FUNC initcpulse(void) {
    (void) Py_InitModule("cpulse", CPulseMethods);
    cpulse_start();
    Py_AtExit(&cpulse_stop);
}
