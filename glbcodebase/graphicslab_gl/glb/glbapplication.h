//-------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 12
// Brief: Define the application to manage detail of win32
//-------------------------------------------------------------------
#ifndef GLB_GLBAPPLICATION_H_
#define GLB_GLBAPPLICATION_H_

#include <Windows.h>

#include <stdint.h>

namespace glb {
    namespace app {
        class ApplicationBase;
    };
};

typedef glb::app::ApplicationBase* (*APPLICATION_CREATOR)();

namespace glb {

namespace app {

class Application {
public:
    // Core
    static bool Initialize(
        APPLICATION_CREATOR creator
        , HINSTANCE hInstance
        , int32_t width, int32_t height
        , const wchar_t* caption);
    static void Update();
    static void Destroy();

    static HWND GetWindowHandle();
};

class ApplicationBase {
public:
    ApplicationBase() {
    }

    virtual~ApplicationBase() {
    }

    virtual bool Initialize() = 0;
    virtual void Update(float dt) = 0;
    virtual void Destroy() = 0;
};

};  // namespace app

};  // namespace glb

#endif  // GLB_GLBAPPLICATION_H_