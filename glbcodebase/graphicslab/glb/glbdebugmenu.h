//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 11 / 28
// Brief: Debug Menu System
//----------------------------------------------------------------------
#ifndef GLB_GLBDEBUGMENU_H_
#define GLB_GLBDEBUGMENU_H_

#include <stdint.h>

namespace glb {

namespace debugmenu {

//-----------------------------------------------------------------------------------
// Mgr DECLARATION
//----------------------------------------------------------------------------------
class Mgr {
public:
    static void Initialize();
    static void Update();
    static void Destroy();

    static void AddMenuItem(const char* menu, bool& trigger);
    static void AddMenuItem(const char* menu, int32_t& value, int32_t min, int32_t max);
    static void AddMenuItem(const char* menu, float& value, float step, float min, float max, const char* format);
};

};  // namespace debugmenu

};  // namespace glb

#endif  // GLB_GLBDEBUGMENU_H_