//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 22
// Brief: Demostrate the environment mapping
//----------------------------------------------------------------------
#include "glb_environmentmap.h"
#include "resource.h"

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
        scene::ModelCamera* cam = scene::ModelCamera::Create(math::Vector(0.0f, 0.0f, 130.0f), math::Vector(0.0f, 0.0f, 0.0));
        glb::scene::Scene::SetCamera(glb::scene::PRIMIAY_CAM, cam);

        // Light
        scene::Light light(scene::PARALLEL_LIGHT);
        light.ambient = math::Vector(0.2f, 0.2f, 0.2f);
        light.diffuse = math::Vector(2.0f, 2.0f, 2.0f);
        light.specular = math::Vector(10.0f, 10.0f, 10.0f);
        light.dir = math::Vector(-1.0f, -1.0f, -1.0f);
        light.dir.Normalize();
        light.pow = 128.0f;
        glb::scene::Scene::SetLight(light, 0);

        // Perspective
        glb::render::Render::SetPerspective(glb::render::Render::PRIMARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 500.0f);

        // HDR
        glb::render::Render::SetExposureLevel(0.7f);
        glb::render::Render::SetLightAdaption(0.04f);
        glb::render::Render::SetHighLightBase(0.9f);

        glb::scene::Scene::AddSkyObject("sky.obj");

        m_Cube = glb::scene::Scene::AddObject("cube_no_light.obj");
        scene::Object* obj = scene::Scene::GetObjectById(m_Cube);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(math::Vector(-200.0f, 0.0f, 0.0f));

        m_Cube = glb::scene::Scene::AddObject("cube_no_light.obj");
        obj = glb::scene::Scene::GetObjectById(m_Cube);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(math::Vector(200.0f, 0.0f, 0.0f));

        m_Cube = glb::scene::Scene::AddObject("cube_no_light.obj");
        obj = glb::scene::Scene::GetObjectById(m_Cube);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(math::Vector(0.0f, 200.0f, 0.0f));

        m_Cube = glb::scene::Scene::AddObject("cube_no_light.obj");
        obj = glb::scene::Scene::GetObjectById(m_Cube);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(math::Vector(0.0f, -200.0f, 0.0f));

        m_Cube = glb::scene::Scene::AddObject("cube_no_light.obj");
        obj = glb::scene::Scene::GetObjectById(m_Cube);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(math::Vector(0.0f, 0.0f, -200.0f));

        m_Cube = glb::scene::Scene::AddObject("cube_no_light.obj");
        obj = glb::scene::Scene::GetObjectById(m_Cube);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(math::Vector(0.0f, 0.0f, 200.0f));

        m_Cube = glb::scene::Scene::AddObject("ball.obj");
        //m_Cube = glb::scene::Scene::AddObject("cube.obj");
        obj = glb::scene::Scene::GetObjectById(m_Cube);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(math::Vector(0.0f, 0.0f, 0.0f));
        int32_t ref_tex = glb::render::Render::RequestBakeEnvMap(1024, 1024, obj);
        obj->SetTexWithId(scene::Model::MT_REFLECT, ref_tex);

        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        // Update scene
        scene::ModelCamera* model_cam = reinterpret_cast<scene::ModelCamera*>(scene::Scene::GetCurCamera());
        float rot = 1.0f;
        model_cam->Rotate(rot);
        scene::Scene::Update();

        // Draw scene
        render::Render::Draw();

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
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationEM::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}