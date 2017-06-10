//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 23
// Brief: A tower defense game for my girl
//----------------------------------------------------------------------
#include "glb_td.h"
#include "resource.h"

#include "gametimer.h"
#include "dynamic/dynamicworld.h"
#include "entity/entitymgr.h"
#include "pyscript/pyscriptmgr.h"

#if 0
#include "vld.h"
#endif

class ApplicationTd : public glb::app::ApplicationBase {
public:
    ApplicationTd() {}
    virtual~ApplicationTd() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationTd;
    }

protected:

public:
    bool Initialize() {
        // Light
        scene::Light light(scene::PARALLEL_LIGHT);
        light.ambient = math::Vector(1.0f, 1.0f, 1.0f);
        light.diffuse = math::Vector(2.0f, 2.0f, 2.0f);
        light.specular = math::Vector(100.0f, 100.0f, 100.0f);
        light.dir = math::Vector(-1.0f, -1.0f, -1.0f);
        light.dir.Normalize();
        light.pow = 128.0f;
        glb::scene::Scene::SetLight(light, 0);

        // Perspective
        glb::render::Render::SetPerspective(glb::render::Render::PRIMARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 500.0f);

        // HDR
        glb::render::Render::SetExposureLevel(0.3f);
        glb::render::Render::SetLightAdaption(1.0f);
        glb::render::Render::SetHighLightBase(0.95f);

        // Game Manager
        dynamic::DynamicWorld::Initialize();
        pyscript::PyScriptMgr::Initialize("res/script/");
        entity::EntityMgr::Initialize();
        td::GameTimer::Initialize();

        pyscript::PyScriptMgr::LoadScript("testlevel");
        pyscript::PyScriptMgr::RunScript("testlevel");
        pyscript::PyScriptMgr::LoadScript("timeline");

        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        glb::Input::Update();
        pyscript::PyScriptMgr::RunScript("timeline");

        // Update entity
        dynamic::DynamicWorld::Update();
        td::GameTimer::Update(dt);
        entity::EntityMgr::Update(dt);
        entity::EntityMgr::Draw();

        // Update scene
        glb::scene::Scene::Update();
        glb::render::Render::Draw();

        time.EndProfile();
        char buf[128];
        sprintf_s(buf, "%f\n", time.GetProfileTimeInMs());
        //printf(buf);
    }

    void Destroy() {
        td::GameTimer::Destroy();
        entity::EntityMgr::Destroy();
        pyscript::PyScriptMgr::Destroy();
        dynamic::DynamicWorld::Destroy();
    }
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_td", sizeof(L"glb_td"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 2048;
    config.shadow_map_height = 2048;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationTd::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    FreeConsole();

    return 0;
}