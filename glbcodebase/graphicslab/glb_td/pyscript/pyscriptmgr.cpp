//--------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[133266012@qq.com]
// Date: 2016 / 07 / 09
// Brief: The manager to control the python game script system
//--------------------------------------------------------------------------------
#include "pyscriptmgr.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include <map>
#include <string>

#include "Python.h"

#include "topython.h"

namespace pyscript {

//---------------------------------------------------------------------------
// @brief: Constant/Variable
//---------------------------------------------------------------------------
class PyScriptMgrImp;
static PyScriptMgrImp* s_Imp = NULL;

//---------------------------------------------------------------------------
// @brief: Class/Typedef
//---------------------------------------------------------------------------
class PyScriptMgrImp {
 public:
     PyScriptMgrImp();
     virtual ~PyScriptMgrImp();

 public:
     void Initialize(const char* script_root_path);
     void LoadScript(const char* script_file_name);
     void RunScript(const char* script_file_name);
     void Destroy();

 private:
     std::map<std::string, PyObject*> m_ScriptDatabase;
};

//---------------------------------------------------------------------------
// @brief: Implementation
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// @brief: PyScriptMgrImp
//---------------------------------------------------------------------------
PyScriptMgrImp::PyScriptMgrImp() {
    m_ScriptDatabase.clear();
}

PyScriptMgrImp::~PyScriptMgrImp() {
    m_ScriptDatabase.clear();
}

void PyScriptMgrImp::Initialize(const char* script_root_path) {
    bool is_initialized = false;

    Py_Initialize();
    if (Py_IsInitialized()) {
        do {
            // Add new path
            char add_path_py[256];
            sprintf_s(add_path_py, "sys.path.append(\"%s\")", script_root_path);
            PyRun_SimpleString("import sys");
            PyRun_SimpleString(add_path_py);

            // Add host api
            int ret = PyImport_AppendInittab("host_api", &PyInit_Host_API);

            is_initialized = true;

            // Do not call Py_XDECREF unless you have called Py_XINCREF
        } while (false);
    } else {
        assert(false && "Initialize python failed");
    }

    assert(is_initialized && "Initialize pyscriptmgr failed");
}

void PyScriptMgrImp::LoadScript(const char* script_file_name) {
    if (script_file_name) {
        bool has_error = false;
        do {
            PyObject* name = PyUnicodeUCS2_FromString(script_file_name);
            if (name == NULL) {
                has_error = true;
                break;
            }

            PyObject* module = PyImport_Import(name);
            if (module == NULL) {
                has_error = true;
                break;
            }

            PyObject* dict = PyModule_GetDict(module);
            if (dict == NULL) {
                has_error = true;
                break;
            }

            PyObject* func = PyDict_GetItemString(dict, "main");
            if (func == NULL) {
                has_error = true;
                break;
            }

            m_ScriptDatabase.insert(std::pair<std::string, PyObject*>(script_file_name, func));

            func = NULL;
            dict = NULL;
            module = NULL;
            name = NULL;
        } while (false);
        assert(has_error == false && "Something wrong");
    } else {
        assert(false && "Invalid paramter");
    }
}

void PyScriptMgrImp::RunScript(const char* script_file_name) {
    if (script_file_name) {
        std::map<std::string, PyObject*>::iterator it = m_ScriptDatabase.find(script_file_name);
        if (it != m_ScriptDatabase.end()) {
            PyObject* func = it->second;
            PyObject_CallObject(func, NULL);
            func = NULL;
        } else {
            assert(false && "Invalid script");
        }
    } else {
        assert(false && "Invalid paramter");
    }
}

void PyScriptMgrImp::Destroy() {
    m_ScriptDatabase.clear();
    Py_Finalize();
}

//---------------------------------------------------------------------------
// @brief: PyScriptMgr
//---------------------------------------------------------------------------
void PyScriptMgr::Initialize(const char* script_root_path) {
    if (s_Imp == NULL) {
        s_Imp = new PyScriptMgrImp();
        if (s_Imp != NULL) {
            s_Imp->Initialize(script_root_path);
        } else {
            assert(false && "Allocate memory failed");
        }
    }
}

void PyScriptMgr::LoadScript(const char* script_file_name) {
    if (s_Imp != NULL) {
        s_Imp->LoadScript(script_file_name);
    } else {
        assert(false && "Create implementation first");
    }
}

void PyScriptMgr::RunScript(const char* script_file_name) {
    if (s_Imp != NULL) {
        s_Imp->RunScript(script_file_name);
    } else {
        assert(false && "Create impelementation first");
    }
}

void PyScriptMgr::Destroy() {
    if (s_Imp != NULL) {
        s_Imp->Destroy();
        delete s_Imp;
        s_Imp = NULL;
    }
}

};  // namespace pyscript