#include "Python.h"
#include "topython.inl"

static PyModuleDef s_HostAPI_ModuleDef = {
    PyModuleDef_HEAD_INIT,
    "host_api",
    "host_api",
    -1,
    s_HostAPI_MethodDef,
    NULL,
    NULL,
    NULL,
    NULL
};

static PyObject* PyInit_Host_API() {
    return PyModule_Create(&s_HostAPI_ModuleDef);
}