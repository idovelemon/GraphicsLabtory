//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 10 / 28
// Brief: Draw the entire scene
//------------------------------------------------------------------
#include "glbrender.h"

#include <map>

#include "glbcamera.h"
#include "glbmacro.h"
#include "glbmesh.h"
#include "glbobject.h"
#include "glbprofile.h"
#include "glbrenderdevice.h"
#include "glbrendertarget.h"
#include "glbscene.h"
#include "glbshader.h"
#include "glbtexture.h"
#include "GL\glew.h"

namespace glb {

namespace render {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// TYPE DECLARATION
//-----------------------------------------------------------------------------------
class RenderImp;
static RenderImp* s_RenderImp = NULL;

struct PerspectiveProj {
    float fov;
    float aspect;
    float znear;
    float zfar;
    Matrix m;
};

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// ShaderGroup DECLARATION
//----------------------------------------------------------------------------------
class ShaderGroup {
public:
    ShaderGroup(shader::Descriptor desc, shader::Program* shader_program);
    virtual ~ShaderGroup();

public:
    void AddObject(Object* obj);
    shader::Descriptor GetShaderDesc();
    std::vector<Object*>& GetObjects();
    shader::Program* GetShaderProgram();

private:
    shader::Descriptor              m_ShaderDesc;
    shader::Program*                m_ShaderProgram;
    std::vector<Object*>            m_Objects;
};

//-----------------------------------------------------------------------------------
// RenderImp DECLARATION
//----------------------------------------------------------------------------------
class RenderImp {
public:
    RenderImp();
    virtual ~RenderImp();

public:
    void Initialize(float width, float height);
    void Destroy();
    void Draw();

    void SetPerspective(int32_t type, float fov, float aspect, float znear, float zfar);
    Matrix& GetPerspective(int32_t type);
    void SetCurPerspectiveType(int32_t type);
    int32_t GetCurPerspectiveType();
    Vector GetFrustumPointInView(int32_t index);
    float GetFarClip();
    float GetScreenWidth();
    float GetScreenHeight();
    int32_t GetRandomRotateTex();

    float GetHDRAverageLum();
    int32_t GetHDRSceneTex();
    float GetBloomWidth();
    float GetBloomHeight();
    void SetExposureLevel(float level);
    float GetExposureLevel();
    void SetLightAdaption(float adaption);
    float GetLightAdaption();

    void AddLine(Vector start, Vector end, Vector color);

protected:
    void PreDraw();
    void DrawShadowMap();
    void DrawAOMap();
    void DrawLightLoop();
    void DrawDebug();
    void DrawHDR();
    void AfterDraw();

    void PrepareShadowMap();
    void PrepareDepthMap();
    void PrepareAOMap();
    void PrepareHDR();

    // HDR
    void DownsamplerHDRScene();
    void CalcLogLum();
    void CalcAverageLum();
    void FilterBrightness();
    void DownsamplerBrightness();
    void BloomH();
    void BloomV();
    void BlendHDRScene();

    // SSAO
    void DrawDepthMap();
    void GenRandRotateMap();
    void DrawAO();
    void BiBlurH();
    void BiBlurV();

    void SetUniform(int32_t location, uniform::Wrapper& wrapper);
    inline float ZValueFromCamera(Object* obj);

private:
    float                                   m_Width;
    float                                   m_Height;
    std::vector<ShaderGroup>                m_ShaderGroups;

    // Perspective
    int32_t                                 m_PerspectiveType;
    PerspectiveProj                         m_Perspective[Render::MAX_PERS];

    // Shadow
    RenderTarget*                           m_ShadowRenderTarget;
    int32_t                                 m_ShadowMap;
    int32_t                                 m_ShadowShader;

    // Depth
    RenderTarget*                           m_DepthTarget;
    int32_t                                 m_DepthShader;
    int32_t                                 m_DepthMap;

    // HDR
    RenderTarget*                           m_HDRRenderTarget;
    RenderTarget*                           m_BloomRenderTarget;
    int32_t                                 m_HDRSceneTex;
    int32_t                                 m_HighLightSceneTex;
    int32_t                                 m_LogLumTex;
    int32_t                                 m_BloomTex;
    float                                   m_AverageLum;
    int32_t                                 m_LogLumShader;
    int32_t                                 m_FilterBrightnessShader;
    int32_t                                 m_BloomHShader;
    int32_t                                 m_BloomVShader;
    int32_t                                 m_TonemapShader;
    int32_t                                 m_MaxMipmapLevel;
    mesh::ScreenMesh*                       m_ScreenMesh;
    float                                   m_BloomWidth;
    float                                   m_BloomHeight;
    float                                   m_ExposureLevel;
    float                                   m_LightAdaption;
    float                                   m_PreAverageLum;

    // SSAO
    RenderTarget*                           m_AORenderTarget;
    int32_t                                 m_RandRotateMap;
    int32_t                                 m_AOMap;
    int32_t                                 m_BiBlurMap;
    int32_t                                 m_AOShader;
    int32_t                                 m_BiBlurHShader;
    int32_t                                 m_BiBlurVShader;

    mesh::DebugMesh*                        m_DebugMesh;
};

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// ShaderGroup DEFINITION
//-----------------------------------------------------------------------------------
ShaderGroup::ShaderGroup(shader::Descriptor desc, shader::Program* shader_program)
: m_ShaderDesc(desc)
, m_ShaderProgram(shader_program) {
}

ShaderGroup::~ShaderGroup() {
    m_ShaderProgram = NULL;
    m_Objects.clear();
}

void ShaderGroup::AddObject(Object* obj) {
    m_Objects.push_back(obj);
}

shader::Descriptor ShaderGroup::GetShaderDesc() {
    return m_ShaderDesc;
}

std::vector<Object*>& ShaderGroup::GetObjects() {
    return m_Objects;
}

shader::Program* ShaderGroup::GetShaderProgram() {
    return m_ShaderProgram;
}

//-----------------------------------------------------------------------------------
// RenderImp DEFINITION
//-----------------------------------------------------------------------------------
RenderImp::RenderImp()
: m_Width(0)
, m_Height(0)
, m_PerspectiveType(Render::PRIMARY_PERS)

// Shadow
, m_ShadowRenderTarget(NULL)
, m_ShadowMap(-1)
, m_ShadowShader(-1)

// Depth
, m_DepthTarget(NULL)
, m_DepthMap(-1)
, m_DepthShader(-1)

// HDR
, m_HDRRenderTarget(NULL)
, m_BloomRenderTarget(NULL)
, m_HDRSceneTex(-1)
, m_HighLightSceneTex(-1)
, m_LogLumTex(-1)
, m_BloomTex(-1)
, m_AverageLum(0.0f)
, m_LogLumShader(-1)
, m_FilterBrightnessShader(-1)
, m_BloomHShader(-1)
, m_BloomVShader(-1)
, m_TonemapShader(-1)
, m_MaxMipmapLevel(0)
, m_BloomWidth(0.0f)
, m_BloomHeight(0.0f)
, m_ExposureLevel(0.0f)
, m_LightAdaption(0.0f)
, m_PreAverageLum(0.0f)

// SSAO
, m_AORenderTarget(NULL)
, m_RandRotateMap(-1)
, m_AOMap(-1)
, m_BiBlurMap(-1)
, m_AOShader(-1)
, m_BiBlurHShader(-1)
, m_BiBlurVShader(-1)

, m_DebugMesh(NULL) {
    memset(m_Perspective, 0, sizeof(m_Perspective));
    m_ShaderGroups.clear();
}

RenderImp::~RenderImp() {
    Destroy();
}

void RenderImp::Initialize(float width, float height) {
    m_ShaderGroups.clear();

    m_Width = width;
    m_Height = height;

    PrepareShadowMap();
    PrepareDepthMap();
    PrepareAOMap();
    PrepareHDR();

    m_DebugMesh = mesh::DebugMesh::Create();
}

void RenderImp::Destroy() {
    m_ShaderGroups.clear();

    m_Width = 0;
    m_Height = 0;
    m_PerspectiveType = Render::PRIMARY_PERS;

    // Shadow
    GLB_SAFE_DELETE(m_ShadowRenderTarget);
    m_ShadowMap = -1;
    m_ShadowShader = -1;

    // Depth
    GLB_SAFE_DELETE(m_DepthTarget);
    m_DepthMap = -1;
    m_DepthShader = -1;

    // HDR
    GLB_SAFE_DELETE(m_HDRRenderTarget);
    GLB_SAFE_DELETE(m_BloomRenderTarget);
    m_HDRSceneTex = -1;
    m_HighLightSceneTex = -1;
    m_LogLumTex = -1;
    m_BloomTex = -1;
    m_AverageLum = 0.0f;
    m_LogLumShader = -1;
    m_FilterBrightnessShader = -1;
    m_BloomHShader = -1;
    m_BloomVShader = -1;
    m_TonemapShader = -1;
    m_MaxMipmapLevel = 0;
    m_BloomWidth = 0.0f;
    m_BloomHeight = 0.0f;
    m_ExposureLevel = 0.0f;
    m_LightAdaption = 0.0f;
    m_PreAverageLum = 0.0f;

    // SSAO
    GLB_SAFE_DELETE(m_AORenderTarget);
    m_RandRotateMap = -1;
    m_AOMap = -1;
    m_BiBlurMap = -1;
    m_AOShader = -1;
    m_BiBlurHShader = -1;
    m_BiBlurVShader = -1;

    GLB_SAFE_DELETE(m_DebugMesh);
}

void RenderImp::Draw() {
    PreDraw();
    DrawShadowMap();
    DrawDepthMap();
    DrawAOMap();
    DrawLightLoop();
    DrawDebug();
    DrawHDR();
    AfterDraw();
    GLB_CHECK_GL_ERROR;
}

void RenderImp::SetPerspective(int32_t type, float fov, float aspect, float znear, float zfar) {
    if (0 <= type && type < Render::MAX_PERS) {
        m_Perspective[type].fov = fov;
        m_Perspective[type].aspect = aspect;
        m_Perspective[type].znear = znear;
        m_Perspective[type].zfar = zfar;
        m_Perspective[type].m.MakeProjectionMatrix(aspect, fov, znear, zfar);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

Matrix& RenderImp::GetPerspective(int32_t type) {
    if (0 <= type && type < Render::MAX_PERS) {
        return m_Perspective[type].m;
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return m_Perspective[Render::PRIMARY_PERS].m;
}

void RenderImp::SetCurPerspectiveType(int32_t type) {
    m_PerspectiveType = type;
}

int32_t RenderImp::GetCurPerspectiveType() {
    return m_PerspectiveType;
}

Vector RenderImp::GetFrustumPointInView(int32_t index) {
    Vector v(0.0f, 0.0f, 0.0f);

    if (0 <= index && index <= Render::FRD) {
        float fov_half = m_Perspective[Render::PRIMARY_PERS].fov / 2.0f;
        float tan_fov_half = tan(fov_half / 180.0f * 3.1415926f);
        float near_height_half = m_Perspective[Render::PRIMARY_PERS].znear * tan_fov_half;
        float far_height_half = m_Perspective[Render::PRIMARY_PERS].zfar * tan_fov_half;
        float near_width_half = near_height_half * m_Perspective[Render::PRIMARY_PERS].aspect;
        float far_width_half = far_height_half * m_Perspective[Render::PRIMARY_PERS].aspect;

        // For opengl, camera look at negative z-axis
        switch (index) {
        case Render::NLU:
            v.x = -near_width_half;
            v.y = near_height_half;
            v.z = -m_Perspective[Render::PRIMARY_PERS].znear;
            break;

        case Render::NLD:
            v.x = -near_width_half;
            v.y = -near_height_half;
            v.z = -m_Perspective[Render::PRIMARY_PERS].znear;
            break;

        case Render::NRU:
            v.x = near_width_half;
            v.y = near_height_half;
            v.z = -m_Perspective[Render::PRIMARY_PERS].znear;
            break;

        case Render::NRD:
            v.x = near_width_half;
            v.y = -near_height_half;
            v.z = -m_Perspective[Render::PRIMARY_PERS].znear;
            break;

        case Render::FLU:
            v.x = -far_width_half;
            v.y = far_height_half;
            v.z = -m_Perspective[Render::PRIMARY_PERS].zfar;
            break;

        case Render::FLD:
            v.x = -far_width_half;
            v.y = -far_height_half;
            v.z = -m_Perspective[Render::PRIMARY_PERS].zfar;
            break;

        case Render::FRU:
            v.x = far_width_half;
            v.y = far_height_half;
            v.z = -m_Perspective[Render::PRIMARY_PERS].zfar;
            break;

        case Render::FRD:
            v.x = far_width_half;
            v.y = -far_height_half;
            v.z = -m_Perspective[Render::PRIMARY_PERS].zfar;
            break;

        default:
            GLB_SAFE_ASSERT(false);
        }
    }

    return v;
}

float RenderImp::GetFarClip() {
    return m_Perspective[Render::PRIMARY_PERS].zfar;
}

float RenderImp::GetScreenWidth() {
    return m_Width;
}

float RenderImp::GetScreenHeight() {
    return m_Height;
}

int32_t RenderImp::GetRandomRotateTex() {
    return m_RandRotateMap;
}

float RenderImp::GetHDRAverageLum() {
    return m_AverageLum;
}

int32_t RenderImp::GetHDRSceneTex() {
    return m_HDRSceneTex;
}

float RenderImp::GetBloomWidth() {
    return m_BloomWidth;
}

float RenderImp::GetBloomHeight() {
    return m_BloomHeight;
}

void RenderImp::SetExposureLevel(float level) {
    m_ExposureLevel = level;
}

float RenderImp::GetExposureLevel() {
    return m_ExposureLevel;
}

void RenderImp::SetLightAdaption(float adaption) {
    m_LightAdaption = adaption;
}

float RenderImp::GetLightAdaption() {
    return m_LightAdaption;
}

void RenderImp::AddLine(Vector start, Vector end, Vector color) {
    if (m_DebugMesh != NULL) {
        m_DebugMesh->AddLine(start, end, color);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderImp::PreDraw() {
    std::vector<Object*> objs;

    // Add sky object
    Object* obj = glb::scene::Scene::GetSkyObject();
    if (obj != NULL) {
        objs.push_back(obj);
    }

    // Add normal object
    glb::scene::Scene::GetAllObjects(objs);

    // Sort object by shader
    std::map<std::string, ShaderGroup> opaque_group;
    std::vector<Object*> transparent_objs;
    for (int32_t i = 0; i < static_cast<int32_t>(objs.size()); i++) {
        shader::Descriptor desc = objs[i]->GetShaderDesc();
        bool is_transparent = desc.GetFlag(shader::GLB_ENABLE_ALPHA_TEX);
        if (is_transparent) {
            transparent_objs.push_back(objs[i]);
        } else {
            std::map<std::string, ShaderGroup>::iterator it = opaque_group.find(desc.GetString());
            if (it != opaque_group.end()) {
                it->second.AddObject(objs[i]);
            } else {
                shader::Descriptor desc = objs[i]->GetShaderDesc();
                shader::Program* program = shader::Mgr::GetShader(objs[i]->GetShaderProgramID());
                ShaderGroup new_group(desc, program);
                new_group.AddObject(objs[i]);
                opaque_group.insert(std::pair<std::string, ShaderGroup>(new_group.GetShaderDesc().GetString(), new_group));
            }
        }
    }

    // Transparent objects
    std::vector<ShaderGroup> transparent_group;
    {
        // Sort transparent group from far to near
        std::vector<float> obj_zvalues;
        for (int32_t i = 0; i < static_cast<int32_t>(transparent_objs.size()); i++) {
            obj_zvalues.push_back(ZValueFromCamera(transparent_objs[i]));
        }

        for (int32_t i = static_cast<int32_t>(transparent_objs.size()); i >= 0 ; i--) {
            for (int32_t j = 0; j < i - 1; j++) {
                float obj1_zvalue = obj_zvalues[j];
                float obj2_zvalue = obj_zvalues[j + 1];
                if (obj1_zvalue < obj2_zvalue) {
                    Object* temp = transparent_objs[j];
                    transparent_objs[j] = transparent_objs[j + 1];
                    transparent_objs[j + 1] = temp;

                    obj_zvalues[j] = obj2_zvalue;
                    obj_zvalues[j + 1] = obj1_zvalue;
                }
            }
        }

        // Split into groups
        for (int32_t i = 0; i < static_cast<int32_t>(transparent_objs.size()); i++) {
            if (transparent_group.size() == 0) {
                ShaderGroup new_group(transparent_objs[i]->GetShaderDesc(), shader::Mgr::GetShader(transparent_objs[i]->GetShaderProgramID()));
                new_group.AddObject(transparent_objs[i]);
                transparent_group.push_back(new_group);
            } else {
                shader::Descriptor desc = transparent_objs[i]->GetShaderDesc();
                if (transparent_group[transparent_group.size() - 1].GetShaderDesc().Equal(desc)) {
                    transparent_group[transparent_group.size() - 1].AddObject(transparent_objs[i]);
                } else {
                    ShaderGroup new_group(transparent_objs[i]->GetShaderDesc(), shader::Mgr::GetShader(transparent_objs[i]->GetShaderProgramID()));
                    new_group.AddObject(transparent_objs[i]);
                    transparent_group.push_back(new_group);
                }
            }
        }
    }

    // Merge two groups, make sure transparent group is after opaque group
    for (std::map<std::string, ShaderGroup>::iterator it = opaque_group.begin(); it != opaque_group.end(); ++it) {
        m_ShaderGroups.push_back(it->second);
    }
    for (int32_t i = 0; i < static_cast<int32_t>(transparent_group.size()); i++) {
        m_ShaderGroups.push_back(transparent_group[i]);
    }
}

void RenderImp::DrawShadowMap() {
    // Render Target
    render::Device::SetRenderTarget(m_ShadowRenderTarget->GetRenderTargetObj());

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_NONE);

    // Clear
    render::Device::SetClearDepth(1.0f);
    render::Device::Clear(CLEAR_DEPTH);

    // Shader
    shader::Program* program = shader::Mgr::GetShader(m_ShadowShader);
    std::vector<uniform::UniformEntry>& uniforms = program->GetUniforms();
    render::Device::SetShader(program->GetShader());
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Scene uniforms
    for (int32_t j = 0; j < static_cast<int32_t>(uniforms.size()); j++) {
        uniform::UniformEntry entry = uniforms[j];
        if (entry.flag) {
            // TODO: for now, id is the index of the uniform picker table
            uniform::Wrapper uniform_wrapper = uniform::kUniformPickers[entry.id].picker(NULL);
            SetUniform(entry.location, uniform_wrapper);
        }
    }

    for (int32_t i = 0; i < static_cast<int32_t>(m_ShaderGroups.size()); i++) {
        std::vector<Object*> objs = m_ShaderGroups[i].GetObjects();

        // Objects
        for (int32_t j = 0; j < static_cast<int32_t>(objs.size()); j++) {
            Object* obj = objs[j];

            // Check if cast shadow & enable depth
            if (obj->GetModel()->IsCastShadow() && obj->IsDepthTestEnable()) {
                // Object Uniform
                for (int32_t k = 0; k < static_cast<int32_t>(uniforms.size()); k++) {
                    uniform::UniformEntry entry = uniforms[k];
                    if (!entry.flag) {
                        // TODO: for now, id is the index of the uniform picker table
                        uniform::Wrapper uniform_wrapper = uniform::kUniformPickers[entry.id].picker(obj);
                        SetUniform(entry.location, uniform_wrapper);
                    }
                }

                // Vertex Buffer
                int32_t mesh_id = obj->GetModel()->GetMeshId();
                uint32_t vao = mesh::Mgr::GetMeshById(mesh_id)->GetVAO();
                uint32_t vbo = mesh::Mgr::GetMeshById(mesh_id)->GetVBO();
                VertexLayout layout = mesh::Mgr::GetMeshById(mesh_id)->GetVertexLayout();
                int32_t num = mesh::Mgr::GetMeshById(mesh_id)->GetVertexNum();
                render::Device::SetVertexArray(vao);
                render::Device::SetVertexBuffer(vbo);
                render::Device::SetVertexLayout(layout);

                if (obj->IsCullFaceEnable()) {
                    render::Device::SetCullFaceEnable(true);
                    render::Device::SetCullFaceMode(obj->GetCullFaceMode());
                } else {
                    render::Device::SetCullFaceEnable(false);
                }

                if (obj->IsDepthTestEnable()) {
                    render::Device::SetDepthTestEnable(true);
                } else {
                    render::Device::SetDepthTestEnable(false);
                }

                // Draw
                render::Device::Draw(render::PT_TRIANGLES, 0, num);
            }
        }
    }

    // Reset render target
    render::Device::SetRenderTarget(0);
}

void RenderImp::DrawAOMap() {
    GenRandRotateMap();
    DrawAO();
    BiBlurH();
    BiBlurV();
}

void RenderImp::DrawLightLoop() {
    // Render Target
    render::Device::SetRenderTarget(m_HDRRenderTarget->GetRenderTargetObj());

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

    // Clear
    render::Device::SetClearColor(1.0f, 1.0f, 1.0f);
    render::Device::SetClearDepth(1.0f);
    render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

    for (int32_t i = 0; i < static_cast<int32_t>(m_ShaderGroups.size()); i++) {
        std::vector<Object*> objs = m_ShaderGroups[i].GetObjects();
        shader::Descriptor desc = m_ShaderGroups[i].GetShaderDesc();
        shader::Program* program = m_ShaderGroups[i].GetShaderProgram();
        std::vector<uniform::UniformEntry>& uniforms = program->GetUniforms();

        // Shader
        render::Device::SetShader(program->GetShader());
        render::Device::SetShaderLayout(program->GetShaderLayout());

        // Scene uniforms
        for (int32_t j = 0; j < static_cast<int32_t>(uniforms.size()); j++) {
            uniform::UniformEntry entry = uniforms[j];
            if (entry.flag) {
                // TODO: for now, id is the index of the uniform picker table
                uniform::Wrapper uniform_wrapper = uniform::kUniformPickers[entry.id].picker(NULL);
                SetUniform(entry.location, uniform_wrapper);
            }
        }

        // Objects
        for (int32_t j = 0; j < static_cast<int32_t>(objs.size()); j++) {
            Object* obj = objs[j];

            // Textures
            if (obj->GetModel()->HasDiffuseTexture()) {
                render::Device::SetTexture(render::TS_DIFFUSE, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(Model::MT_DIFFUSE))->GetTexObj(), 0);
            }
            if (obj->GetModel()->HasAlphaTexture()) {
                render::Device::SetTexture(render::TS_ALPHA, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(Model::MT_ALPHA))->GetTexObj(), 1);
            }
            if (obj->GetModel()->HasNormalTexture()) {
                render::Device::SetTexture(render::TS_NORMAL, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(Model::MT_NORMAL))->GetTexObj(), 2);
            }
            if (obj->GetModel()->IsAcceptShadow()) {
                render::Device::SetTexture(render::TS_SHADOW, texture::Mgr::GetTextureById(m_ShadowMap)->GetTexObj(), 3);
            }
            if (obj->GetModel()->IsUseAO()) {
                render::Device::SetTexture(render::TS_AO_MAP, texture::Mgr::GetTextureById(m_AOMap)->GetTexObj(), 4);
            }

            // Object Uniform
            for (int32_t k = 0; k < static_cast<int32_t>(uniforms.size()); k++) {
                uniform::UniformEntry entry = uniforms[k];
                if (!entry.flag) {
                    // TODO: for now, id is the index of the uniform picker table
                    uniform::Wrapper uniform_wrapper = uniform::kUniformPickers[entry.id].picker(obj);
                    SetUniform(entry.location, uniform_wrapper);
                }
            }

            // Vertex Buffer
            int32_t mesh_id = obj->GetModel()->GetMeshId();
            uint32_t vao = mesh::Mgr::GetMeshById(mesh_id)->GetVAO();
            uint32_t vbo = mesh::Mgr::GetMeshById(mesh_id)->GetVBO();
            VertexLayout layout = mesh::Mgr::GetMeshById(mesh_id)->GetVertexLayout();
            int32_t num = mesh::Mgr::GetMeshById(mesh_id)->GetVertexNum();
            render::Device::SetVertexArray(vao);
            render::Device::SetVertexBuffer(vbo);
            render::Device::SetVertexLayout(layout);

            if (obj->IsCullFaceEnable()) {
                render::Device::SetCullFaceEnable(true);
                render::Device::SetCullFaceMode(obj->GetCullFaceMode());
            } else {
                render::Device::SetCullFaceEnable(false);
            }

            if (obj->IsDepthTestEnable()) {
                render::Device::SetDepthTestEnable(true);
            } else {
                render::Device::SetDepthTestEnable(false);
            }

            if (obj->IsAlphaBlendEnable()) {
                render::Device::SetAlphaBlendEnable(true);
                render::Device::SetAlphaBlendFunc(render::FACTOR_SRC, obj->GetAlphaBlendFunc(render::FACTOR_SRC));
                render::Device::SetAlphaBlendFunc(render::FACTOR_DST, obj->GetAlphaBlendFunc(render::FACTOR_DST));
            } else {
                render::Device::SetAlphaBlendEnable(false);
            }

            // Draw
            render::Device::Draw(render::PT_TRIANGLES, 0, num);
        }
    }

    // Reset render target
    render::Device::SetRenderTarget(0);
}

void RenderImp::DrawDebug() {
    // Render Target
    render::Device::SetRenderTarget(m_HDRRenderTarget->GetRenderTargetObj());

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

    // Shader
    int32_t shader_id = shader::Mgr::GetShader(m_DebugMesh->GetShaderDesc());
    shader::Program* program = shader::Mgr::GetShader(shader_id);
    render::Device::SetShader(program->GetShader());
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Scene uniforms
    std::vector<uniform::UniformEntry> uniforms = program->GetUniforms();
    for (int32_t j = 0; j < static_cast<int32_t>(uniforms.size()); j++) {
        uniform::UniformEntry entry = uniforms[j];
        if (entry.flag) {
            // TODO: for now, id is the index of the uniform picker table
            uniform::Wrapper uniform_wrapper = uniform::kUniformPickers[entry.id].picker(NULL);
            SetUniform(entry.location, uniform_wrapper);
        }
    }

    // Object Uniform
    for (int32_t k = 0; k < static_cast<int32_t>(uniforms.size()); k++) {
        uniform::UniformEntry entry = uniforms[k];
        if (entry.id == uniform::GLB_WORLDM) {
            Matrix identity;
            identity.MakeIdentityMatrix();
            render::Device::SetUniformMatrix(entry.location, identity);
            break;
        }
    }

    // Vertex Buffer
    uint32_t vao = m_DebugMesh->GetVAO();
    uint32_t vbo = m_DebugMesh->GetVBO();
    VertexLayout layout = m_DebugMesh->GetVertexLayout();
    int32_t num = m_DebugMesh->GetVertexNum();
    render::Device::SetVertexArray(vao);
    render::Device::SetVertexBuffer(vbo);
    render::Device::SetVertexLayout(layout);

    // Render State
    render::Device::SetDepthTestEnable(true);

    // Draw
    render::Device::Draw(render::PT_LINES, 0, num);

    // Reset debug mesh
    m_DebugMesh->ClearAllLines();

    // Reset render target
    render::Device::SetRenderTarget(0);
}

void RenderImp::DrawHDR() {
    DownsamplerHDRScene();
    CalcLogLum();
    CalcAverageLum();
    FilterBrightness();
    BloomH();
    BloomV();
    BlendHDRScene();
} 

void RenderImp::AfterDraw() {
    render::Device::SwapBuffer();

    // Clear all the groups
    m_ShaderGroups.clear();
}

void RenderImp::PrepareShadowMap() {
    // Create shadow render target
    m_ShadowRenderTarget = RenderTarget::Create(static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height));
    GLB_SAFE_ASSERT(m_ShadowRenderTarget != NULL);

    // Create shadow map
    texture::Texture* shadow_map = texture::Texture::CreateFloat16DepthTexture(static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height));
    if (shadow_map != NULL) {
        m_ShadowMap = texture::Mgr::AddTexture(shadow_map);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    if (m_ShadowRenderTarget != NULL) {
        m_ShadowRenderTarget->AttachDepthTexture(texture::Mgr::GetTextureById(m_ShadowMap));
    }

    // Create shadow shader
    m_ShadowShader = shader::Mgr::AddShader("..\\glb\\shader\\shadow.vs", "..\\glb\\shader\\shadow.ps");
}

void RenderImp::PrepareDepthMap() {
    // Create depth map
    texture::Texture* depth_map = texture::Texture::CreateFloat16DepthTexture(static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height));
    if (depth_map != NULL) {
        m_DepthMap = texture::Mgr::AddTexture(depth_map);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    // Create render target
    m_DepthTarget = RenderTarget::Create(static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height));
    if (m_DepthTarget != NULL) {
        m_DepthTarget->AttachDepthTexture(depth_map);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    // Create shader
    m_DepthShader = shader::Mgr::AddShader("..\\glb\\shader\\depth.vs", "..\\glb\\shader\\depth.ps");
}

void RenderImp::PrepareAOMap() {
    // Create random rotate map
    texture::Texture* random_rotate_map = texture::Texture::CreateFloat16Texture(4, 4);
    if (random_rotate_map != NULL) {
        m_RandRotateMap = texture::Mgr::AddTexture(random_rotate_map);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    // Create ao map
    texture::Texture* ao_map = texture::Texture::CreateFloat16Texture(static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height));
    if (ao_map != NULL) {
        m_AOMap = texture::Mgr::AddTexture(ao_map);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    // Create temp biblur map
    texture::Texture* biblur_map = texture::Texture::CreateFloat16Texture(static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height));
    if (biblur_map != NULL) {
        m_BiBlurMap = texture::Mgr::AddTexture(biblur_map);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    // Create render target
    m_AORenderTarget = RenderTarget::Create(static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height));
    if (m_AORenderTarget != NULL) {
        m_AORenderTarget->AttachColorTexture(render::COLORBUF_COLOR_ATTACHMENT0, ao_map);
        m_AORenderTarget->AttachColorTexture(render::COLORBUF_COLOR_ATTACHMENT1, biblur_map);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    // Create shader
    m_AOShader = shader::Mgr::AddShader("..\\glb\\shader\\ssao.vs", "..\\glb\\shader\\ssao.ps");
    m_BiBlurHShader = shader::Mgr::AddShader("..\\glb\\shader\\biblur.vs", "..\\glb\\shader\\biblurh.ps");
    m_BiBlurVShader = shader::Mgr::AddShader("..\\glb\\shader\\biblur.vs", "..\\glb\\shader\\biblurv.ps");
}

void RenderImp::PrepareHDR() {
    // Create hdr render target
    m_HDRRenderTarget = RenderTarget::Create(static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height));
    m_BloomRenderTarget = RenderTarget::Create(static_cast<int32_t>(m_Width / 2.0f), static_cast<int32_t>(m_Height / 2.0f));
    GLB_SAFE_ASSERT(m_HDRRenderTarget != NULL);
    GLB_SAFE_ASSERT(m_BloomRenderTarget != NULL);

    // Create hdr texture
    texture::Texture* hdr_tex = texture::Texture::CreateFloat16Texture(static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height));
    texture::Texture* log_lum_tex = texture::Texture::CreateFloat16Texture(static_cast<int32_t>(m_Width / 2.0f), static_cast<int32_t>(m_Height / 2.0f));
    texture::Texture* bloom_tex = texture::Texture::CreateFloat16Texture(static_cast<int32_t>(m_Width / 2.0f), static_cast<int32_t>(m_Height / 2.0f));
    if (hdr_tex != NULL && bloom_tex != NULL && log_lum_tex != NULL) {
        m_HDRSceneTex = texture::Mgr::AddTexture(hdr_tex);
        m_LogLumTex = texture::Mgr::AddTexture(log_lum_tex);
        m_BloomTex = texture::Mgr::AddTexture(bloom_tex);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    // Setup render target
    if (m_HDRRenderTarget != NULL && m_BloomRenderTarget != NULL) {
        m_HDRRenderTarget->AttachColorTexture(COLORBUF_COLOR_ATTACHMENT0, hdr_tex);
        m_BloomRenderTarget->AttachColorTexture(COLORBUF_COLOR_ATTACHMENT0, log_lum_tex);
        m_BloomRenderTarget->AttachColorTexture(COLORBUF_COLOR_ATTACHMENT1, bloom_tex);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    // Max mipmap level
    m_MaxMipmapLevel = static_cast<int32_t>(floor(log(max(m_Width / 2.0 * 1.0f, m_Height / 2.0 * 1.0f)) / log(2.0f)));

    // Bloom size
    m_BloomWidth = m_Width / 2.0f;
    m_BloomHeight = m_Height / 2.0f;

    // Create hdr shader
    m_LogLumShader = shader::Mgr::AddShader("..\\glb\\shader\\loglum.vs", "..\\glb\\shader\\loglum.ps");
    m_FilterBrightnessShader = shader::Mgr::AddShader("..\\glb\\shader\\brightfilter.vs", "..\\glb\\shader\\brightfilter.ps");
    m_BloomHShader = shader::Mgr::AddShader("..\\glb\\shader\\bloom.vs", "..\\glb\\shader\\bloomh.ps");
    m_BloomVShader = shader::Mgr::AddShader("..\\glb\\shader\\bloom.vs", "..\\glb\\shader\\bloomv.ps");
    m_TonemapShader = shader::Mgr::AddShader("..\\glb\\shader\\tonemap.vs", "..\\glb\\shader\\tonemap.ps");

    // Create screen mesh
    m_ScreenMesh = mesh::ScreenMesh::Create(static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height));
}

void RenderImp::DownsamplerHDRScene() {
    GLint obj = texture::Mgr::GetTextureById(m_HDRSceneTex)->GetTexObj();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, obj);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void RenderImp::CalcLogLum() {
    // Change viewport to match render target
    render::Device::SetViewport(0.0f, 0.0f, m_Width / 2.0f, m_Height / 2.0f);

    // Render Target
    render::Device::SetRenderTarget(m_BloomRenderTarget->GetRenderTargetObj());

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

    // Clear
    render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
    render::Device::SetClearDepth(1.0f);
    render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

    // Shader
    shader::Program* program = shader::Mgr::GetShader(m_LogLumShader);
    render::Device::SetShader(program->GetShader());
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Set texture
    render::Device::ClearTexture();
    render::Device::SetTexture(render::TS_HDRSCENE, texture::Mgr::GetTextureById(m_HDRSceneTex)->GetTexObj(), 0);

    // Scene uniforms
    std::vector<uniform::UniformEntry> uniforms = program->GetUniforms();
    for (int32_t j = 0; j < static_cast<int32_t>(uniforms.size()); j++) {
        uniform::UniformEntry entry = uniforms[j];
        if (entry.flag) {
            // TODO: for now, id is the index of the uniform picker table
            uniform::Wrapper uniform_wrapper = uniform::kUniformPickers[entry.id].picker(NULL);
            SetUniform(entry.location, uniform_wrapper);
        }
    }

    // Vertex Buffer
    uint32_t vao = m_ScreenMesh->GetVAO();
    uint32_t vbo = m_ScreenMesh->GetVBO();
    VertexLayout layout = m_ScreenMesh->GetVertexLayout();
    int32_t num = m_ScreenMesh->GetVertexNum();
    render::Device::SetVertexArray(vao);
    render::Device::SetVertexBuffer(vbo);
    render::Device::SetVertexLayout(layout);

    // Render State
    render::Device::SetDepthTestEnable(false);
    render::Device::SetCullFaceEnable(true);
    render::Device::SetCullFaceMode(render::CULL_BACK);

    // Draw
    render::Device::Draw(render::PT_TRIANGLES, 0, num);

    // Reset render target
    render::Device::SetRenderTarget(0);

    // Reset viewport
    render::Device::SetViewport(0.0f, 0.0f, m_Width, m_Height);
}

void RenderImp::CalcAverageLum() {
    GLint obj = texture::Mgr::GetTextureById(m_LogLumTex)->GetTexObj();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, obj);
    glGenerateMipmap(GL_TEXTURE_2D);

    GLfloat pixel[4];
    memset(pixel, 0, sizeof(pixel));
    glGetTexImage(GL_TEXTURE_2D, m_MaxMipmapLevel, GL_RGBA, GL_FLOAT, pixel);

    float cur_scene_average_lum = exp(pixel[0]);
    m_AverageLum = m_PreAverageLum + (cur_scene_average_lum - m_PreAverageLum) * m_LightAdaption;
    m_PreAverageLum = m_AverageLum;
}

void RenderImp::FilterBrightness() {
    // Change viewport to match render target
    render::Device::SetViewport(0.0f, 0.0f, m_Width / 2.0f, m_Height / 2.0f);

    // Render Target
    render::Device::SetRenderTarget(m_BloomRenderTarget->GetRenderTargetObj());

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

    // Clear
    render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
    render::Device::SetClearDepth(1.0f);
    render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

    // Shader
    shader::Program* program = shader::Mgr::GetShader(m_FilterBrightnessShader);
    render::Device::SetShader(program->GetShader());
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Set texture
    render::Device::ClearTexture();
    render::Device::SetTexture(render::TS_HDRSCENE, texture::Mgr::GetTextureById(m_HDRSceneTex)->GetTexObj(), 0);

    // Scene uniforms
    std::vector<uniform::UniformEntry> uniforms = program->GetUniforms();
    for (int32_t j = 0; j < static_cast<int32_t>(uniforms.size()); j++) {
        uniform::UniformEntry entry = uniforms[j];
        if (entry.flag) {
            // TODO: for now, id is the index of the uniform picker table
            uniform::Wrapper uniform_wrapper = uniform::kUniformPickers[entry.id].picker(NULL);
            SetUniform(entry.location, uniform_wrapper);
        }
    }

    // Vertex Buffer
    uint32_t vao = m_ScreenMesh->GetVAO();
    uint32_t vbo = m_ScreenMesh->GetVBO();
    VertexLayout layout = m_ScreenMesh->GetVertexLayout();
    int32_t num = m_ScreenMesh->GetVertexNum();
    render::Device::SetVertexArray(vao);
    render::Device::SetVertexBuffer(vbo);
    render::Device::SetVertexLayout(layout);

    // Render State
    render::Device::SetDepthTestEnable(false);
    render::Device::SetCullFaceEnable(true);
    render::Device::SetCullFaceMode(render::CULL_BACK);

    // Draw
    render::Device::Draw(render::PT_TRIANGLES, 0, num);

    // Reset render target
    render::Device::SetRenderTarget(0);

    // Reset viewport
    render::Device::SetViewport(0.0f, 0.0f, m_Width, m_Height);
}

void RenderImp::DownsamplerBrightness() {
    GLint obj = texture::Mgr::GetTextureById(m_HighLightSceneTex)->GetTexObj();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, obj);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void RenderImp::BloomH() {
    // Change viewport to match render target
    render::Device::SetViewport(0.0f, 0.0f, m_Width / 2.0f, m_Height / 2.0f);

    // Render Target
    render::Device::SetRenderTarget(m_BloomRenderTarget->GetRenderTargetObj());

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT1);

    // Clear
    render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
    render::Device::SetClearDepth(1.0f);
    render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

    // Shader
    shader::Program* program = shader::Mgr::GetShader(m_BloomHShader);
    render::Device::SetShader(program->GetShader());
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Set texture
    render::Device::ClearTexture();
    render::Device::SetTexture(render::TS_LOG_LUM, texture::Mgr::GetTextureById(m_LogLumTex)->GetTexObj(), 0);

    // Scene uniforms
    std::vector<uniform::UniformEntry> uniforms = program->GetUniforms();
    for (int32_t j = 0; j < static_cast<int32_t>(uniforms.size()); j++) {
        uniform::UniformEntry entry = uniforms[j];
        if (entry.flag) {
            // TODO: for now, id is the index of the uniform picker table
            uniform::Wrapper uniform_wrapper = uniform::kUniformPickers[entry.id].picker(NULL);
            SetUniform(entry.location, uniform_wrapper);
        }
    }

    // Vertex Buffer
    uint32_t vao = m_ScreenMesh->GetVAO();
    uint32_t vbo = m_ScreenMesh->GetVBO();
    VertexLayout layout = m_ScreenMesh->GetVertexLayout();
    int32_t num = m_ScreenMesh->GetVertexNum();
    render::Device::SetVertexArray(vao);
    render::Device::SetVertexBuffer(vbo);
    render::Device::SetVertexLayout(layout);

    // Render State
    render::Device::SetDepthTestEnable(false);
    render::Device::SetCullFaceEnable(true);
    render::Device::SetCullFaceMode(render::CULL_BACK);

    // Draw
    render::Device::Draw(render::PT_TRIANGLES, 0, num);

    // Reset render target
    render::Device::SetRenderTarget(0);

    // Reset viewport
    render::Device::SetViewport(0.0f, 0.0f, m_Width, m_Height);
}

void RenderImp::BloomV() {
    // Change viewport to match render target
    render::Device::SetViewport(0.0f, 0.0f, m_Width / 2.0f, m_Height / 2.0f);

    // Render Target
    render::Device::SetRenderTarget(m_BloomRenderTarget->GetRenderTargetObj());

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

    // Clear
    render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
    render::Device::SetClearDepth(1.0f);
    render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

    // Shader
    shader::Program* program = shader::Mgr::GetShader(m_BloomVShader);
    render::Device::SetShader(program->GetShader());
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Set texture
    render::Device::ClearTexture();
    render::Device::SetTexture(render::TS_HDR_BLOOM, texture::Mgr::GetTextureById(m_BloomTex)->GetTexObj(), 0);

    // Scene uniforms
    std::vector<uniform::UniformEntry> uniforms = program->GetUniforms();
    for (int32_t j = 0; j < static_cast<int32_t>(uniforms.size()); j++) {
        uniform::UniformEntry entry = uniforms[j];
        if (entry.flag) {
            // TODO: for now, id is the index of the uniform picker table
            uniform::Wrapper uniform_wrapper = uniform::kUniformPickers[entry.id].picker(NULL);
            SetUniform(entry.location, uniform_wrapper);
        }
    }

    // Vertex Buffer
    uint32_t vao = m_ScreenMesh->GetVAO();
    uint32_t vbo = m_ScreenMesh->GetVBO();
    VertexLayout layout = m_ScreenMesh->GetVertexLayout();
    int32_t num = m_ScreenMesh->GetVertexNum();
    render::Device::SetVertexArray(vao);
    render::Device::SetVertexBuffer(vbo);
    render::Device::SetVertexLayout(layout);

    // Render State
    render::Device::SetDepthTestEnable(false);
    render::Device::SetCullFaceEnable(true);
    render::Device::SetCullFaceMode(render::CULL_BACK);

    // Draw
    render::Device::Draw(render::PT_TRIANGLES, 0, num);

    // Reset render target
    render::Device::SetRenderTarget(0);

    // Reset viewport
    render::Device::SetViewport(0.0f, 0.0f, m_Width, m_Height);
}

void RenderImp::BlendHDRScene() {
    // Render Target
    render::Device::SetRenderTarget(0);

    // Clear
    render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
    render::Device::SetClearDepth(1.0f);
    render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

    // Shader
    shader::Program* program = shader::Mgr::GetShader(m_TonemapShader);
    render::Device::SetShader(program->GetShader());
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Set texture
    render::Device::ClearTexture();
    render::Device::SetTexture(render::TS_LOG_LUM, texture::Mgr::GetTextureById(m_LogLumTex)->GetTexObj(), 0);
    render::Device::SetTexture(render::TS_HDRSCENE, texture::Mgr::GetTextureById(m_HDRSceneTex)->GetTexObj(), 1);

    // Scene uniforms
    std::vector<uniform::UniformEntry> uniforms = program->GetUniforms();
    for (int32_t j = 0; j < static_cast<int32_t>(uniforms.size()); j++) {
        uniform::UniformEntry entry = uniforms[j];
        if (entry.flag) {
            // TODO: for now, id is the index of the uniform picker table
            uniform::Wrapper uniform_wrapper = uniform::kUniformPickers[entry.id].picker(NULL);
            SetUniform(entry.location, uniform_wrapper);
        }
    }

    // Vertex Buffer
    uint32_t vao = m_ScreenMesh->GetVAO();
    uint32_t vbo = m_ScreenMesh->GetVBO();
    VertexLayout layout = m_ScreenMesh->GetVertexLayout();
    int32_t num = m_ScreenMesh->GetVertexNum();
    render::Device::SetVertexArray(vao);
    render::Device::SetVertexBuffer(vbo);
    render::Device::SetVertexLayout(layout);

    // Render State
    render::Device::SetDepthTestEnable(false);
    render::Device::SetCullFaceEnable(true);
    render::Device::SetCullFaceMode(render::CULL_BACK);

    // Draw
    render::Device::Draw(render::PT_TRIANGLES, 0, num);

    // Reset render target
    render::Device::SetRenderTarget(0);
}

void RenderImp::DrawDepthMap() {
    // Render Target
    render::Device::SetRenderTarget(m_DepthTarget->GetRenderTargetObj());

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_NONE);

    // Clear
    render::Device::SetClearDepth(1.0f);
    render::Device::Clear(CLEAR_DEPTH);

    // Shader
    shader::Program* program = shader::Mgr::GetShader(m_DepthShader);
    std::vector<uniform::UniformEntry>& uniforms = program->GetUniforms();
    render::Device::SetShader(program->GetShader());
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Scene uniforms
    for (int32_t j = 0; j < static_cast<int32_t>(uniforms.size()); j++) {
        uniform::UniformEntry entry = uniforms[j];
        if (entry.flag) {
            // TODO: for now, id is the index of the uniform picker table
            uniform::Wrapper uniform_wrapper = uniform::kUniformPickers[entry.id].picker(NULL);
            SetUniform(entry.location, uniform_wrapper);
        }
    }

    for (int32_t i = 0; i < static_cast<int32_t>(m_ShaderGroups.size()); i++) {
        std::vector<Object*> objs = m_ShaderGroups[i].GetObjects();

        // Objects
        for (int32_t j = 0; j < static_cast<int32_t>(objs.size()); j++) {
            Object* obj = objs[j];

            // Check if cast shadow & enable depth
            if (obj->GetModel()->IsCastShadow() && obj->IsDepthTestEnable()) {
                // Object Uniform
                for (int32_t k = 0; k < static_cast<int32_t>(uniforms.size()); k++) {
                    uniform::UniformEntry entry = uniforms[k];
                    if (!entry.flag) {
                        // TODO: for now, id is the index of the uniform picker table
                        uniform::Wrapper uniform_wrapper = uniform::kUniformPickers[entry.id].picker(obj);
                        SetUniform(entry.location, uniform_wrapper);
                    }
                }

                // Vertex Buffer
                int32_t mesh_id = obj->GetModel()->GetMeshId();
                uint32_t vao = mesh::Mgr::GetMeshById(mesh_id)->GetVAO();
                uint32_t vbo = mesh::Mgr::GetMeshById(mesh_id)->GetVBO();
                VertexLayout layout = mesh::Mgr::GetMeshById(mesh_id)->GetVertexLayout();
                int32_t num = mesh::Mgr::GetMeshById(mesh_id)->GetVertexNum();
                render::Device::SetVertexArray(vao);
                render::Device::SetVertexBuffer(vbo);
                render::Device::SetVertexLayout(layout);

                if (obj->IsCullFaceEnable()) {
                    render::Device::SetCullFaceEnable(true);
                    render::Device::SetCullFaceMode(obj->GetCullFaceMode());
                } else {
                    render::Device::SetCullFaceEnable(false);
                }

                if (obj->IsDepthTestEnable()) {
                    render::Device::SetDepthTestEnable(true);
                } else {
                    render::Device::SetDepthTestEnable(false);
                }

                // Draw
                render::Device::Draw(render::PT_TRIANGLES, 0, num);
            }
        }
    }

    // Reset render target
    render::Device::SetRenderTarget(0);
}

void RenderImp::GenRandRotateMap() {
    // Build rand seed
    float seed = 0.0f;
    Matrix proj = m_Perspective[Render::PRIMARY_PERS].m;
    Matrix view = scene::Scene::GetCamera(scene::Scene::GetCurCamera())->GetViewMatrix();
    for (int32_t i = 0; i < 16; i++) {
        seed += proj.GetData()[i];
        seed += view.GetData()[i];
    }
    srand(floor(seed * 1000));

    // Build random rotate vector
    Vector rand_rotate_v[16];
    for (int32_t i = 0; i < 16; i++) {
        int32_t angle = rand() % 360;
        float sinv = sin(angle / 180.0f * 3.1415926f);
        float cosv = cos(angle / 180.0f * 3.1415926f);
        rand_rotate_v[i].x = sinv;
        rand_rotate_v[i].y = cosv;
        rand_rotate_v[i].z = 0.0f;
        rand_rotate_v[i].w = 0.0f;
    }

    // Update to pixels
    float pixels[16 * 4];
    for (int32_t i = 0; i < 16; i++) {
        pixels[i * 4 + 0] = rand_rotate_v[i].x;
        pixels[i * 4 + 1] = rand_rotate_v[i].y;
        pixels[i * 4 + 2] = rand_rotate_v[i].z;
        pixels[i * 4 + 3] = rand_rotate_v[i].w;
    }
    texture::Mgr::GetTextureById(m_RandRotateMap)->UpdateTexture(pixels);
}

void RenderImp::DrawAO() {
    // Render Target
    render::Device::SetRenderTarget(m_AORenderTarget->GetRenderTargetObj());

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

    // Clear
    render::Device::SetClearDepth(1.0f);
    render::Device::SetClearColor(1.0f, 1.0f, 1.0f);
    render::Device::Clear(CLEAR_DEPTH | CLEAR_COLOR);

    // Shader
    shader::Program* program = shader::Mgr::GetShader(m_AOShader);
    std::vector<uniform::UniformEntry>& uniforms = program->GetUniforms();
    render::Device::SetShader(program->GetShader());
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Texture
    render::Device::SetTexture(render::TS_DEPTH, texture::Mgr::GetTextureById(m_DepthMap)->GetTexObj(), 0);
    render::Device::SetTexture(render::TS_RANDOM_ROTATE, texture::Mgr::GetTextureById(m_RandRotateMap)->GetTexObj(), 1);

    // Scene uniforms
    for (int32_t j = 0; j < static_cast<int32_t>(uniforms.size()); j++) {
        uniform::UniformEntry entry = uniforms[j];
        if (entry.flag) {
            // TODO: for now, id is the index of the uniform picker table
            uniform::Wrapper uniform_wrapper = uniform::kUniformPickers[entry.id].picker(NULL);
            SetUniform(entry.location, uniform_wrapper);
        }
    }

    // Vertex Buffer
    uint32_t vao = m_ScreenMesh->GetVAO();
    uint32_t vbo = m_ScreenMesh->GetVBO();
    VertexLayout layout = m_ScreenMesh->GetVertexLayout();
    int32_t num = m_ScreenMesh->GetVertexNum();
    render::Device::SetVertexArray(vao);
    render::Device::SetVertexBuffer(vbo);
    render::Device::SetVertexLayout(layout);

    // Draw
    render::Device::Draw(render::PT_TRIANGLES, 0, num);

    // Reset render target
    render::Device::SetRenderTarget(0);
}

void RenderImp::BiBlurH() {
    // Render Target
    render::Device::SetRenderTarget(m_AORenderTarget->GetRenderTargetObj());

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT1);

    // Clear
    render::Device::SetClearDepth(1.0f);
    render::Device::SetClearColor(1.0f, 1.0f, 1.0f);
    render::Device::Clear(CLEAR_DEPTH | CLEAR_COLOR);

    // Shader
    shader::Program* program = shader::Mgr::GetShader(m_BiBlurHShader);
    std::vector<uniform::UniformEntry>& uniforms = program->GetUniforms();
    render::Device::SetShader(program->GetShader());
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Texture
    render::Device::SetTexture(render::TS_DEPTH, texture::Mgr::GetTextureById(m_DepthMap)->GetTexObj(), 0);
    render::Device::SetTexture(render::TS_AO_MAP, texture::Mgr::GetTextureById(m_AOMap)->GetTexObj(), 1);

    // Scene uniforms
    for (int32_t j = 0; j < static_cast<int32_t>(uniforms.size()); j++) {
        uniform::UniformEntry entry = uniforms[j];
        if (entry.flag) {
            // TODO: for now, id is the index of the uniform picker table
            uniform::Wrapper uniform_wrapper = uniform::kUniformPickers[entry.id].picker(NULL);
            SetUniform(entry.location, uniform_wrapper);
        }
    }

    // Vertex Buffer
    uint32_t vao = m_ScreenMesh->GetVAO();
    uint32_t vbo = m_ScreenMesh->GetVBO();
    VertexLayout layout = m_ScreenMesh->GetVertexLayout();
    int32_t num = m_ScreenMesh->GetVertexNum();
    render::Device::SetVertexArray(vao);
    render::Device::SetVertexBuffer(vbo);
    render::Device::SetVertexLayout(layout);

    // Draw
    render::Device::Draw(render::PT_TRIANGLES, 0, num);

    // Reset render target
    render::Device::SetRenderTarget(0);
}

void RenderImp::BiBlurV() {
    // Render Target
    render::Device::SetRenderTarget(m_AORenderTarget->GetRenderTargetObj());

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

    // Clear
    render::Device::SetClearDepth(1.0f);
    render::Device::SetClearColor(1.0f, 1.0f, 1.0f);
    render::Device::Clear(CLEAR_DEPTH | CLEAR_COLOR);

    // Shader
    shader::Program* program = shader::Mgr::GetShader(m_BiBlurVShader);
    std::vector<uniform::UniformEntry>& uniforms = program->GetUniforms();
    render::Device::SetShader(program->GetShader());
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Texture
    render::Device::SetTexture(render::TS_DEPTH, texture::Mgr::GetTextureById(m_DepthMap)->GetTexObj(), 0);
    render::Device::SetTexture(render::TS_BI_BLUR_MAP, texture::Mgr::GetTextureById(m_BiBlurMap)->GetTexObj(), 1);

    // Scene uniforms
    for (int32_t j = 0; j < static_cast<int32_t>(uniforms.size()); j++) {
        uniform::UniformEntry entry = uniforms[j];
        if (entry.flag) {
            // TODO: for now, id is the index of the uniform picker table
            uniform::Wrapper uniform_wrapper = uniform::kUniformPickers[entry.id].picker(NULL);
            SetUniform(entry.location, uniform_wrapper);
        }
    }

    // Vertex Buffer
    uint32_t vao = m_ScreenMesh->GetVAO();
    uint32_t vbo = m_ScreenMesh->GetVBO();
    VertexLayout layout = m_ScreenMesh->GetVertexLayout();
    int32_t num = m_ScreenMesh->GetVertexNum();
    render::Device::SetVertexArray(vao);
    render::Device::SetVertexBuffer(vbo);
    render::Device::SetVertexLayout(layout);

    // Draw
    render::Device::Draw(render::PT_TRIANGLES, 0, num);

    // Reset render target
    render::Device::SetRenderTarget(0);
}

void RenderImp::SetUniform(int32_t location, uniform::Wrapper& wrapper) {
    switch (wrapper.GetFormat()) {
    case uniform::Wrapper::FMT_INT:
        {
            int32_t value = wrapper.GetInt();
            render::Device::SetUniform1i(location, value);
        }
        break;

    case uniform::Wrapper::FMT_SAMPLER:
        {
            int32_t sampler = wrapper.GetSampler();
            render::Device::SetUniformSampler(location, sampler);
        }
        break;

    case uniform::Wrapper::FMT_FLOAT:
        {
            float value = wrapper.GetFloat();
            render::Device::SetUniform1f(location, value);
        }
        break;

    case uniform::Wrapper::FMT_FLOAT3:
        {
            Vector values = wrapper.GetVector();
            render::Device::SetUniform3f(location, values);
        }
        break;

    case uniform::Wrapper::FMT_MATRIX:
        {
            Matrix m = wrapper.GetMatrix();
            render::Device::SetUniformMatrix(location, m);
        }
        break;
    }
}

float RenderImp::ZValueFromCamera(Object* obj) {
    float result = 0.0f;

    camera::CameraBase* cam = scene::Scene::GetCamera(scene::PRIMIAY_CAM);
    if (obj != NULL && cam != NULL) {
        Vector cam_pos = cam->GetPos();
        Vector cam_target = cam->GetTarget();
        Vector obj_pos = obj->GetPos();
        //Vector obj_to_cam = obj_pos - cam_pos;
        //Vector target_to_cam = cam_target - cam_pos;
        //result = Dot(obj_to_cam, target_to_cam);
        // The code below is the calculation of above code
        result = 
            (obj_pos.x * cam_target.x) - (obj_pos.x * cam_pos.x) + (cam_pos.x * cam_pos.x) +
            (obj_pos.y * cam_target.y) - (obj_pos.y * cam_pos.y) + (cam_pos.y * cam_pos.y) +
            (obj_pos.z * cam_target.z) - (obj_pos.z * cam_pos.z) + (cam_pos.z * cam_pos.z);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

//-----------------------------------------------------------------------------------
// Render DEFINITION
//-----------------------------------------------------------------------------------
void Render::Initialize(int32_t width, int32_t height) {
    if (s_RenderImp == NULL) {
        s_RenderImp = new RenderImp;
        if (s_RenderImp != NULL) {
            s_RenderImp->Initialize(width, height);
        } else {
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Render::Destroy() {
    if (s_RenderImp != NULL) {
        s_RenderImp->Destroy();
        GLB_SAFE_DELETE(s_RenderImp);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Render::Draw() {
    if (s_RenderImp != NULL) {
        s_RenderImp->Draw();
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Render::SetPerspective(int32_t type, float fov, float aspect, float znear, float zfar) {
    if (s_RenderImp != NULL) {
        s_RenderImp->SetPerspective(type, fov, aspect, znear, zfar);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

Matrix Render::GetPerspective(int32_t type) {
    Matrix proj;

    if (s_RenderImp != NULL) {
        proj = s_RenderImp->GetPerspective(type);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return proj;
}

void Render::SetCurPerspectiveType(int32_t type) {
    if (s_RenderImp != NULL) {
        s_RenderImp->SetCurPerspectiveType(type);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

int32_t Render::GetCurPerspectiveType() {
    int32_t result = PRIMARY_PERS;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetCurPerspectiveType();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

Vector Render::GetFrustumPointInView(int32_t index) {
    Vector v;

    if (s_RenderImp != NULL) {
        v = s_RenderImp->GetFrustumPointInView(index);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return v;
}

float Render::GetFarClip() {
    float result;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetFarClip();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

float Render::GetScreenWidth() {
    float result = 0.0f;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetScreenWidth();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

float Render::GetScreenHeight() {
    float result = 0.0f;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetScreenHeight();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Render::GetRandomRotateTex() {
    int32_t result = 0;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetRandomRotateTex();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

float Render::GetHDRAverageLum() {
    float result = 0.0f;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetHDRAverageLum();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Render::GetHDRSceneTex() {
    int32_t result = -1;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetHDRSceneTex();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

float Render::GetBloomWidth() {
    float result = 0.0f;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetBloomWidth();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

float Render::GetBloomHeight() {
    float result = 0.0f;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetBloomHeight();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

void Render::SetExposureLevel(float level) {
    if (s_RenderImp != NULL) {
        s_RenderImp->SetExposureLevel(level);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

float Render::GetExposureLevel() {
    float result = 0.0f;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetExposureLevel();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

void Render::SetLightAdaption(float adaption) {
    if (s_RenderImp != NULL) {
        s_RenderImp->SetLightAdaption(adaption);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

float Render::GetLightAdaption() {
    float result = 0.0f;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetLightAdaption();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

void Render::AddLine(Vector start, Vector end, Vector color) {
    if (s_RenderImp != NULL) {
        s_RenderImp->AddLine(start, end, color);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}
};  // namespace render

};  // namespace glb