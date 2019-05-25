//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2019. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2019 / 05 / 24
// Brief: Demostrate the Multisampler Anti Aliasing(MSAA)
//----------------------------------------------------------------------
#include "glb_msaa.h"
#include "resource.h"

class ApplicationMSAA : public glb::app::ApplicationBase {
public:
    ApplicationMSAA() {}
    virtual~ApplicationMSAA() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationMSAA;
    }

public:
    bool Initialize() {
        // Camera
        scene::FreeCamera* cam = scene::FreeCamera::Create(math::Vector(0.0f, 0.5f, 0.5f), math::Vector(0.0f, 0.0f, 0.0f));
        scene::Scene::SetCamera(glb::scene::PRIMIAY_CAM, cam);

        // Light
        scene::Light ambientLight(scene::AMBIENT_LIGHT);
        ambientLight.color = math::Vector(0.1f, 0.1f, 0.1f);
        scene::Scene::SetLight(ambientLight, 0);

        scene::Light paralleLight(scene::PARALLEL_LIGHT);
        paralleLight.color = math::Vector(1.5f, 1.5f, 0.75f);
        paralleLight.dir = math::Vector(-1.0f, -1.0f, -1.0f);
        paralleLight.dir.Normalize();
        scene::Scene::SetLight(paralleLight, 1);

        // Perspective
        render::Render::SetPerspective(glb::render::Render::PRIMARY_PERS, 60.0f, 800 * 1.0f / 600, 0.01f, 10000.0f);

        // HDR
        render::Render::SetHighLightBase(0.9f);

        // Objects
        int32_t cube = scene::Scene::AddObject("res/grid.obj", "res/cube.mat");
        scene::Scene::GetObjectById(cube)->SetCullFaceEnable(true);
        scene::Scene::GetObjectById(cube)->SetCullFaceMode(glb::render::CULL_BACK);
        scene::Scene::GetObjectById(cube)->SetDepthTestEnable(true);
        scene::Scene::GetObjectById(cube)->SetWorldMatrix(math::Matrix::CreateIdentityMatrix());
        m_Cube = cube;

        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        glb::Input::Update();

        // Update scene
        scene::Light lit = glb::scene::Scene::GetLight(1);
        float rotY = 1.0f;
        math::Matrix rotLight;
        rotLight.MakeRotateYMatrix(rotY);
        lit.dir = rotLight * lit.dir;
        glb::scene::Scene::SetLight(lit, 1);

        // Rotate the model
        glb::scene::Scene::GetObjectById(m_Cube)->SetWorldMatrix(math::Matrix::CreateRotateXYZMatrix(-90.0f, 45.0f, 0.0f));

        glb::scene::Scene::Update();

        math::Vector pos = glb::scene::Scene::GetCurCamera()->GetPos();
        math::Vector target = glb::scene::Scene::GetCurCamera()->GetTarget();
        math::Vector look = pos - target;
        math::Matrix rotCamera;
        rotCamera.MakeRotateYMatrix(Input::GetMouseMoveX() * 0.1f);
        look = rotCamera * look;
        pos = target + look;
        glb::scene::Scene::GetCurCamera()->SetPos(pos);
        glb::scene::Scene::GetCurCamera()->Update(1.0f);

        // Draw scene
        glb::render::Render::Draw();

        time.EndProfile();
        printf("%f\n", time.GetProfileTimeInMs());
    }

    void Destroy() {
    }

protected:
    int32_t m_Cube;
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_msaa", sizeof(L"glb_msaa"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.decalMapWidth = 1024;
    config.decalMapHeight = 1024;
    config.msaaSamplerNum = 16;  // Change MSAA quality
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationMSAA::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}