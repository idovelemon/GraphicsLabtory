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

static PyObject* ObjectAddObject(PyObject* self, PyObject* args) {
	const char* name;
	int ret;
	if (!PyArg_ParseTuple(args, "s", &name)) {
		 return NULL;
	}
	ret = ObjectAddObject(name);
	return Py_BuildValue("i", ret);
}

static PyObject* ObjectSetPos(PyObject* self, PyObject* args) {
	int id;
	float x;
	float y;
	float z;
	if (!PyArg_ParseTuple(args, "ifff", &id, &x, &y, &z)) {
		 return NULL;
	}
	ObjectSetPos(id, x, y, z);
	return Py_BuildValue("");
}

static PyMethodDef s_HostAPI_MethodDef[] = {
	{"TestPrintHelloWorld", TestPrintHelloWorld, METH_VARARGS, NULL},
	{"DebugPrint", DebugPrint, METH_VARARGS, NULL},
	{"ObjectAddObject", ObjectAddObject, METH_VARARGS, NULL},
	{"ObjectSetPos", ObjectSetPos, METH_VARARGS, NULL},
	{NULL, NULL, NULL, NULL}
};