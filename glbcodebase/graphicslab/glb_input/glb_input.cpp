//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2017. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2017 / 05 / 02
// Brief: Add DirectInput
//----------------------------------------------------------------------
#include "glb_input.h"

#include "resource.h"

class ApplicationInput : public glb::app::ApplicationBase {
public:
    ApplicationInput() {}
    virtual~ApplicationInput() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationInput;
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
        scene::Light light(scene::PARALLEL_LIGHT);
        light.ambient = math::Vector(1.0f, 1.0f, 1.0f);
        light.diffuse = math::Vector(2.0f, 2.0f, 2.0f);
        light.specular = math::Vector(1.0f, 1.0f, 1.0f);
        light.dir = math::Vector(-1.0f, -1.0f, 0.0f);
        light.dir.Normalize();
        light.pow = 128.0f;
        scene::Scene::SetLight(light, 0);

        // Perspective
        render::Render::SetPerspective(render::Render::PRIMARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 100.0f);
        render::Render::SetPerspective(render::Render::SECONDARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 10000.0f);

        // HDR
        render::Render::SetExposureLevel(1.0f);
        render::Render::SetLightAdaption(0.1f);
        render::Render::SetHighLightBase(0.95f);

        // Add ground
        int32_t ground = scene::Scene::AddObject("res/ground.obj");
        scene::Object* obj = scene::Scene::GetObjectById(ground);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        obj->SetPos(math::Vector(0.0f, -1.0f, 0.0f));

        int32_t num = 4;
        for (int32_t i = 0; i < num; i++) {
            for (int32_t j = 0; j < num; j++) {
                // Add object
                int32_t tree_header = scene::Scene::AddObject("res/tree_header.obj");
                scene::Object* obj = scene::Scene::GetObjectById(tree_header);
                obj->SetCullFaceEnable(true);
                obj->SetCullFaceMode(render::CULL_BACK);
                obj->SetDepthTestEnable(true);
                obj->SetPos(math::Vector(0.0f - (j - num/2) * 20.0f, 5.0f, 0.0f - (i - num/2) * 20.0f));

                int32_t tree_trunk = scene::Scene::AddObject("res/tree_trunk.obj");
                obj = scene::Scene::GetObjectById(tree_trunk);
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
        Input::Update();
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
        if (Input::IsKeyboardButtonPressed(BK_F1)) {
            scene::Scene::SetCurCamera(scene::PRIMIAY_CAM);
            render::Render::SetCurPerspectiveType(render::Render::PRIMARY_PERS);
        } else if (Input::IsKeyboardButtonPressed(BK_F2)) {
            scene::Scene::SetCurCamera(scene::SECONDARY_CAM);
            render::Render::SetCurPerspectiveType(render::Render::SECONDARY_PERS);
        }
        //camera::ModelCamera* model_cam = reinterpret_cast<camera::ModelCamera*>(scene::Scene::GetCurCamera());
        //float rot = 0.5f;
        //model_cam->Rotate(rot);
        scene::FreeCamera* cam = reinterpret_cast<scene::FreeCamera*>(scene::Scene::GetCurCamera());
        
        int diff_x = Input::GetMouseMoveX();
        int diff_y = Input::GetMouseMoveY();
        util::log::LogPrint("DiffX: %d DiffY:%d", diff_x, diff_y);
        cam->Rotate(diff_y * 0.1f, diff_x * 0.1f);
        float mx = 0.0f, my = 0.0f, mz = 0.0f;
        if (Input::IsKeyboardButtonPressed(BK_A)) {
            mx = -0.1f;
        } else if (Input::IsKeyboardButtonPressed(BK_D)) {
            mx = 0.1f;
        }
        if (Input::IsKeyboardButtonPressed(BK_W)) {
            mz = 0.1f;
        } else if (Input::IsKeyboardButtonPressed(BK_S)) {
            mz = -0.1f;
        }
        cam->Move(mx, 0.0f, mz);
    }

    void Destroy() {
    }
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_input", sizeof(L"glb_input"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 2048;
    config.shadow_map_height = 2048;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationInput::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}