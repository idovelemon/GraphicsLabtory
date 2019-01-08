//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 01
// Brief: Demostrate the Normal Mapping
//----------------------------------------------------------------------
#include "glb_normalmap.h"
#include "resource.h"

class ApplicationNormalMap : public glb::app::ApplicationBase {
public:
    ApplicationNormalMap() {}
    virtual~ApplicationNormalMap() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationNormalMap;
    }

public:
    bool Initialize() {
        // Camera
        scene::FreeCamera* cam = scene::FreeCamera::Create(math::Vector(0.0f, 0.0f, 200.0f), math::Vector(0.0f, 0.0f, 0.0f));
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
        int32_t cube = scene::Scene::AddObject("res/cube.obj", "res/cube.mat");
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

        // Randomly Rotate the sphere
        static float sRot = 0.0f;
        sRot = sRot + 0.5f;
        glb::scene::Scene::GetObjectById(m_Cube)->SetWorldMatrix(math::Matrix::CreateRotateXYZMatrix(sRot, sRot, sRot));

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
    memcpy(config.caption, L"glb_normalmap", sizeof(L"glb_normalmap"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.decalMapWidth = 1024;
    config.decalMapHeight = 1024;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationNormalMap::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}