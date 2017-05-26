//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 23
// Brief: Interface that will be used in python file
// Note: When you add a new function, you must run the topython.py script
//------------------------------------------------------------------
#ifndef PYSCRIPT_TOPYTHON_FUNCTION_H_
#define PYSCRIPT_TOPYTHON_FUNCTION_H_

// Debug
void DebugPrint(const char* s);

// Entity
int EntityCreate();
void EntityAddTransformCom(int id,
                           float px, float py, float pz,
                           float rx, float ry, float rz,
                           float sx, float sy, float sz);
void EntityAddRenderCom(int id, const char* name);
void EntityAddCameraCom(int id, float px, float py, float pz, float tx, float ty, float tz);

#endif  // PYSCRIPT_TOPYTHON_FUNCTION_H_