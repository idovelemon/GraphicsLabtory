//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 02 / 16
// Brief: Demostrate the screen space ambient occlusion
//----------------------------------------------------------------------
#include "glb_ssao.h"

class ApplicationSSAO : public glb::app::ApplicationBase {
public:
    ApplicationSSAO() {}
    virtual~ApplicationSSAO() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationSSAO;
    }

public:
    bool Initialize() {
        // Camera
        camera::FreeCamera* cam = camera::FreeCamera::Create(glb::Vector(0.0f, 75.0f, 115.0f), glb::Vector(0.0f, 0.0f, 0.0f));
        glb::scene::Scene::SetCamera(glb::scene::PRIMIAY_CAM, cam);

        // Light
        light::Light light(light::PARALLEL_LIGHT);
        light.ambient = glb::Vector(0.5f, 0.5f, 0.5f);
        light.diffuse = glb::Vector(0.0f, 0.0f, 0.0f);
        light.specular = glb::Vector(0.0f, 0.0f, 0.0f);
        light.dir = glb::Vector(-1.0f, -1.0f, 1.0f);
        light.dir.Normalize();
        light.pow = 128.0f;
        glb::scene::Scene::SetLight(light, 0);

        // Perspective
        glb::render::Render::SetPerspective(glb::render::Render::PRIMARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 500.0f);

        // HDR
        glb::render::Render::SetExposureLevel(0.7f);
        glb::render::Render::SetLightAdaption(0.04f);

        int32_t city = glb::scene::Scene::AddObject("city.obj");
        glb::scene::Scene::GetObjectById(city)->SetCullFaceEnable(true);
        glb::scene::Scene::GetObjectById(city)->SetCullFaceMode(glb::render::CULL_BACK);
        glb::scene::Scene::GetObjectById(city)->SetDepthTestEnable(true);
        glb::scene::Scene::GetObjectById(city)->SetPos(Vector(0.0, 0.0, 0.0));

        return true;
    }

    void Update(float dt) {
        glb::profile::ProfileTime time;
        time.BeginProfile();

        // Update scene
        glb::light::Light lit = glb::scene::Scene::GetLight(0);
        float rotY = 1.0f;
        Matrix rot;
        rot.MakeRotateYMatrix(rotY);
        lit.dir = rot * lit.dir;
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
    memcpy(config.caption, L"glb_ssao", sizeof(L"glb_ssao"));
    config.screen_width = 800;
    config.screen_height = 600;
    if (!glb::app::Application::Initialize(ApplicationSSAO::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}