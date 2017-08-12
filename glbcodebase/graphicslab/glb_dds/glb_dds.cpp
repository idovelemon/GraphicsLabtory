//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 08 / 07
// Brief: Add support for DDS file format
//----------------------------------------------------------------------
#include "glb_dds.h"
#include "resource.h"

class ApplicationDDS : public glb::app::ApplicationBase {
public:
    ApplicationDDS() {
    }

    virtual~ApplicationDDS() {
        Destroy();
    }

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationDDS;
    }

public:
    bool Initialize() {
        // Camera
        scene::FreeCamera* cam = scene::FreeCamera::Create(math::Vector(0.0f, 0.0f, 160.0f), math::Vector(0.0f, 0.0f, 0.0f));
        glb::scene::Scene::SetCamera(glb::scene::PRIMIAY_CAM, cam);

        // Light
        scene::Light light(scene::PARALLEL_LIGHT);
        light.ambient = math::Vector(0.1f, 0.1f, 0.1f);
        light.diffuse = math::Vector(0.5f, 0.5f, 0.5f);
        light.specular = math::Vector(10.0f, 10.0f, 10.0f);
        light.dir = math::Vector(-1.0f, -1.0f, -1.0f);
        light.dir.Normalize();
        light.pow = 100.0f;
        glb::scene::Scene::SetLight(light, 0);

        // Perspective
        glb::render::Render::SetPerspective(glb::render::Render::PRIMARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 500.0f);

        // HDR
        glb::render::Render::SetExposureLevel(0.7f);
        glb::render::Render::SetLightAdaption(0.04f);
        glb::render::Render::SetHighLightBase(0.9f);

        // Objects
        int32_t cube = glb::scene::Scene::AddObject("res/cube.obj");
        glb::scene::Scene::GetObjectById(cube)->SetCullFaceEnable(true);
        glb::scene::Scene::GetObjectById(cube)->SetCullFaceMode(glb::render::CULL_BACK);
        glb::scene::Scene::GetObjectById(cube)->SetDepthTestEnable(true);
        glb::scene::Scene::GetObjectById(cube)->SetPos(math::Vector(0.0f, 0.0f, 0.0f));

        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        // Update scene
        glb::scene::Scene::Update();

        // Draw scene
        glb::render::Render::Draw();

        time.EndProfile();
        printf("%f\n", time.GetProfileTimeInMs());
    }

    void Destroy() {
    }

private:
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_dds", sizeof(L"glb_dds"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationDDS::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    FreeConsole();

    return 0;
}