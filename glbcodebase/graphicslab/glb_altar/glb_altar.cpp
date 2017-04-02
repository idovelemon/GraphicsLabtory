//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 03 / 31
// Brief: Display The Altar scene
//----------------------------------------------------------------------
#include "glb_altar.h"

class ApplicationAltar : public glb::app::ApplicationBase {
public:
    ApplicationAltar() {}
    virtual~ApplicationAltar() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationAltar;
    }

protected:
    int32_t m_Cubes[4];
    int32_t m_Ball;

public:
    bool Initialize() {
        // Camera
        camera::ModelCamera* cam = camera::ModelCamera::Create(glb::Vector(0.0f, 2.3f, 1.5f), glb::Vector(0.0f, 2.2f, 0.0f));
        //camera::FreeCamera* cam = camera::FreeCamera::Create(glb::Vector(0.0f, 2.0f, 2.0f), glb::Vector(0.0f, 0.0f, 0.0));
        glb::scene::Scene::SetCamera(glb::scene::PRIMIAY_CAM, cam);

        // Light
        light::Light light(light::PARALLEL_LIGHT);
        light.ambient = glb::Vector(0.1f, 0.1f, 0.1f);
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
        glb::render::Render::SetLightAdaption(0.0f);
        glb::render::Render::SetHighLightBase(0.95f);

        // Add sky
        glb::scene::Scene::AddSkyObject("res/sky.obj");

        // Add ground
        int32_t ground = glb::scene::Scene::AddObject("res/ground.obj");
        glb::Object* obj = glb::scene::Scene::GetObjectById(ground);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(Vector(0.0f, 0.0f, 0.0f));

        // Add center base
        int32_t center_base = glb::scene::Scene::AddObject("res/center_base.obj");
        obj = glb::scene::Scene::GetObjectById(center_base);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(Vector(0.0f, 1.0f, 0.0f));

        // Add other base
        int32_t other_base = glb::scene::Scene::AddObject("res/other_base.obj");
        obj = glb::scene::Scene::GetObjectById(other_base);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(Vector(3.0f, 1.0f, 0.0f));

        other_base = glb::scene::Scene::AddObject("res/other_base.obj");
        obj = glb::scene::Scene::GetObjectById(other_base);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(Vector(-3.0f, 1.0f, 0.0f));

        other_base = glb::scene::Scene::AddObject("res/other_base.obj");
        obj = glb::scene::Scene::GetObjectById(other_base);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(Vector(0.0f, 1.0f, 3.0f));

        other_base = glb::scene::Scene::AddObject("res/other_base.obj");
        obj = glb::scene::Scene::GetObjectById(other_base);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(Vector(0.0f, 1.0f, -3.0f));

        // Add cubes
        m_Cubes[0] = glb::scene::Scene::AddObject("res/cube.obj");
        obj = glb::scene::Scene::GetObjectById(m_Cubes[0]);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(Vector(3.0f, 2.3f, 0.0f));

        m_Cubes[1] = glb::scene::Scene::AddObject("res/cube.obj");
        obj = glb::scene::Scene::GetObjectById(m_Cubes[1]);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(Vector(-3.0f, 2.3f, 0.0f));

        m_Cubes[2] = glb::scene::Scene::AddObject("res/cube.obj");
        obj = glb::scene::Scene::GetObjectById(m_Cubes[2]);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(Vector(0.0f, 2.3f, 3.0f));

        m_Cubes[3] = glb::scene::Scene::AddObject("res/cube.obj");
        obj = glb::scene::Scene::GetObjectById(m_Cubes[3]);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(Vector(0.0f, 2.3f, -3.0f));

        // Add ball
        m_Ball = glb::scene::Scene::AddObject("res/ball.obj");
        obj = glb::scene::Scene::GetObjectById(m_Ball);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(glb::render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(Vector(0.0f, 2.2f, 0.0f));
        int32_t em = glb::render::Render::RequestBakeEnvMap(256, 256, obj);
        obj->SetTexWithId(glb::Model::MT_REFLECT, em);

        return true;
    }

    void Update(float dt) {
        glb::profile::ProfileTime time;
        time.BeginProfile();

        // Update scene
        UpdateCamera();
        UpdateObjects();
        glb::scene::Scene::Update();

        // Draw scene
        glb::render::Render::Draw();

        time.EndProfile();
        char buf[128];
        sprintf_s(buf, "%f\n", time.GetProfileTimeInMs());
        OutputDebugStringA(buf);
    }

    void UpdateCamera() {
        camera::ModelCamera* model_cam = reinterpret_cast<camera::ModelCamera*>(scene::Scene::GetCurCamera());
        float rot = 0.5f;
        model_cam->Rotate(rot);
        //camera::FreeCamera* cam = reinterpret_cast<camera::FreeCamera*>(scene::Scene::GetCurCamera());
        //
        //POINT cursor_pos;
        //GetCursorPos(&cursor_pos);
        //static int last_pos_x = cursor_pos.x, last_pos_y = cursor_pos.y;
        //int diff_x = cursor_pos.x - last_pos_x;
        //int diff_y = cursor_pos.y - last_pos_y;
        //last_pos_x = cursor_pos.x;
        //last_pos_y = cursor_pos.y;
        //cam->Rotate(diff_y * 0.1f, diff_x * 0.1f);
        //float mx = 0.0f, my = 0.0f, mz = 0.0f;
        //if (GetKeyState('A') & 0x8000) {
        //    mx = -0.01f;
        //} else if (GetKeyState('D') & 0x8000) {
        //    mx = 0.01f;
        //}
        //if (GetKeyState('W') & 0x8000) {
        //    mz = 0.01f;
        //} else if (GetKeyState('S') & 0x8000) {
        //    mz = -0.01f;
        //}
        //cam->Move(mx, 0.0f, mz);
    }

    void UpdateObjects() {
        float rot = 1.0f;
        for (int32_t i = 0; i < 4; i++) {
            Object* obj = glb::scene::Scene::GetObjectById(m_Cubes[i]);
            Vector rot_v = obj->GetRotation();
            rot_v.x += rot;
            rot_v.y += rot;
            rot_v.z += rot;
            obj->SetRotation(rot_v);
        }
    }

    void Destroy() {
    }
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_altar", sizeof(L"glb_altar"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 2048;
    config.shadow_map_height = 2048;
    if (!glb::app::Application::Initialize(ApplicationAltar::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}