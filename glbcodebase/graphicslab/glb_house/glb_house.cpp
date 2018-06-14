//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 06 / 14
//----------------------------------------------------------------------
#include "glb_house.h"
#include "resource.h"

class ApplicationHouse: public glb::app::ApplicationBase {
public:
    ApplicationHouse() {}
    virtual~ApplicationHouse() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationHouse;
    }

public:
    bool Initialize() {
        // Camera
        scene::FreeCamera* cam = scene::FreeCamera::Create(math::Vector(0.0f, 100.0f, 300.0f), math::Vector(0.0f, 0.0f, 0.0));
        scene::Scene::SetCamera(glb::scene::PRIMIAY_CAM, cam);

        // Light
        scene::Light ambient(scene::AMBIENT_LIGHT);
        ambient.color = math::Vector(0.1f, 0.1f, 0.1f);
        scene::Scene::SetLight(ambient, 0);

        scene::Light light(scene::PARALLEL_LIGHT);
        light.color = math::Vector(1.2f, 1.2f, 1.2f);
        light.dir = math::Vector(-1.0f, -1.0f, 1.0f);
        light.dir.Normalize();
        scene::Scene::SetLight(light, 1);

        // Perspective
        render::Render::SetPerspective(glb::render::Render::PRIMARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 500.0f);

        // HDR
        glb::render::Render::SetHighLightBase(1.5f);

        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        glb::Input::Update();

        // Update scene
        math::Vector pos = glb::scene::Scene::GetCurCamera()->GetPos();
        math::Vector target = glb::scene::Scene::GetCurCamera()->GetTarget();
        math::Vector look = pos - target;
        math::Matrix rotCamera;
        rotCamera.MakeRotateYMatrix(Input::GetMouseMoveX() * 0.1f);
        look = rotCamera * look;
        pos = target + look;
        glb::scene::Scene::GetCurCamera()->SetPos(pos);
        glb::scene::Scene::GetCurCamera()->Update(1.0f);

        // Update scene
        scene::Light lit = scene::Scene::GetLight(1);
        float rotY = 1.0f;
        math::Matrix rot;
        rot.MakeRotateYMatrix(rotY);
        lit.dir = rot * lit.dir;
        //glb::scene::Scene::SetLight(lit, 1);
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
    memcpy(config.caption, L"glb_House", sizeof(L"glb_House"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    if (!glb::app::Application::Initialize(ApplicationHouse::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}