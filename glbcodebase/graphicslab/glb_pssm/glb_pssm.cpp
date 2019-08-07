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
        scene::FreeCamera* cam = scene::FreeCamera::Create(math::Vector(0.0f, 10.0f, 10.0f), math::Vector(0.0f, 10.0f, 0.0f));
        scene::Scene::SetCamera(scene::PRIMIAY_CAM, cam);
        cam = scene::FreeCamera::Create(math::Vector(-5.0f, 10.0f, 10.0f), math::Vector(0.0f, 0.0f, 0.0));
        scene::Scene::SetCamera(scene::SECONDARY_CAM, cam);

        // Light
        scene::Light ambientLight(scene::AMBIENT_LIGHT);
        ambientLight.color = math::Vector(0.1f, 0.1f, 0.1f);
        scene::Scene::SetLight(ambientLight, 0);

        scene::Light light(scene::PARALLEL_LIGHT);
        light.color = math::Vector(1.2f, 1.2f, 1.2f);
        light.dir = math::Vector(-1.0f, -2.0f, 1.0f);
        light.dir.Normalize();
        scene::Scene::SetLight(light, 1);

        // Perspective
        render::Render::SetPerspective(render::Render::PRIMARY_PERS, 45.0f, 800 * 1.0f / 600, 0.01f, 1000.0f);
        render::Render::SetPerspective(render::Render::SECONDARY_PERS, 45.0f, 800 * 1.0f / 600, 0.01f, 1000.0f);

        // HDR
        render::Render::SetHighLightBase(0.95f);

        // Add ground
        int32_t ground = scene::Scene::AddObject("res/level0.obj", "res/level0.mat");
        scene::Object* obj = scene::Scene::GetObjectById(ground);
        obj->SetCullFaceEnable(true);
        obj->SetCullFaceMode(render::CULL_BACK);
        obj->SetDepthTestEnable(true);
        math::Matrix groundMat = math::Matrix::CreateIdentityMatrix();
        groundMat.Scale(5.0f, 1.0f, 5.0f);
        groundMat.Translate(0.0f, -1.0f, 0.0f);
        obj->SetWorldMatrix(groundMat);

        int32_t numi = 20;
        int32_t numj = 8;
        for (int32_t i = 0; i < numi; i++) {
            for (int32_t j = 0; j < numj; j++) {
                // Add object
                int32_t tree_header = scene::Scene::AddObject("res/coreRot.obj", "res/coreRot.mat");
                scene::Object* obj = scene::Scene::GetObjectById(tree_header);
                obj->SetCullFaceEnable(true);
                obj->SetCullFaceMode(render::CULL_BACK);
                obj->SetDepthTestEnable(true);

                math::Matrix worldMat = math::Matrix::CreateIdentityMatrix();
                worldMat.Scale(3.0f, 10.0f, 3.0f);
                worldMat.Translate(0.0f - (j - numj/2) * 20.0f, 0.0f, 0.0f - (i - numi/2) * 20.0f);
                obj->SetWorldMatrix(worldMat);
            }
        }

        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        // Update input device
        Input::Update();

        // Update scene
        UpdateLight();
        UpdateCamera();
        scene::Scene::Update();

        // Draw scene
        render::Render::Draw();

        time.EndProfile();
        char buf[128];
        sprintf_s(buf, "%f\n", time.GetProfileTimeInMs());
        //OutputDebugStringA(buf);
    }

    void UpdateLight() {
        scene::Light light = scene::Scene::GetLight(1);
        math::Matrix rotateMat = math::Matrix::CreateRotateYMatrix(0.1f);
        light.dir = rotateMat * light.dir;
        //scene::Scene::SetLight(light, 1);
    }

    void UpdateCamera() {
        if (Input::IsKeyboardButtonPressed(BK_LCONTROL)) {
            if (Input::IsKeyboardButtonPressed(BK_F1)) {
                scene::Scene::SetCurCamera(scene::PRIMIAY_CAM);
                render::Render::SetCurPerspectiveType(render::Render::PRIMARY_PERS);
            } else if (Input::IsKeyboardButtonPressed(BK_F2)) {
                scene::Scene::SetCurCamera(scene::SECONDARY_CAM);
                render::Render::SetCurPerspectiveType(render::Render::SECONDARY_PERS);
            }
            scene::FreeCamera* cam = reinterpret_cast<scene::FreeCamera*>(scene::Scene::GetCurCamera());

            float diff_x = Input::GetMouseMoveX();
            float diff_y = Input::GetMouseMoveY();
            cam->Rotate(diff_y * 0.1f, diff_x * 0.1f);
            float mx = 0.0f, my = 0.0f, mz = 0.0f;
            if (Input::IsKeyboardButtonPressed(BK_A)) {
                mx = -0.5f;
            } else if (Input::IsKeyboardButtonPressed(BK_D)) {
                mx = 0.5f;
            }
            if (Input::IsKeyboardButtonPressed(BK_W)) {
                mz = 0.5f;
            } else if (Input::IsKeyboardButtonPressed(BK_S)) {
                mz = -0.5f;
            }
            cam->Move(mx, 0.0f, mz);
        }
    }

    void Destroy() {
    }
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_pssm", sizeof(L"glb_pssm"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.decalMapWidth = 1024;
    config.decalMapHeight = 1024;
    config.msaaSamplerNum = 16;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationPSSM::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}