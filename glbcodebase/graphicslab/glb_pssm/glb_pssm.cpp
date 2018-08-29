//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 04 / 03
// Brief: Add Parallel Split Shadow Map to GLB Library
//----------------------------------------------------------------------
#include "glb_pssm.h"

#include "resource.h"

class ApplicationPSSM : public glb::app::ApplicationBase {
public:
    ApplicationPSSM() {}
    virtual~ApplicationPSSM() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationPSSM;
    }

protected:

public:
    bool Initialize() {
        // Camera
        //camera::ModelCamera* cam = camera::ModelCamera::Create(glb::Vector(0.0f, 2.3f, 1.5f), glb::Vector(0.0f, 2.2f, 0.0f));
        scene::FreeCamera* cam = scene::FreeCamera::Create(math::Vector(0.0f, 10.0f, 10.0f), math::Vector(3.586983442f, 6.26142120f, -3.62632370f));
        scene::Scene::SetCamera(scene::PRIMIAY_CAM, cam);
        cam = scene::FreeCamera::Create(math::Vector(-5.0f, 10.0f, 10.0f), math::Vector(0.0f, 0.0f, 0.0));
        scene::Scene::SetCamera(scene::SECONDARY_CAM, cam);

        // Light
        scene::Light ambientLight(scene::AMBIENT_LIGHT);
        ambientLight.color = math::Vector(0.1f, 0.1f, 0.1f);
        scene::Scene::SetLight(ambientLight, 0);

        scene::Light light(scene::PARALLEL_LIGHT);
        light.color = math::Vector(1.2f, 1.2f, 1.2f);
        light.dir = math::Vector(-1.0f, -1.0f, 0.0f);
        light.dir.Normalize();
        scene::Scene::SetLight(light, 1);

        // Perspective
        render::Render::SetPerspective(render::Render::PRIMARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 100.0f);
        render::Render::SetPerspective(render::Render::SECONDARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 10000.0f);

        // HDR
        render::Render::SetHighLightBase(0.95f);

        // Add ground
        int32_t ground = scene::Scene::AddObject("res/level0.obj");
        scene::Object* obj = scene::Scene::GetObjectById(ground);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(math::Vector(0.0f, -1.0f, 0.0f));

        int32_t num = 4;
        for (int32_t i = 0; i < num; i++) {
            for (int32_t j = 0; j < num; j++) {
                // Add object
                int32_t tree_header = scene::Scene::AddObject("res/coreRot.obj");
                scene::Object* obj = scene::Scene::GetObjectById(tree_header);
                obj->SetCullFaceEnable(true);
                obj->SetCullFaceMode(render::CULL_BACK);
                obj->SetDepthTestEnable(true);
                obj->SetPos(math::Vector(0.0f - (j - num/2) * 20.0f, 0.0f, 0.0f - (i - num/2) * 20.0f));
            }
        }

        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        // Update scene
        UpdateCamera();
        scene::Scene::Update();

        // Draw scene
        render::Render::Draw();

        time.EndProfile();
        char buf[128];
        sprintf_s(buf, "%f\n", time.GetProfileTimeInMs());
        OutputDebugStringA(buf);
    }

    void UpdateCamera() {
        if (GetKeyState(VK_F1) & 0x8000) {
            scene::Scene::SetCurCamera(scene::PRIMIAY_CAM);
            render::Render::SetCurPerspectiveType(render::Render::PRIMARY_PERS);
        } else if (GetKeyState(VK_F2) & 0x8000) {
            scene::Scene::SetCurCamera(scene::SECONDARY_CAM);
            render::Render::SetCurPerspectiveType(render::Render::SECONDARY_PERS);
        }
        //camera::ModelCamera* model_cam = reinterpret_cast<camera::ModelCamera*>(scene::Scene::GetCurCamera());
        //float rot = 0.5f;
        //model_cam->Rotate(rot);
        scene::FreeCamera* cam = reinterpret_cast<scene::FreeCamera*>(scene::Scene::GetCurCamera());
        
        POINT cursor_pos;
        GetCursorPos(&cursor_pos);
        static int last_pos_x = cursor_pos.x, last_pos_y = cursor_pos.y;
        int diff_x = cursor_pos.x - last_pos_x;
        int diff_y = cursor_pos.y - last_pos_y;
        last_pos_x = cursor_pos.x;
        last_pos_y = cursor_pos.y;
        cam->Rotate(diff_y * 0.1f, diff_x * 0.1f);
        float mx = 0.0f, my = 0.0f, mz = 0.0f;
        if (GetKeyState('A') & 0x8000) {
            mx = -0.1f;
        } else if (GetKeyState('D') & 0x8000) {
            mx = 0.1f;
        }
        if (GetKeyState('W') & 0x8000) {
            mz = 0.1f;
        } else if (GetKeyState('S') & 0x8000) {
            mz = -0.1f;
        }
        cam->Move(mx, 0.0f, mz);
    }

    void Destroy() {
    }
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_pssm", sizeof(L"glb_pssm"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 2048;
    config.shadow_map_height = 2048;
    config.decalMapWidth = 1024;
    config.decalMapHeight = 1024;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationPSSM::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}