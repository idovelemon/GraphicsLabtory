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

static PyObject* EntitySetDead(PyObject* self, PyObject* args) {
	int id;
	int dead;
	if (!PyArg_ParseTuple(args, "ii", &id, &dead)) {
		 return NULL;
	}
	EntitySetDead(id, dead);
	return Py_BuildValue("");
}

static PyObject* EntityIsDead(PyObject* self, PyObject* args) {
	int id;
	int ret;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	ret = EntityIsDead(id);
	return Py_BuildValue("i", ret);
}

static PyObject* EntityAddRefCount(PyObject* self, PyObject* args) {
	int id;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	EntityAddRefCount(id);
	return Py_BuildValue("");
}

static PyObject* EntityDecRefCount(PyObject* self, PyObject* args) {
	int id;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	EntityDecRefCount(id);
	return Py_BuildValue("");
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
	float px;
	float py;
	float pz;
	float rx;
	float ry;
	float rz;
	float sx;
	float sy;
	float sz;
	bool enableInstance;
	int maxInstanceNum;
	if (!PyArg_ParseTuple(args, "isfffffffffbi", &id, &name, &px, &py, &pz, &rx, &ry, &rz, &sx, &sy, &sz, &enableInstance, &maxInstanceNum)) {
		 return NULL;
	}
	EntityAddRenderCom(id, name, px, py, pz, rx, ry, rz, sx, sy, sz, enableInstance, maxInstanceNum);
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

static PyObject* EntityAddScriptCom(PyObject* self, PyObject* args) {
	int id;
	const char* script;
	if (!PyArg_ParseTuple(args, "is", &id, &script)) {
		 return NULL;
	}
	EntityAddScriptCom(id, script);
	return Py_BuildValue("");
}

static PyObject* EntityAddRoleCom(PyObject* self, PyObject* args) {
	int id;
	int mt;
	int st;
	if (!PyArg_ParseTuple(args, "iii", &id, &mt, &st)) {
		 return NULL;
	}
	EntityAddRoleCom(id, mt, st);
	return Py_BuildValue("");
}

static PyObject* EntityAddArsenalCom(PyObject* self, PyObject* args) {
	int id;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	EntityAddArsenalCom(id);
	return Py_BuildValue("");
}

static PyObject* EntityAddDataCom(PyObject* self, PyObject* args) {
	int id;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	EntityAddDataCom(id);
	return Py_BuildValue("");
}

static PyObject* EntityAddCollisionCom(PyObject* self, PyObject* args) {
	int id;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	EntityAddCollisionCom(id);
	return Py_BuildValue("");
}

static PyObject* EntityGetPosX(PyObject* self, PyObject* args) {
	int id;
	float ret;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	ret = EntityGetPosX(id);
	return Py_BuildValue("f", ret);
}

static PyObject* EntityGetPosY(PyObject* self, PyObject* args) {
	int id;
	float ret;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	ret = EntityGetPosY(id);
	return Py_BuildValue("f", ret);
}

static PyObject* EntityGetPosZ(PyObject* self, PyObject* args) {
	int id;
	float ret;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	ret = EntityGetPosZ(id);
	return Py_BuildValue("f", ret);
}

static PyObject* EntitySetPos(PyObject* self, PyObject* args) {
	int id;
	float x;
	float y;
	float z;
	if (!PyArg_ParseTuple(args, "ifff", &id, &x, &y, &z)) {
		 return NULL;
	}
	EntitySetPos(id, x, y, z);
	return Py_BuildValue("");
}

static PyObject* EntityGetRotX(PyObject* self, PyObject* args) {
	int id;
	float ret;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	ret = EntityGetRotX(id);
	return Py_BuildValue("f", ret);
}

static PyObject* EntityGetRotY(PyObject* self, PyObject* args) {
	int id;
	float ret;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	ret = EntityGetRotY(id);
	return Py_BuildValue("f", ret);
}

static PyObject* EntityGetRotZ(PyObject* self, PyObject* args) {
	int id;
	float ret;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	ret = EntityGetRotZ(id);
	return Py_BuildValue("f", ret);
}

static PyObject* EntitySetRot(PyObject* self, PyObject* args) {
	int id;
	float x;
	float y;
	float z;
	if (!PyArg_ParseTuple(args, "ifff", &id, &x, &y, &z)) {
		 return NULL;
	}
	EntitySetRot(id, x, y, z);
	return Py_BuildValue("");
}

static PyObject* EntityGetScaleX(PyObject* self, PyObject* args) {
	int id;
	float ret;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	ret = EntityGetScaleX(id);
	return Py_BuildValue("f", ret);
}

static PyObject* EntityGetScaleY(PyObject* self, PyObject* args) {
	int id;
	float ret;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	ret = EntityGetScaleY(id);
	return Py_BuildValue("f", ret);
}

static PyObject* EntityGetScaleZ(PyObject* self, PyObject* args) {
	int id;
	float ret;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	ret = EntityGetScaleZ(id);
	return Py_BuildValue("f", ret);
}

static PyObject* EntitySetScale(PyObject* self, PyObject* args) {
	int id;
	float x;
	float y;
	float z;
	if (!PyArg_ParseTuple(args, "ifff", &id, &x, &y, &z)) {
		 return NULL;
	}
	EntitySetScale(id, x, y, z);
	return Py_BuildValue("");
}

static PyObject* EntityTransformSetParent(PyObject* self, PyObject* args) {
	int id;
	int parent;
	if (!PyArg_ParseTuple(args, "ii", &id, &parent)) {
		 return NULL;
	}
	EntityTransformSetParent(id, parent);
	return Py_BuildValue("");
}

static PyObject* EntityAddWeapon(PyObject* self, PyObject* args) {
	int id;
	int wt;
	const char* script;
	if (!PyArg_ParseTuple(args, "iis", &id, &wt, &script)) {
		 return NULL;
	}
	EntityAddWeapon(id, wt, script);
	return Py_BuildValue("");
}

static PyObject* EntityActiveWeapon(PyObject* self, PyObject* args) {
	int id;
	int wt;
	if (!PyArg_ParseTuple(args, "ii", &id, &wt)) {
		 return NULL;
	}
	EntityActiveWeapon(id, wt);
	return Py_BuildValue("");
}

static PyObject* EntityGetWeaponBulletNum(PyObject* self, PyObject* args) {
	int id;
	int ret;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	ret = EntityGetWeaponBulletNum(id);
	return Py_BuildValue("i", ret);
}

static PyObject* EntitySetWeaponBulletNum(PyObject* self, PyObject* args) {
	int id;
	int num;
	if (!PyArg_ParseTuple(args, "ii", &id, &num)) {
		 return NULL;
	}
	EntitySetWeaponBulletNum(id, num);
	return Py_BuildValue("");
}

static PyObject* EntityMoveToTarget(PyObject* self, PyObject* args) {
	int id;
	float speed;
	float tx;
	float ty;
	float tz;
	if (!PyArg_ParseTuple(args, "iffff", &id, &speed, &tx, &ty, &tz)) {
		 return NULL;
	}
	EntityMoveToTarget(id, speed, tx, ty, tz);
	return Py_BuildValue("");
}

static PyObject* EntityFindCloestEnemy(PyObject* self, PyObject* args) {
	int id;
	int ret;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	ret = EntityFindCloestEnemy(id);
	return Py_BuildValue("i", ret);
}

static PyObject* EntityShoot(PyObject* self, PyObject* args) {
	int id;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	EntityShoot(id);
	return Py_BuildValue("");
}

static PyObject* EntityAddIntData(PyObject* self, PyObject* args) {
	int id;
	const char* name;
	int data;
	if (!PyArg_ParseTuple(args, "isi", &id, &name, &data)) {
		 return NULL;
	}
	EntityAddIntData(id, name, data);
	return Py_BuildValue("");
}

static PyObject* EntityAddFloatData(PyObject* self, PyObject* args) {
	int id;
	const char* name;
	float data;
	if (!PyArg_ParseTuple(args, "isf", &id, &name, &data)) {
		 return NULL;
	}
	EntityAddFloatData(id, name, data);
	return Py_BuildValue("");
}

static PyObject* EntityAddStringData(PyObject* self, PyObject* args) {
	int id;
	const char* name;
	const char* data;
	if (!PyArg_ParseTuple(args, "iss", &id, &name, &data)) {
		 return NULL;
	}
	EntityAddStringData(id, name, data);
	return Py_BuildValue("");
}

static PyObject* EntityGetIntData(PyObject* self, PyObject* args) {
	int id;
	const char* name;
	int ret;
	if (!PyArg_ParseTuple(args, "is", &id, &name)) {
		 return NULL;
	}
	ret = EntityGetIntData(id, name);
	return Py_BuildValue("i", ret);
}

static PyObject* EntityGetFloatData(PyObject* self, PyObject* args) {
	int id;
	const char* name;
	float ret;
	if (!PyArg_ParseTuple(args, "is", &id, &name)) {
		 return NULL;
	}
	ret = EntityGetFloatData(id, name);
	return Py_BuildValue("f", ret);
}

static PyObject* EntitySetIntData(PyObject* self, PyObject* args) {
	int id;
	const char* name;
	int data;
	if (!PyArg_ParseTuple(args, "isi", &id, &name, &data)) {
		 return NULL;
	}
	EntitySetIntData(id, name, data);
	return Py_BuildValue("");
}

static PyObject* EntitySetFloatData(PyObject* self, PyObject* args) {
	int id;
	const char* name;
	float data;
	if (!PyArg_ParseTuple(args, "isf", &id, &name, &data)) {
		 return NULL;
	}
	EntitySetFloatData(id, name, data);
	return Py_BuildValue("");
}

static PyObject* EntitySetRotateByLookVec(PyObject* self, PyObject* args) {
	int id;
	float x;
	float y;
	float z;
	if (!PyArg_ParseTuple(args, "ifff", &id, &x, &y, &z)) {
		 return NULL;
	}
	EntitySetRotateByLookVec(id, x, y, z);
	return Py_BuildValue("");
}

static PyObject* EntitySetScaleByLookVec(PyObject* self, PyObject* args) {
	int id;
	float x;
	float y;
	float z;
	if (!PyArg_ParseTuple(args, "ifff", &id, &x, &y, &z)) {
		 return NULL;
	}
	EntitySetScaleByLookVec(id, x, y, z);
	return Py_BuildValue("");
}

static PyObject* EntityCalcDistBetweenEntities(PyObject* self, PyObject* args) {
	int e0;
	int e1;
	float ret;
	if (!PyArg_ParseTuple(args, "ii", &e0, &e1)) {
		 return NULL;
	}
	ret = EntityCalcDistBetweenEntities(e0, e1);
	return Py_BuildValue("f", ret);
}

static PyObject* EntitySetDrawEnable(PyObject* self, PyObject* args) {
	int id;
	int display;
	if (!PyArg_ParseTuple(args, "ii", &id, &display)) {
		 return NULL;
	}
	EntitySetDrawEnable(id, display);
	return Py_BuildValue("");
}

static PyObject* EntityBeginIterate(PyObject* self, PyObject* args) {
	EntityBeginIterate();
	return Py_BuildValue("");
}

static PyObject* EntityIterate(PyObject* self, PyObject* args) {
	int ret;
	ret = EntityIterate();
	return Py_BuildValue("i", ret);
}

static PyObject* EntityEndIterate(PyObject* self, PyObject* args) {
	EntityEndIterate();
	return Py_BuildValue("");
}

static PyObject* EntityIsMainType(PyObject* self, PyObject* args) {
	int id;
	int main_type;
	int ret;
	if (!PyArg_ParseTuple(args, "ii", &id, &main_type)) {
		 return NULL;
	}
	ret = EntityIsMainType(id, main_type);
	return Py_BuildValue("i", ret);
}

static PyObject* EntityIsSubType(PyObject* self, PyObject* args) {
	int id;
	int sub_type;
	int ret;
	if (!PyArg_ParseTuple(args, "ii", &id, &sub_type)) {
		 return NULL;
	}
	ret = EntityIsSubType(id, sub_type);
	return Py_BuildValue("i", ret);
}

static PyObject* EntityUpdateCollision(PyObject* self, PyObject* args) {
	int id;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	EntityUpdateCollision(id);
	return Py_BuildValue("");
}

static PyObject* EntityCheckCollision(PyObject* self, PyObject* args) {
	int id;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	EntityCheckCollision(id);
	return Py_BuildValue("");
}

static PyObject* EntityCollisionBeginIterate(PyObject* self, PyObject* args) {
	int id;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	EntityCollisionBeginIterate(id);
	return Py_BuildValue("");
}

static PyObject* EntityCollisionIterate(PyObject* self, PyObject* args) {
	int id;
	int ret;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	ret = EntityCollisionIterate(id);
	return Py_BuildValue("i", ret);
}

static PyObject* EntityCollisionEndIterate(PyObject* self, PyObject* args) {
	int id;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	EntityCollisionEndIterate(id);
	return Py_BuildValue("");
}

static PyObject* EntityGetCollisionWidth(PyObject* self, PyObject* args) {
	int id;
	float ret;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	ret = EntityGetCollisionWidth(id);
	return Py_BuildValue("f", ret);
}

static PyObject* EntityGetCollisionLength(PyObject* self, PyObject* args) {
	int id;
	float ret;
	if (!PyArg_ParseTuple(args, "i", &id)) {
		 return NULL;
	}
	ret = EntityGetCollisionLength(id);
	return Py_BuildValue("f", ret);
}

static PyObject* EntityFindEntity(PyObject* self, PyObject* args) {
	int main;
	int sub;
	int ret;
	if (!PyArg_ParseTuple(args, "ii", &main, &sub)) {
		 return NULL;
	}
	ret = EntityFindEntity(main, sub);
	return Py_BuildValue("i", ret);
}

static PyObject* TimeGetPrevGameTime(PyObject* self, PyObject* args) {
	float ret;
	ret = TimeGetPrevGameTime();
	return Py_BuildValue("f", ret);
}

static PyObject* TimeGetCurGameTime(PyObject* self, PyObject* args) {
	float ret;
	ret = TimeGetCurGameTime();
	return Py_BuildValue("f", ret);
}

static PyObject* TimeGetGameFrameSpeed(PyObject* self, PyObject* args) {
	float ret;
	ret = TimeGetGameFrameSpeed();
	return Py_BuildValue("f", ret);
}

static PyObject* InputGetPointAtEntity(PyObject* self, PyObject* args) {
	int ret;
	ret = InputGetPointAtEntity();
	return Py_BuildValue("i", ret);
}

static PyObject* InputGetCursorPosX(PyObject* self, PyObject* args) {
	float ret;
	ret = InputGetCursorPosX();
	return Py_BuildValue("f", ret);
}

static PyObject* InputGetCursorPosY(PyObject* self, PyObject* args) {
	float ret;
	ret = InputGetCursorPosY();
	return Py_BuildValue("f", ret);
}

static PyObject* InputHasKeyPressed(PyObject* self, PyObject* args) {
	int key;
	int ret;
	if (!PyArg_ParseTuple(args, "i", &key)) {
		 return NULL;
	}
	ret = InputHasKeyPressed(key);
	return Py_BuildValue("i", ret);
}

static PyObject* InputHasMousePressed(PyObject* self, PyObject* args) {
	int mouse;
	int ret;
	if (!PyArg_ParseTuple(args, "i", &mouse)) {
		 return NULL;
	}
	ret = InputHasMousePressed(mouse);
	return Py_BuildValue("i", ret);
}

static PyMethodDef s_HostAPI_MethodDef[] = {
	{"DebugPrint", DebugPrint, METH_VARARGS, NULL},
	{"EntityCreate", EntityCreate, METH_VARARGS, NULL},
	{"EntitySetDead", EntitySetDead, METH_VARARGS, NULL},
	{"EntityIsDead", EntityIsDead, METH_VARARGS, NULL},
	{"EntityAddRefCount", EntityAddRefCount, METH_VARARGS, NULL},
	{"EntityDecRefCount", EntityDecRefCount, METH_VARARGS, NULL},
	{"EntityAddTransformCom", EntityAddTransformCom, METH_VARARGS, NULL},
	{"EntityAddRenderCom", EntityAddRenderCom, METH_VARARGS, NULL},
	{"EntityAddCameraCom", EntityAddCameraCom, METH_VARARGS, NULL},
	{"EntityAddScriptCom", EntityAddScriptCom, METH_VARARGS, NULL},
	{"EntityAddRoleCom", EntityAddRoleCom, METH_VARARGS, NULL},
	{"EntityAddArsenalCom", EntityAddArsenalCom, METH_VARARGS, NULL},
	{"EntityAddDataCom", EntityAddDataCom, METH_VARARGS, NULL},
	{"EntityAddCollisionCom", EntityAddCollisionCom, METH_VARARGS, NULL},
	{"EntityGetPosX", EntityGetPosX, METH_VARARGS, NULL},
	{"EntityGetPosY", EntityGetPosY, METH_VARARGS, NULL},
	{"EntityGetPosZ", EntityGetPosZ, METH_VARARGS, NULL},
	{"EntitySetPos", EntitySetPos, METH_VARARGS, NULL},
	{"EntityGetRotX", EntityGetRotX, METH_VARARGS, NULL},
	{"EntityGetRotY", EntityGetRotY, METH_VARARGS, NULL},
	{"EntityGetRotZ", EntityGetRotZ, METH_VARARGS, NULL},
	{"EntitySetRot", EntitySetRot, METH_VARARGS, NULL},
	{"EntityGetScaleX", EntityGetScaleX, METH_VARARGS, NULL},
	{"EntityGetScaleY", EntityGetScaleY, METH_VARARGS, NULL},
	{"EntityGetScaleZ", EntityGetScaleZ, METH_VARARGS, NULL},
	{"EntitySetScale", EntitySetScale, METH_VARARGS, NULL},
	{"EntityTransformSetParent", EntityTransformSetParent, METH_VARARGS, NULL},
	{"EntityAddWeapon", EntityAddWeapon, METH_VARARGS, NULL},
	{"EntityActiveWeapon", EntityActiveWeapon, METH_VARARGS, NULL},
	{"EntityGetWeaponBulletNum", EntityGetWeaponBulletNum, METH_VARARGS, NULL},
	{"EntitySetWeaponBulletNum", EntitySetWeaponBulletNum, METH_VARARGS, NULL},
	{"EntityMoveToTarget", EntityMoveToTarget, METH_VARARGS, NULL},
	{"EntityFindCloestEnemy", EntityFindCloestEnemy, METH_VARARGS, NULL},
	{"EntityShoot", EntityShoot, METH_VARARGS, NULL},
	{"EntityAddIntData", EntityAddIntData, METH_VARARGS, NULL},
	{"EntityAddFloatData", EntityAddFloatData, METH_VARARGS, NULL},
	{"EntityAddStringData", EntityAddStringData, METH_VARARGS, NULL},
	{"EntityGetIntData", EntityGetIntData, METH_VARARGS, NULL},
	{"EntityGetFloatData", EntityGetFloatData, METH_VARARGS, NULL},
	{"EntitySetIntData", EntitySetIntData, METH_VARARGS, NULL},
	{"EntitySetFloatData", EntitySetFloatData, METH_VARARGS, NULL},
	{"EntitySetRotateByLookVec", EntitySetRotateByLookVec, METH_VARARGS, NULL},
	{"EntitySetScaleByLookVec", EntitySetScaleByLookVec, METH_VARARGS, NULL},
	{"EntityCalcDistBetweenEntities", EntityCalcDistBetweenEntities, METH_VARARGS, NULL},
	{"EntitySetDrawEnable", EntitySetDrawEnable, METH_VARARGS, NULL},
	{"EntityBeginIterate", EntityBeginIterate, METH_VARARGS, NULL},
	{"EntityIterate", EntityIterate, METH_VARARGS, NULL},
	{"EntityEndIterate", EntityEndIterate, METH_VARARGS, NULL},
	{"EntityIsMainType", EntityIsMainType, METH_VARARGS, NULL},
	{"EntityIsSubType", EntityIsSubType, METH_VARARGS, NULL},
	{"EntityUpdateCollision", EntityUpdateCollision, METH_VARARGS, NULL},
	{"EntityCheckCollision", EntityCheckCollision, METH_VARARGS, NULL},
	{"EntityCollisionBeginIterate", EntityCollisionBeginIterate, METH_VARARGS, NULL},
	{"EntityCollisionIterate", EntityCollisionIterate, METH_VARARGS, NULL},
	{"EntityCollisionEndIterate", EntityCollisionEndIterate, METH_VARARGS, NULL},
	{"EntityGetCollisionWidth", EntityGetCollisionWidth, METH_VARARGS, NULL},
	{"EntityGetCollisionLength", EntityGetCollisionLength, METH_VARARGS, NULL},
	{"EntityFindEntity", EntityFindEntity, METH_VARARGS, NULL},
	{"TimeGetPrevGameTime", TimeGetPrevGameTime, METH_VARARGS, NULL},
	{"TimeGetCurGameTime", TimeGetCurGameTime, METH_VARARGS, NULL},
	{"TimeGetGameFrameSpeed", TimeGetGameFrameSpeed, METH_VARARGS, NULL},
	{"InputGetPointAtEntity", InputGetPointAtEntity, METH_VARARGS, NULL},
	{"InputGetCursorPosX", InputGetCursorPosX, METH_VARARGS, NULL},
	{"InputGetCursorPosY", InputGetCursorPosY, METH_VARARGS, NULL},
	{"InputHasKeyPressed", InputHasKeyPressed, METH_VARARGS, NULL},
	{"InputHasMousePressed", InputHasMousePressed, METH_VARARGS, NULL},
	{NULL, NULL, NULL, NULL}
};