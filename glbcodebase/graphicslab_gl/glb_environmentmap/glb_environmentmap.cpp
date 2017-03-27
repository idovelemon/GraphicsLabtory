//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 22
// Brief: Demostrate the environment mapping
//----------------------------------------------------------------------
#include "glb_environmentmap.h"

class ApplicationEM : public glb::app::ApplicationBase {
public:
    ApplicationEM() {}
    virtual~ApplicationEM() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationEM;
    }

protected:
    int32_t m_Cube;

public:
    bool Initialize() {
        // Camera
        camera::ModelCamera* cam = camera::ModelCamera::Create(glb::Vector(0.0f, 0.0f, 130.0f), glb::Vector(0.0f, 0.0f, 0.0));
        glb::scene::Scene::SetCamera(glb::scene::PRIMIAY_CAM, cam);

        // Light
        light::Light light(light::PARALLEL_LIGHT);
        light.ambient = glb::Vector(0.2f, 0.2f, 0.2f);
        light.diffuse = glb::Vector(2.0f, 2.0f, 2.0f);
        light.specular = glb::Vector(10.0f, 10.0f, 10.0f);
        light.dir = glb::Vector(-1.0f, -1.0f, -1.0f);
        light.dir.Normalize();
        light.pow = 128.0f;
        glb::scene::Scene::SetLight(light, 0);

        // Perspective
        glb::render::Render::SetPerspective(glb::render::Render::PRIMARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 500.0f);

        // HDR
        glb::render::Render::SetExposureLevel(0.7f);
        glb::render::Render::SetLightAdaption(0.04f);

        glb::scene::Scene::AddSkyObject("sky.obj");

        m_Cube = glb::scene::Scene::AddObject("cube.obj");
        glb::Object* obj = glb::scene::Scene::GetObjectById(m_Cube);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(Vector(-200.0f, 0.0f, 0.0f));

        m_Cube = glb::scene::Scene::AddObject("cube.obj");
        obj = glb::scene::Scene::GetObjectById(m_Cube);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(Vector(200.0f, 0.0f, 0.0f));

        m_Cube = glb::scene::Scene::AddObject("cube.obj");
        obj = glb::scene::Scene::GetObjectById(m_Cube);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(Vector(0.0f, 200.0f, 0.0f));

        m_Cube = glb::scene::Scene::AddObject("cube.obj");
        obj = glb::scene::Scene::GetObjectById(m_Cube);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(Vector(0.0f, -200.0f, 0.0f));

        m_Cube = glb::scene::Scene::AddObject("cube.obj");
        obj = glb::scene::Scene::GetObjectById(m_Cube);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(Vector(0.0f, 0.0f, -200.0f));

        m_Cube = glb::scene::Scene::AddObject("cube.obj");
        obj = glb::scene::Scene::GetObjectById(m_Cube);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(Vector(0.0f, 0.0f, 200.0f));

        m_Cube = glb::scene::Scene::AddObject("ball.obj");
        obj = glb::scene::Scene::GetObjectById(m_Cube);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(Vector(0.0f, 0.0f, 0.0f));
        int32_t ref_tex = glb::render::Render::RequestBakeEnvMap(1024, 1024, obj);
        obj->SetTexWithId(glb::Model::MT_REFLECT, ref_tex);

        return true;
    }

    void Update(float dt) {
        glb::profile::ProfileTime time;
        time.BeginProfile();

        // Update scene
        camera::ModelCamera* model_cam = reinterpret_cast<camera::ModelCamera*>(scene::Scene::GetCurCamera());
        float rot = 1.0f;
        model_cam->Rotate(rot);
        glb::scene::Scene::Update();

        // Draw scene
        glb::render::Render::Draw();

        time.EndProfile();
        char buf[128];
        sprintf_s(buf, "%f\n", time.GetProfileTimeInMs());
        OutputDebugStringA(buf);
    }

    void Destroy() {
    }
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_environmentmap", sizeof(L"glb_environmentmap"));
    config.screen_width = 800;
    config.screen_height = 600;
    if (!glb::app::Application::Initialize(ApplicationEM::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}