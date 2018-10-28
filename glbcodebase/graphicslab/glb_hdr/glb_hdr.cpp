//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 01 / 14
// Brief: Demostrate the high dynamic range rendering
//----------------------------------------------------------------------
#include "glb_hdr.h"
#include "resource.h"

class ApplicationHDR : public glb::app::ApplicationBase {
public:
    ApplicationHDR() {}
    virtual~ApplicationHDR() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationHDR;
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

        //glb::scene::Scene::AddSkyObject("sky.obj");

        int32_t floor = glb::scene::Scene::AddObject("res/floor.obj");
        glb::scene::Scene::GetObjectById(floor)->SetCullFaceEnable(true);
        glb::scene::Scene::GetObjectById(floor)->SetCullFaceMode(glb::render::CULL_BACK);
        glb::scene::Scene::GetObjectById(floor)->SetDepthTestEnable(true);
        glb::scene::Scene::GetObjectById(floor)->SetWorldMatrix(math::Matrix::CreateIdentityMatrix());

        int32_t cube = glb::scene::Scene::AddObject("res/cube.obj");
        glb::scene::Scene::GetObjectById(cube)->SetCullFaceEnable(true);
        glb::scene::Scene::GetObjectById(cube)->SetCullFaceMode(glb::render::CULL_BACK);
        glb::scene::Scene::GetObjectById(cube)->SetDepthTestEnable(true);
        glb::scene::Scene::GetObjectById(cube)->SetWorldMatrix(math::Matrix::CreateTranslateMatrix(70.0f, 50.0f, 0.0f));

        cube = glb::scene::Scene::AddObject("res/cube.obj");
        glb::scene::Scene::GetObjectById(cube)->SetCullFaceEnable(true);
        glb::scene::Scene::GetObjectById(cube)->SetCullFaceMode(glb::render::CULL_BACK);
        glb::scene::Scene::GetObjectById(cube)->SetDepthTestEnable(true);
        glb::scene::Scene::GetObjectById(cube)->SetWorldMatrix(math::Matrix::CreateTranslateMatrix(-70.0f, 50.0f, 0.0f));

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
    memcpy(config.caption, L"glb_hdr", sizeof(L"glb_hdr"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.decalMapWidth = 1024;
    config.decalMapHeight = 1024;
    if (!glb::app::Application::Initialize(ApplicationHDR::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}