//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 12
// Brief: Demostrate application support
//----------------------------------------------------------------------
#include "glb_application.h"
#include "resource.h"

class ApplicationDemo : public glb::app::ApplicationBase {
public:
    ApplicationDemo() {}
    virtual~ApplicationDemo() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationDemo;
    }

public:
    bool Initialize() {
        return true;
    }

    void Update(float dt) {
        render::Device::SetClearColor(rand() % 100 / 100.0f, rand() % 100 / 100.0f, rand() % 100 / 100.0f);
        render::Device::Clear(render::CLEAR_COLOR);
        render::Device::SwapBuffer();
    }

    void Destroy() {
    }
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_application", sizeof(L"glb_application"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.platform_api = glb::app::PA_OPENGL;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationDemo::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}