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

enum PlatformAPI {
    PA_OPENGL = 0,
    PA_DX11,
    PA_MAX,
};

struct AppConfig {
    int32_t screen_width;
    int32_t screen_height;
    int32_t shadow_map_width;
    int32_t shadow_map_height;
    int32_t platform_api;
    wchar_t caption[128];

    AppConfig()
    : screen_width(0)
    , screen_height(0)
    , shadow_map_width(0)
    , shadow_map_height(0)
    , platform_api(0) {
        memset(caption, 0, sizeof(caption));
    }
};

class Application {
public:
    // Core
    static bool Initialize(APPLICATION_CREATOR creator, HINSTANCE hInstance, AppConfig config);
    static void Update();
    static void Destroy();

    static HWND GetWindowHandle();
    static int32_t GetWindowWidth();
    static int32_t GetWindowHeight();
    static int32_t GetShadowMapWidth();
    static int32_t GetShadowMapHeight();
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