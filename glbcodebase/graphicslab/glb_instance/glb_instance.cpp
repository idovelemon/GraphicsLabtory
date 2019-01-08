//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 06 / 27
// Brief: Geometry Instanced
//----------------------------------------------------------------------
#include "glb_instance.h"
#include "resource.h"

#define ENABLE_INSTANCE_RENDERING (TRUE)

static const int32_t kInstanceNum = 21;

class ApplicationDemo : public glb::app::ApplicationBase {
public:
    ApplicationDemo() {}
    virtual~ApplicationDemo() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationDemo;
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
        render::Render::SetPerspective(glb::render::Render::PRIMARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 10000.0f);

        // HDR
        glb::render::Render::SetHighLightBase(1.5f);

        int32_t level = glb::scene::Scene::AddObject("res/level0.obj", "res/level0.mat");
        glb::scene::Scene::GetObjectById(level)->SetCullFaceEnable(true);
        glb::scene::Scene::GetObjectById(level)->SetCullFaceMode(glb::render::CULL_BACK);
        glb::scene::Scene::GetObjectById(level)->SetDepthTestEnable(true);

        glb::math::Matrix mat;
        mat.MakeScaleMatrix(100.0f, 100.0f, 100.0f);
        mat.Translate(0.0f, -80.0f, 0.0f);
        glb::scene::Scene::GetObjectById(level)->SetWorldMatrix(mat);
        //glb::scene::Scene::GetObjectById(level)->SetPos(math::Vector(0.0f, -80.0f, 0.0f));
        //glb::scene::Scene::GetObjectById(level)->SetScale(math::Vector(100.0f, 100.0f, 100.0f));

#if ENABLE_INSTANCE_RENDERING
        {  // Using Geometry Instance Rendering
            int32_t cube = glb::scene::Scene::AddInstanceRenderObject("res/cube.obj", "res/cube.mat", kInstanceNum * kInstanceNum);
            glb::scene::Scene::GetObjectById(cube)->SetCullFaceEnable(true);
            glb::scene::Scene::GetObjectById(cube)->SetCullFaceMode(glb::render::CULL_BACK);
            glb::scene::Scene::GetObjectById(cube)->SetDepthTestEnable(true);

            for (int32_t i = 0; i < kInstanceNum; i++) {
                for (int32_t j = 0; j < kInstanceNum; j++) {
                    int32_t instance = glb::scene::Scene::AddInstanceObject(cube);
                    glb::scene::Scene::GetObjectById(instance)->SetWorldMatrix(math::Matrix::CreateTranslateMatrix((i - (kInstanceNum - 1)/2) * -180.0f, 0.0f, (j - (kInstanceNum - 1)/2) * -180.0f));
                    m_Instances.push_back(instance);
                }
            }
        }
#else
        {  // Using Normal Model Rendering
            for (int32_t i = 0; i < kInstanceNum; i++) {
                for (int32_t j = 0; j < kInstanceNum; j++) {
                    int32_t cube = glb::scene::Scene::AddObject("res/cube.obj");
                    glb::scene::Scene::GetObjectById(cube)->SetCullFaceEnable(true);
                    glb::scene::Scene::GetObjectById(cube)->SetCullFaceMode(glb::render::CULL_BACK);
                    glb::scene::Scene::GetObjectById(cube)->SetDepthTestEnable(true);
                    glb::scene::Scene::GetObjectById(instance)->SetWorldMatrix(math::Matrix::CreateTranslateMatrix((i - (kInstanceNum - 1)/2) * -180.0f, 0.0f, (j - (kInstanceNum - 1)/2) * -180.0f));
                    m_Instances.push_back(cube);
                }
            }
        }
#endif

        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        glb::Input::Update();

        // Update scene
        static float rotate = 0.0f;
        rotate = rotate + 1.0f;
        for (int32_t i = 0; i < m_Instances.size(); i++) {
            math::Vector pos = glb::scene::Scene::GetObjectById(m_Instances[i])->GetWorldMatrix().GetTranslation();
            math::Vector scale = glb::scene::Scene::GetObjectById(m_Instances[i])->GetWorldMatrix().GetScale();
            math::Matrix mat;
            mat.MakeScaleMatrix(scale.x, scale.y, scale.z);
            mat.RotateXYZ(rotate + i * 10.0f, rotate + i * 10.0f, rotate + i * 10.0f);
            mat.Translate(pos.x, pos.y, pos.z);
            glb::scene::Scene::GetObjectById(m_Instances[i])->SetWorldMatrix(mat);
        }

        math::Vector pos = glb::scene::Scene::GetCurCamera()->GetPos();
        math::Vector target = glb::scene::Scene::GetCurCamera()->GetTarget();
        math::Vector look = pos - target;
        math::Matrix rotCamera;
        rotCamera.MakeRotateYMatrix(Input::GetMouseMoveX() * 0.1f);
        look = rotCamera * look;
        pos = target + look * (1.0f + Input::GetMouseMoveY() * 0.005f);
        glb::scene::Scene::GetCurCamera()->SetPos(pos);
        glb::scene::Scene::GetCurCamera()->Update(1.0f);

        // Update scene
        scene::Light lit = scene::Scene::GetLight(1);
        float rotY = 1.0f;
        math::Matrix rot;
        rot.MakeRotateYMatrix(rotY);
        lit.dir = rot * lit.dir;
        glb::scene::Scene::SetLight(lit, 1);
        glb::scene::Scene::Update();

        // Draw scene
        glb::render::Render::Draw();

        time.EndProfile();
        printf("%f\n", time.GetProfileTimeInMs());
    }

    void Destroy() {
    }

private:
    std::vector<int32_t> m_Instances;
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_instance", sizeof(L"glb_instance"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.platform_api = glb::app::PA_OPENGL;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.decalMapWidth = 1024;
    config.decalMapHeight = 1024;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationDemo::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    FreeConsole();

    return 0;
}