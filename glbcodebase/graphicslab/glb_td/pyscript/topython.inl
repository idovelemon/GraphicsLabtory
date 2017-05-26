#include "topython_function.h"

static PyObject* DebugPrint(PyObject* self, PyObject* args) {
	const char* s;
	if (!PyArg_ParseTuple(args, "s", &s)) {
		 return NULL;
	}
	DebugPrint(s);
	return Py_BuildValue("");
}

static PyObject* EntityCreate(PyObject* self, PyObject* args) {
	int ret;
	ret = EntityCreate();
	return Py_BuildValue("i", ret);
}

static PyObject* EntityAddTransformCom(PyObject* self, PyObject* args) {
	int id;
	float px;
	float py;
	float pz;
	float rx;
	float ry;
	float rz;
	float sx;
	float sy;
	float sz;
	if (!PyArg_ParseTuple(args, "ifffffffff", &id, &px, &py, &pz, &rx, &ry, &rz, &sx, &sy, &sz)) {
		 return NULL;
	}
	EntityAddTransformCom(id, px, py, pz, rx, ry, rz, sx, sy, sz);
	return Py_BuildValue("");
}

static PyObject* EntityAddRenderCom(PyObject* self, PyObject* args) {
	int id;
	const char* name;
	if (!PyArg_ParseTuple(args, "is", &id, &name)) {
		 return NULL;
	}
	EntityAddRenderCom(id, name);
	return Py_BuildValue("");
}

static PyObject* EntityAddCameraCom(PyObject* self, PyObject* args) {
	int id;
	float px;
	float py;
	float pz;
	float tx;
	float ty;
	float tz;
	if (!PyArg_ParseTuple(args, "iffffff", &id, &px, &py, &pz, &tx, &ty, &tz)) {
		 return NULL;
	}
	EntityAddCameraCom(id, px, py, pz, tx, ty, tz);
	return Py_BuildValue("");
}

static PyMethodDef s_HostAPI_MethodDef[] = {
	{"DebugPrint", DebugPrint, METH_VARARGS, NULL},
	{"EntityCreate", EntityCreate, METH_VARARGS, NULL},
	{"EntityAddTransformCom", EntityAddTransformCom, METH_VARARGS, NULL},
	{"EntityAddRenderCom", EntityAddRenderCom, METH_VARARGS, NULL},
	{"EntityAddCameraCom", EntityAddCameraCom, METH_VARARGS, NULL},
	{NULL, NULL, NULL, NULL}
};