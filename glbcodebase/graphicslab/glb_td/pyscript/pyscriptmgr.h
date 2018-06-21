//--------------------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[133266012@qq.com]
// Date: 2016 / 07 / 09
// Brief: The manager to control the python game script system
//--------------------------------------------------------------------------------
#ifndef PYSCRIPT_PYSCRIPTMGR_H_
#define PYSCRIPT_PYSCRIPTMGR_H_

#include <stdint.h>

namespace pyscript {

class PyScriptMgr {
public:
     //---------------------------------------------------------------
     // @brief: Initialize the game script system
     // @param: script_root_path The main.py's file path
     // @warning: Please use "../../"'s path format
     //---------------------------------------------------------------
     static void Initialize(const char* script_root_path);

     //---------------------------------------------------------------
     // @brief: Load the script
     //---------------------------------------------------------------
     static void LoadScript(const char* script_file_name);

     //---------------------------------------------------------------
     // @brief: Run the script with script name
     //---------------------------------------------------------------
     static void RunScript(const char* script_file_name);
     static void RunScript(const char* script_file_name, int arg);

     //---------------------------------------------------------------
     // @brief: Return the name's value inf config.py
     //---------------------------------------------------------------
     static float GetValueF(const char* name);

     static void Destroy();
};

};  // namespace pyscript

#endif  // PYSCRIPT_PYSCRIPTMGR_H_