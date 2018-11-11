//---------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 07 / 23
// Brief: Demonstrate the shadow mapping
//---------------------------------------------------------------------
#include "glb_shadowmap.h"
#include "resource.h"

class ApplicationShadowMap : public glb::app::ApplicationBase {
public:
    ApplicationShadowMap() {}
    virtual~ApplicationShadowMap() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationShadowMap;
    }

public:
    bool Initialize() {
        // Camera
        scene::FreeCamera* cam = scene::FreeCamera::Create(math::Vector(0.0f, 0.0f, 200.0f), math::Vector(0.0f, 0.0f, 0.0));
        glb::scene::Scene::SetCamera(glb::scene::PRIMIAY_CAM, cam);

        // Light
        // Light
        scene::Light light;
        light.type = scene::AMBIENT_LIGHT;
        light.color = math::Vector(0.4f, 0.4f, 0.4f);
        scene::Scene::SetLight(light, 0);

        light.type = scene::PARALLEL_LIGHT;
        light.color = math::Vector(2.0f, 2.0f, 2.0f);
        light.dir = math::Vector(-1.0f, -1.0f, 0.0f);
        light.dir.Normalize();
        scene::Scene::SetLight(light, 1);

        // Perspective
        glb::render::Render::SetPerspective(glb::render::Render::PRIMARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 500.0f);

        // HDR

        glb::scene::Scene::AddSkyObject("sky.obj");

        int32_t floor = glb::scene::Scene::AddObject("floor.obj");
        glb::scene::Scene::GetObjectById(floor)->SetCullFaceEnable(true);
        glb::scene::Scene::GetObjectById(floor)->SetCullFaceMode(glb::render::CULL_BACK);
        glb::scene::Scene::GetObjectById(floor)->SetDepthTestEnable(true);
        glb::scene::Scene::GetObjectById(floor)->SetWorldMatrix(math::Matrix::CreateTranslateMatrix(0.0, 0.0, 0.0));

        int32_t cube = glb::scene::Scene::AddObject("cube.obj");
        glb::scene::Scene::GetObjectById(cube)->SetCullFaceEnable(true);
        glb::scene::Scene::GetObjectById(cube)->SetCullFaceMode(glb::render::CULL_BACK);
        glb::scene::Scene::GetObjectById(cube)->SetDepthTestEnable(true);
        glb::scene::Scene::GetObjectById(cube)->SetWorldMatrix(math::Matrix::CreateTranslateMatrix(70.0f, 50.0f, 0.0f));

        cube = glb::scene::Scene::AddObject("cube.obj");
        glb::scene::Scene::GetObjectById(cube)->SetCullFaceEnable(true);
        glb::scene::Scene::GetObjectById(cube)->SetCullFaceMode(glb::render::CULL_BACK);
        glb::scene::Scene::GetObjectById(cube)->SetDepthTestEnable(true);
        glb::scene::Scene::GetObjectById(cube)->SetWorldMatrix(math::Matrix::CreateTranslateMatrix(-70.0f, 50.0f, 0.0f));
        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        // Update scene
        scene::Light lit = glb::scene::Scene::GetLight(0);
        float rotY = 1.0f;
        math::Matrix rot;
        rot.MakeRotateYMatrix(rotY);
        lit.dir = rot * lit.dir;
        glb::scene::Scene::SetLight(lit, 0);
        glb::scene::Scene::Update();

        // Draw scene
        glb::render::Render::Draw();

        time.EndProfile();
        printf("%f\n", time.GetProfileTimeInMs());
    }

    void Destroy() {
    }
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_shadowmap", sizeof(L"glb_shadowmap"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.decalMapWidth = 1024;
    config.decalMapHeight = 1024;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationShadowMap::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}