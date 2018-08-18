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

static struct{
    char name[128];
    int value;
} s_IntConstant_Tbl[] = {
    // Collision Filter
    {"PlayerFilter", 1 << 7},
    {"EnemyFilter", 1 << 8},
};

static void PyInit_IntConstant(PyObject* module) {
    for (int i = 0; i < sizeof(s_IntConstant_Tbl)/sizeof(s_IntConstant_Tbl[0]); i++) {
        PyModule_AddIntConstant(module, s_IntConstant_Tbl[i].name, s_IntConstant_Tbl[i].value);
    }
}

static PyObject* PyInit_Host_API() {
    PyObject* module = PyModule_Create(&s_HostAPI_ModuleDef);
    PyInit_IntConstant(module);
    return module;
}