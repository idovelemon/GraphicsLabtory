#include "topython_function.h"

static PyObject* TestPrintHelloWorld(PyObject* self, PyObject* args) {
	TestPrintHelloWorld();
	return Py_BuildValue("");
}

static PyObject* DebugPrint(PyObject* self, PyObject* args) {
	const char* s;
	if (!PyArg_ParseTuple(args, "s", &s)) {
		 return NULL;
	}
	DebugPrint(s);
	return Py_BuildValue("");
}

static PyMethodDef s_HostAPI_MethodDef[] = {
	{"TestPrintHelloWorld", TestPrintHelloWorld, METH_VARARGS, NULL},
	{"DebugPrint", DebugPrint, METH_VARARGS, NULL},
	{NULL, NULL, NULL, NULL}
};