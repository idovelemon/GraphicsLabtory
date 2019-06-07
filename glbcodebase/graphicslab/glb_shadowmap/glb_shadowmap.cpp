//---------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 07 / 23
// Brief: Demonstrate the shadow mapping
// Update[2019-06-07]: Add different shadow mapping technique
//---------------------------------------------------------------------
#include "glb_shadowmap.h"
#include "resource.h"

#define STANDARD_SHADOW_MAP (1)
#define PCF_SHADOW_MAP (2)
#define VARIANCE_SHADOW_MAP (3)
#define SHADOW_MAP VARIANCE_SHADOW_MAP

class ApplicationShadowMap : public glb::app::ApplicationBase {
private:
    scene::Object*              m_FloorObj;
    scene::Object*              m_GrassObj;

    render::shader::Program*    m_FloorProgram;
    render::shader::Program*    m_GrassProgram;

    render::shader::Program*    m_ShadowProgram;
    int32_t                     m_ShadowMapID;
    render::RenderTarget*       m_ShadowMapRT;
    math::Matrix                m_ShadowMatrix;

#if SHADOW_MAP == 3
    render::shader::UserProgram*    m_BlurHShader;
    render::shader::UserProgram*    m_BlurVShader;
    int32_t                     m_BlurHTexLoc;
    int32_t                     m_BlurVTexLoc;
    int32_t                     m_BlurWidthLoc;
    int32_t                     m_BlurHeightLoc;
    int32_t                     m_BlurShadowMapID;
    render::RenderTarget*       m_BlurVRT;
    render::RenderTarget*       m_BlurHRT;
    render::mesh::ScreenMesh*   m_ScreenMesh;
#endif


public:
    ApplicationShadowMap() {}
    virtual~ApplicationShadowMap() {}

public:
    static glb::app::ApplicationBase* Create() {
        return new ApplicationShadowMap;
    }

public:
    bool Initialize() {
        // Camera
        scene::FreeCamera* cam = scene::FreeCamera::Create(math::Vector(0.0f, 0.5f, 0.5f), math::Vector(0.0f, 0.0f, 0.0));
        glb::scene::Scene::SetCamera(glb::scene::PRIMIAY_CAM, cam);

        // Light
        scene::Light light;
        light.type = scene::AMBIENT_LIGHT;
        light.color = math::Vector(0.4f, 0.4f, 0.4f);
        scene::Scene::SetLight(light, 0);

        light.type = scene::PARALLEL_LIGHT;
        light.color = math::Vector(2.0f, 2.0f, 2.0f);
        light.dir = math::Vector(-1.0f, -1.0f, 1.0f);
        light.dir.Normalize();
        scene::Scene::SetLight(light, 1);

        // Perspective
        glb::render::Render::SetPerspective(glb::render::Render::PRIMARY_PERS, 69.0f, 800 * 1.0f / 600, 0.1f, 500.0f);

        // VSync
        render::Device::SetupVSync(false);

        // Shadow map
        {
            int32_t shadowMapWidth = app::Application::GetShadowMapWidth();
            int32_t shadowMapHeight = app::Application::GetShadowMapHeight();

            // Create shadow render target
            m_ShadowMapRT = render::RenderTarget::Create(shadowMapWidth, shadowMapHeight);
            GLB_SAFE_ASSERT(m_ShadowMapRT != nullptr);

#if SHADOW_MAP != 3
            // Create shadow map
            render::texture::Texture* shadowMap = render::texture::Texture::CreateFloat32DepthTexture(shadowMapWidth, shadowMapHeight, false);
            if (shadowMap != nullptr) {
                m_ShadowMapID = render::texture::Mgr::AddTexture(shadowMap);
            } else {
                GLB_SAFE_ASSERT(false);
            }

            if (m_ShadowMapRT != nullptr) {
                m_ShadowMapRT->AttachDepthTexture(shadowMap);
            }
#else
            // Create variance shadow map
            render::texture::Texture* varianceShadowMap = render::texture::Texture::CreateFloat32Texture(shadowMapWidth, shadowMapHeight, true);
            if (varianceShadowMap != nullptr) {
                m_ShadowMapID = render::texture::Mgr::AddTexture(varianceShadowMap);
            } else {
                GLB_SAFE_ASSERT(false);
            }

            if (m_ShadowMapRT != nullptr) {
                m_ShadowMapRT->AttachColorTexture(render::COLORBUF_COLOR_ATTACHMENT0, varianceShadowMap);
            }

            // Create blur shadow map
            render::texture::Texture* blurShadowMap = render::texture::Texture::CreateFloat32Texture(shadowMapWidth, shadowMapHeight, false);
            if (blurShadowMap != nullptr) {
                m_BlurShadowMapID = render::texture::Mgr::AddTexture(blurShadowMap);
            } else {
                GLB_SAFE_ASSERT(false);
            }

            m_BlurVRT = render::RenderTarget::Create(shadowMapWidth, shadowMapHeight);
            GLB_SAFE_ASSERT(m_BlurVRT != nullptr);

            if (m_BlurVRT != nullptr) {
                m_BlurVRT->AttachColorTexture(render::COLORBUF_COLOR_ATTACHMENT0, blurShadowMap);
            }

            m_BlurHRT = render::RenderTarget::Create(shadowMapWidth, shadowMapHeight);
            GLB_SAFE_ASSERT(m_BlurHRT != nullptr);

            if (m_BlurHRT != nullptr) {
                m_BlurHRT->AttachColorTexture(render::COLORBUF_COLOR_ATTACHMENT0, varianceShadowMap);
            }

            // Create blur program
            m_BlurHShader = render::shader::UserProgram::Create("res/bloom.vs", "res/bloomh.fs");
            m_BlurVShader = render::shader::UserProgram::Create("res/bloom.vs", "res/bloomv.fs");
            m_BlurHTexLoc = m_BlurHShader->GetUniformLocation("glb_unif_BloomTex");
            m_BlurVTexLoc = m_BlurVShader->GetUniformLocation("glb_unif_BloomTex");
            m_BlurWidthLoc = m_BlurHShader->GetUniformLocation("glb_unif_BloomTexWidth");
            m_BlurHeightLoc = m_BlurVShader->GetUniformLocation("glb_unif_BloomTexHeight");

            // Create screen mesh
            m_ScreenMesh = render::mesh::ScreenMesh::Create();
#endif
        }

        // Object
        {
#if SHADOW_MAP == 1
            m_FloorObj = scene::Object::Create("res/floor.obj", "res/floor.mat");
#elif SHADOW_MAP == 2
            m_FloorObj = scene::Object::Create("res/floor.obj", "res/floor_pcf.mat");
#elif SHADOW_MAP == 3
            m_FloorObj = scene::Object::Create("res/floor.obj", "res/floor_vsm.mat");
#endif
            m_FloorObj->SetAlphaBlendEnable(false);
            m_FloorObj->SetCullFaceEnable(true);
            m_FloorObj->SetCullFaceMode(render::CULL_BACK);
            m_FloorObj->SetDepthTestEnable(true);
            m_FloorObj->SetWorldMatrix(math::Matrix::CreateIdentityMatrix());

#if SHADOW_MAP != 3
            m_GrassObj = scene::Object::Create("res/sceneGrass.obj", "res/sceneGrass.mat");
#else
            m_GrassObj = scene::Object::Create("res/sceneGrass.obj", "res/sceneGrassVSM.mat");
#endif
            m_GrassObj->SetAlphaBlendEnable(false);
            m_GrassObj->SetCullFaceEnable(true);
            m_GrassObj->SetCullFaceMode(render::CULL_BACK);
            m_GrassObj->SetDepthTestEnable(true);
            m_GrassObj->SetWorldMatrix(math::Matrix::CreateIdentityMatrix());
        }

        // Shader
        {
            m_FloorProgram = render::shader::Mgr::GetShader(m_FloorObj->GetMaterial()->GetPassMaterial(render::kLightLoopPassName)->GetShaderID());
            m_GrassProgram = render::shader::Mgr::GetShader(m_GrassObj->GetMaterial()->GetPassMaterial(render::kLightLoopPassName)->GetShaderID());
            m_ShadowProgram = render::shader::Mgr::GetShader(m_GrassObj->GetMaterial()->GetPassMaterial(render::kShadowPassName)->GetShaderID());
        }

        return true;
    }

    void Update(float dt) {
        util::ProfileTime time;
        time.BeginProfile();

        Input::Update();

        // Update camera
        scene::CameraBase* cam = scene::Scene::GetCurCamera();
        math::Vector pos = cam->GetPos();
        math::Vector tgt = cam->GetTarget();
        math::Vector dir = tgt - pos;
        dir.Normalize();
        pos = pos + dir * Input::GetMouseMoveZ() * 0.001f;
        cam->SetPos(pos);

        // Update scene
        scene::Light lit = glb::scene::Scene::GetLight(0);
        float rotY = 1.0f;
        math::Matrix rot;
        rot.MakeRotateYMatrix(rotY);
        lit.dir = rot * lit.dir;
        glb::scene::Scene::SetLight(lit, 0);
        glb::scene::Scene::Update();

        // Draw
        BuildShadowMatrix();
        SetupShadowMaterial();
        DrawShadowMap();

#if SHADOW_MAP == 3
        BlurShadowMap();
        if (render::texture::Mgr::GetTextureById(m_ShadowMapID)) {
            render::texture::Mgr::GetTextureById(m_ShadowMapID)->GenerateMipmap();
        }
#endif

        DrawScene();
        render::Device::SwapBuffer();

        // FPS
        time.EndProfile();
        printf("%f\n", time.GetProfileTimeInMs());

        static int32_t sDisplayFPSDelta = 100;
        if (sDisplayFPSDelta >= 100) {
            std::string titleString = "glb_shadowmap";
            titleString = titleString + " (";
            char buffer[128];
            sprintf_s(buffer, "FPS:%d", static_cast<int32_t>(1000.0f / time.GetProfileTimeInMs()));
            titleString = titleString + buffer;
            titleString = titleString + " )";
            ::SetWindowTextA(app::Application::GetWindowHandle(), titleString.c_str());
            sDisplayFPSDelta = 0;
        }
        sDisplayFPSDelta++;
    }

    void Destroy() {
    }

    void BuildShadowMatrix() {
        math::Vector light_dir = -scene::Scene::GetLight(1).dir;

        // Build light space
        math::Vector lit_space_right = math::Cross(light_dir, math::Vector(0.0f, 1.0f, 0.0f));
        lit_space_right.Normalize();
        math::Vector lit_space_up = math::Cross(light_dir, lit_space_right);
        lit_space_up.Normalize();
        math::Matrix light_space_to_world_space;
        light_space_to_world_space.MakeRotateMatrix(lit_space_right, lit_space_up, light_dir);

        // Size
        float width = 1.0f;
        float height = 1.0f;
        float depth = 5.0f;

        // Camera position in world space
        math::Vector pos = math::Vector(0.0f, 0.0f, 0.0f);
        pos = light_space_to_world_space * pos;

        // Target position in world space
        math::Vector look_at = math::Vector(0.0f, 0.0f, -1.0f);
        look_at.w = 0.0f;
        look_at = light_space_to_world_space * look_at;
        math::Vector target = pos + look_at;

        // Orthogonal projection matrix
        math::Matrix proj;
        proj.MakeOrthogonalMatrix(-width / 2.0f, width / 2.0f, - height / 2.0f, height / 2.0f, - depth / 2.0f, depth / 2.0f);

        // View matrix
        math::Matrix view;
        view.MakeViewMatrix(pos, target);

        // Shadow matrix
        m_ShadowMatrix.MakeIdentityMatrix();
        m_ShadowMatrix.Mul(proj);
        m_ShadowMatrix.Mul(view);
    }

    void SetupShadowMaterial() {
        // Material Parameters
        render::material::PassMaterial* material = m_GrassObj->GetMaterial()->GetPassMaterial(render::kShadowPassName);
        int32_t texUnit = 0;
        for (auto& param : material->GetAllParameters()) {
            if (param.type == render::material::PassMaterial::PARAMETER_TYPE_INTERNAL) {
                if (!strcmp(param.name, "glb_unif_EyePos")) {
                    param.vecValue = render::uniform::uniform_eye_pos_picker(nullptr).GetVector();
                } else if (!strcmp(param.name, "glb_unif_LookAt")) {
                    param.vecValue = render::uniform::uniform_look_at_picker(nullptr).GetVector();
                } else if (!strcmp(param.name, "glb_unif_GlobalLight_Ambient")) {
                    param.vecValue = render::uniform::uniform_global_light_ambient_picker(nullptr).GetVector();
                } else if (!strcmp(param.name, "glb_unif_ParallelLight_Dir")) {
                    param.vecValue = render::uniform::uniform_parallel_light_dir_picker(nullptr).GetVector();
                } else if (!strcmp(param.name, "glb_unif_ParallelLight")) {
                    param.vecValue = render::uniform::uniform_parallel_light_picker(nullptr).GetVector();
                } else if (!strcmp(param.name, "glb_unif_ProjM")) {
                    param.matValue = render::uniform::uniform_projm_picker(nullptr).GetMatrix();
                } else if (!strcmp(param.name, "glb_unif_ViewM")) {
                    param.matValue = render::uniform::uniform_viewm_picker(nullptr).GetMatrix();
                } else if (!strcmp(param.name, "glb_unif_WorldM")) {
                    param.matValue = render::uniform::uniform_worldm_picker(m_GrassObj).GetMatrix();
                } else if (!strcmp(param.name, "glb_unif_Trans_Inv_WorldM")) {
                    param.matValue = render::uniform::uniform_trans_inv_worldm_picker(m_GrassObj).GetMatrix();
                } else if (!strcmp(param.name, "glb_unif_Timer")) {
                    param.floatValue = param.floatValue + 0.001f;
                } else {
                    GLB_SAFE_ASSERT(false);
                }
            } else {
                if (!strcmp(param.name, "glb_unif_ShadowM")) {
                    param.matValue = m_ShadowMatrix;
                }
            }
        }
    }

#if SHADOW_MAP == 3
    void BlurShadowMap() {
        BlurShadowMapV();
        BlurShadowMapH();
    }

    void BlurShadowMapV() {
        // Render Target
        render::Device::SetRenderTarget(m_BlurVRT);

        // Viewport
        render::Device::SetViewport(0, 0, m_BlurVRT->GetWidth(), m_BlurVRT->GetHeight());

        // Draw Buffer
        render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

        // Clear
        render::Device::SetClearDepth(1.0f);
        render::Device::SetClearColor(1.0f, 1.0f, 1.0f);
        render::Device::Clear(render::CLEAR_COLOR | render::CLEAR_DEPTH);

        // Shader
        render::Device::SetShader(m_BlurVShader);
        render::Device::SetShaderLayout(m_BlurVShader->GetShaderLayout());

        // Vertex
        render::Device::SetVertexBuffer(m_ScreenMesh->GetVertexBuffer());
        render::Device::SetVertexLayout(m_ScreenMesh->GetVertexLayout());

        // Texture
        render::Device::ClearTexture();
        render::Device::SetTexture(0, render::texture::Mgr::GetTextureById(m_ShadowMapID), 0);

        // Uniform
        render::Device::SetUniformSampler2D(m_BlurVTexLoc, 0);
        render::Device::SetUniform1f(m_BlurHeightLoc, static_cast<float>(m_BlurVRT->GetHeight()));

        // Draw
        render::Device::Draw(render::PT_TRIANGLES, 0, m_ScreenMesh->GetVertexNum());

        // Reset
        render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());
        render::Device::SetViewport(0, 0, app::Application::GetWindowWidth(), app::Application::GetWindowHeight());
    }

    void BlurShadowMapH() {
        // Render Target
        render::Device::SetRenderTarget(m_BlurHRT);

        // Viewport
        render::Device::SetViewport(0, 0, m_BlurHRT->GetWidth(), m_BlurHRT->GetHeight());

        // Draw Buffer
        render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

        // Clear
        render::Device::SetClearDepth(1.0f);
        render::Device::SetClearColor(1.0f, 1.0f, 1.0f);
        render::Device::Clear(render::CLEAR_COLOR | render::CLEAR_DEPTH);

        // Shader
        render::Device::SetShader(m_BlurHShader);
        render::Device::SetShaderLayout(m_BlurHShader->GetShaderLayout());

        // Vertex
        render::Device::SetVertexBuffer(m_ScreenMesh->GetVertexBuffer());
        render::Device::SetVertexLayout(m_ScreenMesh->GetVertexLayout());

        // Texture
        render::Device::ClearTexture();
        render::Device::SetTexture(0, render::texture::Mgr::GetTextureById(m_BlurShadowMapID), 0);

        // Uniform
        render::Device::SetUniformSampler2D(m_BlurHTexLoc, 0);
        render::Device::SetUniform1f(m_BlurWidthLoc, static_cast<float>(m_BlurHRT->GetWidth()));

        // Draw
        render::Device::Draw(render::PT_TRIANGLES, 0, m_ScreenMesh->GetVertexNum());

        // Reset
        render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());
        render::Device::SetViewport(0, 0, app::Application::GetWindowWidth(), app::Application::GetWindowHeight());
    }
#endif

    void DrawShadowMap() {
        // Render Target
        render::Device::SetRenderTarget(m_ShadowMapRT);

        // Viewport
        render::Device::SetViewport(0, 0, app::Application::GetShadowMapWidth(), app::Application::GetShadowMapHeight());

        // Draw Buffer
#if SHADOW_MAP != 3
        render::Device::SetDrawColorBuffer(render::COLORBUF_NONE);
#else
        render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);
#endif

        // Clear
#if SHADOW_MAP != 3
        render::Device::SetClearDepth(1.0f);
        render::Device::Clear(render::CLEAR_DEPTH);
#else
        render::Device::SetClearDepth(1.0f);
        render::Device::SetClearColor(1.0f, 1.0f, 1.0f);
        render::Device::Clear(render::CLEAR_DEPTH | render::CLEAR_COLOR);
#endif

        // Shader
        render::Device::SetShader(m_ShadowProgram);
        render::Device::SetShaderLayout(m_ShadowProgram->GetShaderLayout());

        // Material Parameters
        render::material::PassMaterial* material = m_GrassObj->GetMaterial()->GetPassMaterial(render::kShadowPassName);
        int32_t texUnit = 0;
        for (auto& param : material->GetAllParameters()) {
            if (param.format == render::PARAMETER_FORMAT_FLOAT) {
                render::Device::SetUniform1f(param.name, param.floatValue);
            } else if (param.format == render::PARAMETER_FORMAT_FLOAT3) {
                render::Device::SetUniform3f(param.name, param.vecValue);
            } else if (param.format == render::PARAMETER_FORMAT_FLOAT4) {
                render::Device::SetUniform4f(param.name, param.vecValue);
            } else if (param.format == render::PARAMETER_FORMAT_INT) {
                render::Device::SetUniform1i(param.name, param.intValue);
            } else if (param.format == render::PARAMETER_FORMAT_MATRIX) {
                render::Device::SetUniformMatrix(param.name, param.matValue);
            } else if (param.format == render::PARAMETER_FORMAT_TEXTURE_2D) {
                render::Device::SetUniformSampler2D(param.name, render::texture::Mgr::GetTextureById(param.intValue), texUnit++);
            } else if (param.format == render::PARAMETER_FORMAT_TEXTURE_3D) {
                render::Device::SetUniformSampler3D(param.name, render::texture::Mgr::GetTextureById(param.intValue), texUnit++);
            } else if (param.format == render::PARAMETER_FORMAT_TEXTURE_CUBE) {
                render::Device::SetUniformSamplerCube(param.name, render::texture::Mgr::GetTextureById(param.intValue), texUnit++);
            }
        }

        // Vertex Buffer
        render::VertexLayout layout = m_GrassObj->GetMesh()->GetVertexLayout();
        int32_t num = m_GrassObj->GetMesh()->GetVertexNum();
        render::Device::SetVertexBuffer(m_GrassObj->GetMesh()->GetVertexBuffer());
        render::Device::SetVertexLayout(layout);

        if (m_GrassObj->IsCullFaceEnable()) {
            render::Device::SetCullFaceEnable(true);
            render::Device::SetCullFaceMode(m_GrassObj->GetCullFaceMode());
        } else {
            render::Device::SetCullFaceEnable(false);
        }

        if (m_GrassObj->IsDepthTestEnable()) {
            render::Device::SetDepthTestEnable(true);
        } else {
            render::Device::SetDepthTestEnable(false);
        }

        if (m_GrassObj->IsAlphaBlendEnable()) {
            render::Device::SetAlphaBlendEnable(true);
            render::Device::SetAlphaBlendFunc(render::FACTOR_SRC, m_GrassObj->GetAlphaBlendFunc(render::FACTOR_SRC));
            render::Device::SetAlphaBlendFunc(render::FACTOR_DST, m_GrassObj->GetAlphaBlendFunc(render::FACTOR_DST));
        } else {
            render::Device::SetAlphaBlendEnable(false);
        }

        // Draw
        if (m_GrassObj->GetObjectType() == scene::Object::OBJECT_TYPE_NORMAL) {
            render::Device::Draw(render::PT_TRIANGLES, 0, num);
        } else if (m_GrassObj->GetObjectType() == scene::Object::OBJECT_TYPE_INSTANCE_RENDER) {
            int32_t instanceNum = reinterpret_cast<scene::InstanceRenderObject*>(m_GrassObj)->GetCurInstanceNum();
            render::Device::DrawInstance(render::PT_TRIANGLES, 0, num, instanceNum);
        }

        // Reset
        render::Device::SetViewport(0, 0, static_cast<int32_t>(800), static_cast<int32_t>(600));
        render::Device::SetRenderTarget(nullptr);
    }

    void DrawScene() {
        // Render Target
        render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());

        // Viewport
        render::Device::SetViewport(0, 0, app::Application::GetWindowWidth(), app::Application::GetWindowHeight());

        // Clear
        render::Device::SetClearDepth(1.0f);
        render::Device::SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        render::Device::Clear(render::CLEAR_DEPTH | render::CLEAR_COLOR);

        SetupFloorMaterial();
        DrawFloor();

        SetupGrassMaterial();
        DrawGrass();

        // Reset
        render::Device::SetViewport(0, 0, static_cast<int32_t>(800), static_cast<int32_t>(600));
        render::Device::SetRenderTarget(nullptr);
    }

    void SetupFloorMaterial() {
        // Material Parameters
        render::material::PassMaterial* material = m_FloorObj->GetMaterial()->GetPassMaterial(render::kLightLoopPassName);
        int32_t texUnit = 0;
        for (auto& param : material->GetAllParameters()) {
            if (param.type == render::material::PassMaterial::PARAMETER_TYPE_INTERNAL) {
                if (!strcmp(param.name, "glb_unif_EyePos")) {
                    param.vecValue = render::uniform::uniform_eye_pos_picker(nullptr).GetVector();
                } else if (!strcmp(param.name, "glb_unif_LookAt")) {
                    param.vecValue = render::uniform::uniform_look_at_picker(nullptr).GetVector();
                } else if (!strcmp(param.name, "glb_unif_GlobalLight_Ambient")) {
                    param.vecValue = render::uniform::uniform_global_light_ambient_picker(nullptr).GetVector();
                } else if (!strcmp(param.name, "glb_unif_ParallelLight_Dir")) {
                    param.vecValue = render::uniform::uniform_parallel_light_dir_picker(nullptr).GetVector();
                } else if (!strcmp(param.name, "glb_unif_ParallelLight")) {
                    param.vecValue = render::uniform::uniform_parallel_light_picker(nullptr).GetVector();
                } else if (!strcmp(param.name, "glb_unif_ProjM")) {
                    param.matValue = render::uniform::uniform_projm_picker(nullptr).GetMatrix();
                } else if (!strcmp(param.name, "glb_unif_ViewM")) {
                    param.matValue = render::uniform::uniform_viewm_picker(nullptr).GetMatrix();
                } else if (!strcmp(param.name, "glb_unif_WorldM")) {
                    param.matValue = render::uniform::uniform_worldm_picker(m_FloorObj).GetMatrix();
                } else if (!strcmp(param.name, "glb_unif_Trans_Inv_WorldM")) {
                    param.matValue = render::uniform::uniform_trans_inv_worldm_picker(m_FloorObj).GetMatrix();
                } else {
                    GLB_SAFE_ASSERT(false);
                }
            } else {
                if (!strcmp(param.name, "glb_unif_ShadowMap")) {
                    param.intValue = m_ShadowMapID;
                } else if (!strcmp(param.name, "glb_unif_ShadowM")) {
                    param.matValue = m_ShadowMatrix;
                }
            }
        }
    }

    void DrawFloor() {
        // Shader
        render::Device::SetShader(m_FloorProgram);
        render::Device::SetShaderLayout(m_FloorProgram->GetShaderLayout());

        // Material Parameters
        render::material::PassMaterial* material = m_FloorObj->GetMaterial()->GetPassMaterial(render::kLightLoopPassName);
        int32_t texUnit = 0;
        for (auto& param : material->GetAllParameters()) {
            if (param.format == render::PARAMETER_FORMAT_FLOAT) {
                render::Device::SetUniform1f(param.name, param.floatValue);
            } else if (param.format == render::PARAMETER_FORMAT_FLOAT3) {
                render::Device::SetUniform3f(param.name, param.vecValue);
            } else if (param.format == render::PARAMETER_FORMAT_FLOAT4) {
                render::Device::SetUniform4f(param.name, param.vecValue);
            } else if (param.format == render::PARAMETER_FORMAT_INT) {
                render::Device::SetUniform1i(param.name, param.intValue);
            } else if (param.format == render::PARAMETER_FORMAT_MATRIX) {
                render::Device::SetUniformMatrix(param.name, param.matValue);
            } else if (param.format == render::PARAMETER_FORMAT_TEXTURE_2D) {
                render::Device::SetUniformSampler2D(param.name, render::texture::Mgr::GetTextureById(param.intValue), texUnit++);
            } else if (param.format == render::PARAMETER_FORMAT_TEXTURE_3D) {
                render::Device::SetUniformSampler3D(param.name, render::texture::Mgr::GetTextureById(param.intValue), texUnit++);
            } else if (param.format == render::PARAMETER_FORMAT_TEXTURE_CUBE) {
                render::Device::SetUniformSamplerCube(param.name, render::texture::Mgr::GetTextureById(param.intValue), texUnit++);
            }
        }

        // Vertex Buffer
        render::VertexLayout layout = m_FloorObj->GetMesh()->GetVertexLayout();
        int32_t num = m_FloorObj->GetMesh()->GetVertexNum();
        render::Device::SetVertexBuffer(m_FloorObj->GetMesh()->GetVertexBuffer());
        render::Device::SetVertexLayout(layout);

        if (m_FloorObj->IsCullFaceEnable()) {
            render::Device::SetCullFaceEnable(true);
            render::Device::SetCullFaceMode(m_FloorObj->GetCullFaceMode());
        } else {
            render::Device::SetCullFaceEnable(false);
        }

        if (m_FloorObj->IsDepthTestEnable()) {
            render::Device::SetDepthTestEnable(true);
        } else {
            render::Device::SetDepthTestEnable(false);
        }

        if (m_FloorObj->IsAlphaBlendEnable()) {
            render::Device::SetAlphaBlendEnable(true);
            render::Device::SetAlphaBlendFunc(render::FACTOR_SRC, m_FloorObj->GetAlphaBlendFunc(render::FACTOR_SRC));
            render::Device::SetAlphaBlendFunc(render::FACTOR_DST, m_FloorObj->GetAlphaBlendFunc(render::FACTOR_DST));
        } else {
            render::Device::SetAlphaBlendEnable(false);
        }

        // Draw
        if (m_FloorObj->GetObjectType() == scene::Object::OBJECT_TYPE_NORMAL) {
            render::Device::Draw(render::PT_TRIANGLES, 0, num);
        } else if (m_FloorObj->GetObjectType() == scene::Object::OBJECT_TYPE_INSTANCE_RENDER) {
            int32_t instanceNum = reinterpret_cast<scene::InstanceRenderObject*>(m_FloorObj)->GetCurInstanceNum();
            render::Device::DrawInstance(render::PT_TRIANGLES, 0, num, instanceNum);
        }
    }

    void SetupGrassMaterial() {
        // Material Parameters
        render::material::PassMaterial* material = m_GrassObj->GetMaterial()->GetPassMaterial(render::kLightLoopPassName);
        int32_t texUnit = 0;
        for (auto& param : material->GetAllParameters()) {
            if (param.type == render::material::PassMaterial::PARAMETER_TYPE_INTERNAL) {
                if (!strcmp(param.name, "glb_unif_EyePos")) {
                    param.vecValue = render::uniform::uniform_eye_pos_picker(nullptr).GetVector();
                } else if (!strcmp(param.name, "glb_unif_LookAt")) {
                    param.vecValue = render::uniform::uniform_look_at_picker(nullptr).GetVector();
                } else if (!strcmp(param.name, "glb_unif_GlobalLight_Ambient")) {
                    param.vecValue = render::uniform::uniform_global_light_ambient_picker(nullptr).GetVector();
                } else if (!strcmp(param.name, "glb_unif_ParallelLight_Dir")) {
                    param.vecValue = render::uniform::uniform_parallel_light_dir_picker(nullptr).GetVector();
                } else if (!strcmp(param.name, "glb_unif_ParallelLight")) {
                    param.vecValue = render::uniform::uniform_parallel_light_picker(nullptr).GetVector();
                } else if (!strcmp(param.name, "glb_unif_ProjM")) {
                    param.matValue = render::uniform::uniform_projm_picker(nullptr).GetMatrix();
                } else if (!strcmp(param.name, "glb_unif_ViewM")) {
                    param.matValue = render::uniform::uniform_viewm_picker(nullptr).GetMatrix();
                } else if (!strcmp(param.name, "glb_unif_WorldM")) {
                    param.matValue = render::uniform::uniform_worldm_picker(m_FloorObj).GetMatrix();
                } else if (!strcmp(param.name, "glb_unif_Trans_Inv_WorldM")) {
                    param.matValue = render::uniform::uniform_trans_inv_worldm_picker(m_FloorObj).GetMatrix();
                } else if (!strcmp(param.name, "glb_unif_Timer")) {
                    param.floatValue = param.floatValue + 0.001f;
                } else {
                    GLB_SAFE_ASSERT(false);
                }
            }
        }
    }

    void DrawGrass() {
        // Shader
        render::Device::SetShader(m_GrassProgram);
        render::Device::SetShaderLayout(m_GrassProgram->GetShaderLayout());

        // Material Parameters
        render::material::PassMaterial* material = m_GrassObj->GetMaterial()->GetPassMaterial(render::kLightLoopPassName);
        int32_t texUnit = 0;
        for (auto& param : material->GetAllParameters()) {
            if (param.format == render::PARAMETER_FORMAT_FLOAT) {
                render::Device::SetUniform1f(param.name, param.floatValue);
            } else if (param.format == render::PARAMETER_FORMAT_FLOAT3) {
                render::Device::SetUniform3f(param.name, param.vecValue);
            } else if (param.format == render::PARAMETER_FORMAT_FLOAT4) {
                render::Device::SetUniform4f(param.name, param.vecValue);
            } else if (param.format == render::PARAMETER_FORMAT_INT) {
                render::Device::SetUniform1i(param.name, param.intValue);
            } else if (param.format == render::PARAMETER_FORMAT_MATRIX) {
                render::Device::SetUniformMatrix(param.name, param.matValue);
            } else if (param.format == render::PARAMETER_FORMAT_TEXTURE_2D) {
                render::Device::SetUniformSampler2D(param.name, render::texture::Mgr::GetTextureById(param.intValue), texUnit++);
            } else if (param.format == render::PARAMETER_FORMAT_TEXTURE_3D) {
                render::Device::SetUniformSampler3D(param.name, render::texture::Mgr::GetTextureById(param.intValue), texUnit++);
            } else if (param.format == render::PARAMETER_FORMAT_TEXTURE_CUBE) {
                render::Device::SetUniformSamplerCube(param.name, render::texture::Mgr::GetTextureById(param.intValue), texUnit++);
            }
        }

        // Vertex Buffer
        render::VertexLayout layout = m_GrassObj->GetMesh()->GetVertexLayout();
        int32_t num = m_GrassObj->GetMesh()->GetVertexNum();
        render::Device::SetVertexBuffer(m_GrassObj->GetMesh()->GetVertexBuffer());
        render::Device::SetVertexLayout(layout);

        if (m_GrassObj->IsCullFaceEnable()) {
            render::Device::SetCullFaceEnable(true);
            render::Device::SetCullFaceMode(m_GrassObj->GetCullFaceMode());
        } else {
            render::Device::SetCullFaceEnable(false);
        }

        if (m_GrassObj->IsDepthTestEnable()) {
            render::Device::SetDepthTestEnable(true);
        } else {
            render::Device::SetDepthTestEnable(false);
        }

        if (m_GrassObj->IsAlphaBlendEnable()) {
            render::Device::SetAlphaBlendEnable(true);
            render::Device::SetAlphaBlendFunc(render::FACTOR_SRC, m_GrassObj->GetAlphaBlendFunc(render::FACTOR_SRC));
            render::Device::SetAlphaBlendFunc(render::FACTOR_DST, m_GrassObj->GetAlphaBlendFunc(render::FACTOR_DST));
        } else {
            render::Device::SetAlphaBlendEnable(false);
        }

        // Draw
        if (m_GrassObj->GetObjectType() == scene::Object::OBJECT_TYPE_NORMAL) {
            render::Device::Draw(render::PT_TRIANGLES, 0, num);
        } else if (m_FloorObj->GetObjectType() == scene::Object::OBJECT_TYPE_INSTANCE_RENDER) {
            int32_t instanceNum = reinterpret_cast<scene::InstanceRenderObject*>(m_GrassObj)->GetCurInstanceNum();
            render::Device::DrawInstance(render::PT_TRIANGLES, 0, num, instanceNum);
        }
    }
};

int _stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR cmdLine, int nShowCmd) {
    glb::app::AppConfig config;
    memcpy(config.caption, L"glb_shadowmap", sizeof(L"glb_shadowmap"));
    config.screen_width = 800;
    config.screen_height = 600;
    config.shadow_map_width = 1024;
    config.shadow_map_height = 1024;
    config.decalMapWidth = 1024;
    config.decalMapHeight = 1024;
    config.msaaSamplerNum = 8;
    config.icon = IDI_ICON1;
    if (!glb::app::Application::Initialize(ApplicationShadowMap::Create, hInstance, config)) {
        return 0;
    }

    glb::app::Application::Update();

    glb::app::Application::Destroy();

    return 0;
}