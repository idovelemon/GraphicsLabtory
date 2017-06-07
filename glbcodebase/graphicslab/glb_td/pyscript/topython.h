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
    // RoleMainType
    {"RMT_NONE", -1},
    {"RMT_PLAYER", 0},
    {"RMT_ENEMY", 1},
    {"RMT_OPERATOR", 2},
    {"RMT_ITEM", 3},
    {"RMT_MAX", 4},

    // RoleSubType
    {"RST_NONE", -1},
    {"RST_MAINTOWER", 0},  // Player Start
    {"RST_SUBTOWER", 1},
    {"RST_WALL", 2},
    {"RST_BULLET_LASER", 3},
    {"RST_BULLET_MACHINEGUN", 4},
    {"RST_BULLET_BOMB", 5},
    {"RST_CRYSTAL", 6},

    {"RST_E01", 101},  // Enemy Start
    {"RST_E02", 102},

    {"RST_PLACER", 201},  // Operator Start

    {"RST_ITEM_BOMB", 301},  // Item Start

    // WeaponType
    {"WT_NONE", -1},
    {"WT_LASER", 0},
    {"WT_MACHINEGUN", 1},

    // Button Mouse
    {"BM_LEFT", 0x0},
    {"BM_RIGHT", 0x1},
    {"BM_CENTER", 0x2},

    // Button Key
    {"BK_1", 0x02},
    {"BK_UP", 0xC8},
    {"BK_LEFT", 0xCB},
    {"BK_RIGHT", 0xCD},
    {"BK_DOWN", 0xD0},
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