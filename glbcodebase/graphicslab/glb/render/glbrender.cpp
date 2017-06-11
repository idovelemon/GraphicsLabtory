//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 10 / 28
// Brief: Draw the entire scene
//------------------------------------------------------------------
#include "glbrender.h"

#include <map>

#include "glbapplication.h"
#include "glbmesh.h"
#include "glbrenderdevice.h"
#include "glbrendertarget.h"
#include "glbshader.h"
#include "glbtexture.h"

#include "scene/glbcamera.h"
#include "scene/glbobject.h"
#include "scene/glbscene.h"

#include "util/glblog.h"
#include "util/glbmacro.h"
#include "util/glbprofile.h"

#ifdef GLB_PLATFORM_OPENGL
#include "GL/glew.h"
#endif

namespace glb {

namespace render {

//-----------------------------------------------------------------------------------
// CONSTANT VALUE
//----------------------------------------------------------------------------------
static const int32_t kDefaultEnvBakersNum = 4;
static const int32_t kPSSMSplitNum = 4;
static const float kMaxSceneSize = 100000.0f;

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
    math::Matrix m;
};

struct EnvBaker {
    RenderTarget* target;
    scene::Object* obj;
    int32_t tex_id;
    int32_t width;
    int32_t height;

    EnvBaker()
    : target(NULL)
    , obj(NULL)
    , tex_id(-1)
    , width(0)
    , height(0) {
    }
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
    void AddObject(scene::Object* obj);
    shader::Descriptor GetShaderDesc();
    std::vector<scene::Object*>& GetObjects();
    shader::Program* GetShaderProgram();

private:
    shader::Descriptor              m_ShaderDesc;
    shader::Program*                m_ShaderProgram;
    std::vector<scene::Object*>     m_Objects;
};

//-----------------------------------------------------------------------------------
// Frustum DECLARATION
//----------------------------------------------------------------------------------
class Frustum {
public:
    Frustum();
    virtual ~Frustum();

    //----------------------------------------
    // @brief: Build the frustum according the input 8 points
    //----------------------------------------
    void Build(math::Vector* points);

    //----------------------------------------
    // @brief: Split the frustum along the z-axis
    // @param: begin The begin ratio of frustum
    // @param: end The end ratio of frustum
    // @ret: frustum The splited frustum
    // @example: Split(0.0f, 0.5f, frustum)
    // This example split current frustum into half, and save the front-half to result.
    //----------------------------------------
    void Split(float begin, float end, Frustum& frustum);

    math::AABB GetBoundBox();
    void GetPoints(math::Vector* points);

    void AddObject(scene::Object* obj);
    void GetAllObjects(std::vector<scene::Object*>& objs);
    void RemoveAllObjects();

protected:
    GLB_OVERRIDE_COPY_AND_ASSIGN(Frustum);

private:
    math::Vector                    m_Points[8];
    std::vector<scene::Object*>     m_Objects;
};

//-----------------------------------------------------------------------------------
// RenderImp DECLARATION
//----------------------------------------------------------------------------------
class RenderImp {
public:
    RenderImp();
    virtual ~RenderImp();

public:
    void Initialize(int32_t width, int32_t height);
    void InitializeAfterUserApp();
    void Destroy();
    void Draw();

    void SetPerspective(int32_t type, float fov, float aspect, float znear, float zfar);
    math::Matrix& GetPerspective(int32_t type);
    void SetCurPerspectiveType(int32_t type);
    int32_t GetCurPerspectiveType();
    math::Vector GetFrustumPointInView(int32_t index);
    float GetFarClip();
    int32_t GetScreenWidth();
    int32_t GetScreenHeight();

    math::Matrix GetShadowMapMatrix(int32_t index);
    int32_t GetCurShadowMapIndex();
    float GetShadowSplitValue(int32_t index);

    int32_t GetRandomRotateTex();

    float GetHDRAverageLum();
    int32_t GetHDRSceneTex();
    float GetBloomWidth();
    float GetBloomHeight();
    void SetExposureLevel(float level);
    float GetExposureLevel();
    void SetLightAdaption(float adaption);
    float GetLightAdaption();
    void SetHighLightBase(float base);
    float GetHighLightBase();

    int32_t RequestBakeEnvMap(int32_t width, int32_t height, scene::Object* obj);
    void CancleBakeEnvMap(scene::Object* obj);

    void AddLine(math::Vector start, math::Vector end, math::Vector color);
    void AddBoundBox(math::AABB bv, math::Vector color);

protected:
    void PreDraw();
    void DrawShadowMap();
    void DrawAOMap();
    void DrawEnvMap();
    void DrawLightLoop();
    void DrawDebug();
    void DrawHDR();
    void AfterDraw();

    void PrepareShadowMap();
    void PrepareDepthMap();
    void PrepareAOMap();
    void PrepareHDR();
    void PrepareEnvMap();

    // PSSM
    void SplitFrustum();
    void SplitShadowReceivers();
    math::Matrix BuildRefLightSpaceMatrix();
    void BuildBasicLightFrustums(math::Matrix light_space_to_world_space);
    void SplitShadowCasters(math::Matrix light_space_to_world_space);
    void ShrinkAllLightFrustums(math::Matrix light_space_to_world_space);
    void BuildShadowMatrixs(math::Matrix light_space_to_world_space);
    void DrawShadowMapCore();
    math::AABB CalcAllObjectsBoundBox(math::Matrix trans, Frustum& frustum);
    void ShrinkLightFrustum(math::AABB caster, math::AABB receiver, Frustum& frustum);
    void CalculateFrustumInView(Frustum& frustum);
    void SplitObjIntoFrustum(std::vector<scene::Object*>& objs, Frustum& frustum, math::Matrix trans);

    // Light Loop
    void PreDrawLightLoop();
    void DrawLightLoopCore();

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

    // Env
    void PreDrawEnvMap();
    void DrawEnvMapCore();

    void SetUniform(int32_t location, uniform::Wrapper& wrapper);
    inline float ZValueFromCamera(scene::Object* obj);

private:
    int32_t                                 m_Width;
    int32_t                                 m_Height;
    std::vector<ShaderGroup>                m_ShaderGroups;

    // Perspective
    int32_t                                 m_PerspectiveType;
    PerspectiveProj                         m_Perspective[Render::MAX_PERS];

    // Shadow
    RenderTarget*                           m_ShadowRenderTarget[kPSSMSplitNum];
    int32_t                                 m_ShadowMap[kPSSMSplitNum];
    int32_t                                 m_ShadowShader;
    Frustum                                 m_SplitFrustums[kPSSMSplitNum];
    Frustum                                 m_LightSpaceFrustum[kPSSMSplitNum];
    math::Matrix                            m_ShadowMatrix[kPSSMSplitNum];
    int32_t                                 m_ShadowMapIndex;
    float                                   m_ShadowSplitValue[kPSSMSplitNum - 1];

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
    float                                   m_HighLightBase;

    // SSAO
    RenderTarget*                           m_AORenderTarget;
    int32_t                                 m_RandRotateMap;
    int32_t                                 m_AOMap;
    int32_t                                 m_BiBlurMap;
    int32_t                                 m_AOShader;
    int32_t                                 m_BiBlurHShader;
    int32_t                                 m_BiBlurVShader;

    // Env Map
    std::vector<EnvBaker>                   m_EnvBakers;

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

void ShaderGroup::AddObject(scene::Object* obj) {
    m_Objects.push_back(obj);
}

shader::Descriptor ShaderGroup::GetShaderDesc() {
    return m_ShaderDesc;
}

std::vector<scene::Object*>& ShaderGroup::GetObjects() {
    return m_Objects;
}

shader::Program* ShaderGroup::GetShaderProgram() {
    return m_ShaderProgram;
}

//-----------------------------------------------------------------------------------
// Frustum DEFINITION
//-----------------------------------------------------------------------------------
Frustum::Frustum() {
    memset(m_Points, 0, sizeof(m_Points));
}

Frustum::~Frustum() {
    m_Objects.clear();
}

void Frustum::Build(math::Vector* points) {
    if (points) {
        memcpy(m_Points, points, 8 * sizeof(math::Vector));
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Frustum::Split(float begin, float end, Frustum& frustum) {
    math::Vector dir_ld = m_Points[Render::FLD] - m_Points[Render::NLD];
    math::Vector dir_lu = m_Points[Render::FLU] - m_Points[Render::NLU];
    math::Vector dir_rd = m_Points[Render::FRD] - m_Points[Render::NRD];
    math::Vector dir_ru = m_Points[Render::FRU] - m_Points[Render::NRU];

    math::Vector points[8];
    points[Render::NLD] = m_Points[Render::NLD] + (dir_ld * begin);
    points[Render::NRD] = m_Points[Render::NRD] + (dir_rd * begin);
    points[Render::NLU] = m_Points[Render::NLU] + (dir_lu * begin);
    points[Render::NRU] = m_Points[Render::NRU] + (dir_ru * begin);
    points[Render::FLD] = m_Points[Render::NLD] + (dir_ld * end);
    points[Render::FRD] = m_Points[Render::NRD] + (dir_rd * end);
    points[Render::FLU] = m_Points[Render::NLU] + (dir_lu * end);
    points[Render::FRU] = m_Points[Render::NRU] + (dir_ru * end);
    frustum.Build(points);
}

math::AABB Frustum::GetBoundBox() {
    return math::AABB(m_Points);
}

void Frustum::GetPoints(math::Vector* points) {
    if (points != NULL) {
        memcpy(points, m_Points, sizeof(m_Points));
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Frustum::AddObject(scene::Object* obj) {
    m_Objects.push_back(obj);
}

void Frustum::GetAllObjects(std::vector<scene::Object*>& objs) {
    objs = m_Objects;
}

void Frustum::RemoveAllObjects() {
    m_Objects.clear();
}
//-----------------------------------------------------------------------------------
// RenderImp DEFINITION
//-----------------------------------------------------------------------------------
RenderImp::RenderImp()
: m_Width(0)
, m_Height(0)
, m_PerspectiveType(Render::PRIMARY_PERS)

// Shadow
, m_ShadowShader(-1)
, m_ShadowMapIndex(-1)

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
, m_HighLightBase(0.0f)

// SSAO
, m_AORenderTarget(NULL)
, m_RandRotateMap(-1)
, m_AOMap(-1)
, m_BiBlurMap(-1)
, m_AOShader(-1)
, m_BiBlurHShader(-1)
, m_BiBlurVShader(-1)

// Env

, m_DebugMesh(NULL) {
    memset(m_Perspective, 0, sizeof(m_Perspective));
    m_ShaderGroups.clear();
    memset(m_ShadowRenderTarget, 0, sizeof(m_ShadowRenderTarget));
    memset(m_ShadowMap, 0, sizeof(m_ShadowMap));
    memset(m_SplitFrustums, 0, sizeof(m_SplitFrustums));
    memset(m_LightSpaceFrustum, 0, sizeof(m_LightSpaceFrustum));
    memset(m_ShadowMatrix, 0, sizeof(m_ShadowMatrix));
    memset(m_ShadowSplitValue, 0, sizeof(m_ShadowSplitValue));
    m_EnvBakers.clear();
}

RenderImp::~RenderImp() {
    Destroy();
}

void RenderImp::Initialize(int32_t width, int32_t height) {
    m_ShaderGroups.clear();

    m_Width = width;
    m_Height = height;

    PrepareShadowMap();
    PrepareDepthMap();
    PrepareAOMap();
    PrepareHDR();
    PrepareEnvMap();

    m_DebugMesh = mesh::DebugMesh::Create();
}

void RenderImp::InitializeAfterUserApp() {
    // PSSM
    SplitFrustum();
}

void RenderImp::Destroy() {
    m_ShaderGroups.clear();

    m_Width = 0;
    m_Height = 0;
    m_PerspectiveType = Render::PRIMARY_PERS;

    // Shadow
    for (int32_t i = 0; i < kPSSMSplitNum; i++) {
        GLB_SAFE_DELETE(m_ShadowRenderTarget[i]);
        m_ShadowMap[i] = -1;
    }
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
    GLB_SAFE_DELETE(m_ScreenMesh);
}

void RenderImp::Draw() {
    PreDraw();
    DrawShadowMap();
    DrawDepthMap();
    DrawAOMap();
    DrawEnvMap();
    DrawLightLoop();
    DrawDebug();
    DrawHDR();
    AfterDraw();

#ifdef GLB_PLATFORM_OPENGL
    //GLB_CHECK_GL_ERROR;
#endif
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

math::Matrix& RenderImp::GetPerspective(int32_t type) {
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

math::Vector RenderImp::GetFrustumPointInView(int32_t index) {
    math::Vector v(0.0f, 0.0f, 0.0f);

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

int32_t RenderImp::GetScreenWidth() {
    return m_Width;
}

int32_t RenderImp::GetScreenHeight() {
    return m_Height;
}

math::Matrix RenderImp::GetShadowMapMatrix(int32_t index) {
    return m_ShadowMatrix[index];
}

int32_t RenderImp::GetCurShadowMapIndex() {
    return m_ShadowMapIndex;
}

float RenderImp::GetShadowSplitValue(int32_t index) {
    return m_ShadowSplitValue[index];
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

void RenderImp::SetHighLightBase(float base) {
    m_HighLightBase = base;
}

float RenderImp::GetHighLightBase() {
    return m_HighLightBase;
}

int32_t RenderImp::RequestBakeEnvMap(int32_t width, int32_t height, scene::Object* obj) {
    int32_t result = -1;

    RenderTarget* rt = RenderTarget::Create(width, height);
    texture::Texture* env_map = texture::Texture::CreateFloat16CubeTexture(width, height);
    if (rt != NULL && env_map != NULL && obj != NULL) {
        DrawColorBuffer attach_pos[6] = {
            COLORBUF_COLOR_ATTACHMENT0,
            COLORBUF_COLOR_ATTACHMENT1,
            COLORBUF_COLOR_ATTACHMENT2,
            COLORBUF_COLOR_ATTACHMENT3,
            COLORBUF_COLOR_ATTACHMENT4,
            COLORBUF_COLOR_ATTACHMENT5
        };
        rt->AttachCubeTexture(attach_pos, env_map);
        result = texture::Mgr::AddTexture(env_map);
        EnvBaker baker;
        baker.target = rt;
        baker.obj = obj;
        baker.tex_id = result;
        baker.width = width;
        baker.height = height;
        m_EnvBakers.push_back(baker);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

void RenderImp::CancleBakeEnvMap(scene::Object* obj) {
    if (obj != NULL) {
        if (!m_EnvBakers.empty()) {
            std::vector<EnvBaker>::iterator it = m_EnvBakers.begin();
            for (; it != m_EnvBakers.end(); ++it) {
                if (it->obj == obj) {
                    GLB_SAFE_DELETE(it->target);
                    it->tex_id = -1;
                    it->obj = NULL;
                    m_EnvBakers.erase(it);
                    break;
                }
            }
        }
    }
}

void RenderImp::AddLine(math::Vector start, math::Vector end, math::Vector color) {
    if (m_DebugMesh != NULL) {
        m_DebugMesh->AddLine(start, end, color);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderImp::AddBoundBox(math::AABB bv, math::Vector color) {
    math::Vector points[8];
    points[render::Render::NLU] = math::Vector(bv.m_Min.x, bv.m_Max.y, bv.m_Max.z);
    points[render::Render::NLD] = math::Vector(bv.m_Min.x, bv.m_Min.y, bv.m_Max.z);
    points[render::Render::NRU] = math::Vector(bv.m_Max.x, bv.m_Max.y, bv.m_Max.z);
    points[render::Render::NRD] = math::Vector(bv.m_Max.x, bv.m_Min.y, bv.m_Max.z);
    points[render::Render::FLU] = math::Vector(bv.m_Min.x, bv.m_Max.y, bv.m_Min.z);
    points[render::Render::FLD] = math::Vector(bv.m_Min.x, bv.m_Min.y, bv.m_Min.z);
    points[render::Render::FRU] = math::Vector(bv.m_Max.x, bv.m_Max.y, bv.m_Min.z);
    points[render::Render::FRD] = math::Vector(bv.m_Max.x, bv.m_Min.y, bv.m_Min.z);

    render::Render::AddLine(points[render::Render::NLU], points[render::Render::NRU], color);
    render::Render::AddLine(points[render::Render::NRU], points[render::Render::NRD], color);
    render::Render::AddLine(points[render::Render::NRD], points[render::Render::NLD], color);
    render::Render::AddLine(points[render::Render::NLD], points[render::Render::NLU], color);
    render::Render::AddLine(points[render::Render::FLU], points[render::Render::FRU], color);
    render::Render::AddLine(points[render::Render::FRU], points[render::Render::FRD], color);
    render::Render::AddLine(points[render::Render::FRD], points[render::Render::FLD], color);
    render::Render::AddLine(points[render::Render::FLD], points[render::Render::FLU], color);
    render::Render::AddLine(points[render::Render::NLD], points[render::Render::FLD], color);
    render::Render::AddLine(points[render::Render::NLU], points[render::Render::FLU], color);
    render::Render::AddLine(points[render::Render::NRU], points[render::Render::FRU], color);
    render::Render::AddLine(points[render::Render::NRD], points[render::Render::FRD], color);
}

void RenderImp::PreDraw() {
    //{  // Draw split frustum
    //    math::Matrix view_to_world = scene::Scene::GetCamera(scene::PRIMIAY_CAM)->GetViewMatrix();
    //    view_to_world.Inverse();
    //    math::Vector colors[kPSSMSplitNum];
    //    colors[0] = math::Vector(1.0f, 0.0f, 0.0f);
    //    colors[1] = math::Vector(0.0f, 1.0f, 1.0f);
    //    colors[2] = math::Vector(0.0f, 0.0f, 1.0f);
    //    colors[3] = math::Vector(1.0f, 1.0f, 0.0f);
    //    for (int32_t i = 0; i < kPSSMSplitNum; i++) {
    //        math::Vector color = colors[i];
    //        math::Vector points[8];
    //        m_SplitFrustums[i].GetPoints(points);

    //        for (int32_t j = 0; j < 8; j++) {
    //            points[j] = view_to_world * points[j];
    //        }

    //        render::Render::AddLine(points[render::Render::NLU], points[render::Render::NRU], color);
    //        render::Render::AddLine(points[render::Render::NRU], points[render::Render::NRD], color);
    //        render::Render::AddLine(points[render::Render::NRD], points[render::Render::NLD], color);
    //        render::Render::AddLine(points[render::Render::NLD], points[render::Render::NLU], color);
    //        render::Render::AddLine(points[render::Render::FLU], points[render::Render::FRU], color);
    //        render::Render::AddLine(points[render::Render::FRU], points[render::Render::FRD], color);
    //        render::Render::AddLine(points[render::Render::FRD], points[render::Render::FLD], color);
    //        render::Render::AddLine(points[render::Render::FLD], points[render::Render::FLU], color);
    //        render::Render::AddLine(points[render::Render::NLD], points[render::Render::FLD], color);
    //        render::Render::AddLine(points[render::Render::NLU], points[render::Render::FLU], color);
    //        render::Render::AddLine(points[render::Render::NRU], points[render::Render::FRU], color);
    //        render::Render::AddLine(points[render::Render::NRD], points[render::Render::FRD], color);
    //    }
    //}
}

void RenderImp::DrawShadowMap() {
    SplitShadowReceivers();
    math::Matrix light_space_to_world_space = BuildRefLightSpaceMatrix();
    BuildBasicLightFrustums(light_space_to_world_space);
    SplitShadowCasters(light_space_to_world_space);
    ShrinkAllLightFrustums(light_space_to_world_space);
    BuildShadowMatrixs(light_space_to_world_space);
    DrawShadowMapCore();
}

void RenderImp::DrawAOMap() {
    GenRandRotateMap();
    DrawAO();
    BiBlurH();
    BiBlurV();
}

void RenderImp::DrawEnvMap() {
    PreDrawEnvMap();
    DrawEnvMapCore();
}

void RenderImp::DrawLightLoop() {
    PreDrawLightLoop();
    DrawLightLoopCore();
}

void RenderImp::DrawDebug() {
    // Render Target
    render::Device::SetRenderTarget(m_HDRRenderTarget);

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

    // Shader
    int32_t shader_id = shader::Mgr::GetShader(m_DebugMesh->GetShaderDesc());
    shader::Program* program = shader::Mgr::GetShader(shader_id);
    render::Device::SetShader(program);
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

    // Scene Object Uniform
    for (int32_t k = 0; k < static_cast<int32_t>(uniforms.size()); k++) {
        uniform::UniformEntry entry = uniforms[k];
        if (entry.id == uniform::GLB_WORLDM) {
            math::Matrix identity;
            identity.MakeIdentityMatrix();
            render::Device::SetUniformMatrix(entry.location, identity);
            break;
        }
    }

    // Vertex Buffer
    VertexLayout layout = m_DebugMesh->GetVertexLayout();
    int32_t num = m_DebugMesh->GetVertexNum();
    render::Device::SetVertexBuffer(m_DebugMesh->GetVertexBuffer());
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
}

void RenderImp::PrepareShadowMap() {
    int32_t shadow_map_width = app::Application::GetShadowMapWidth();
    int32_t shadow_map_height = app::Application::GetShadowMapHeight();

    for (int32_t i = 0; i < kPSSMSplitNum; i++) {
        // Create shadow render target
        m_ShadowRenderTarget[i] = RenderTarget::Create(shadow_map_width, shadow_map_height);
        GLB_SAFE_ASSERT(m_ShadowRenderTarget[i] != NULL);

        // Create shadow map
        texture::Texture* shadow_map = texture::Texture::CreateFloat16DepthTexture(shadow_map_width, shadow_map_height);
        if (shadow_map != NULL) {
            m_ShadowMap[i] = texture::Mgr::AddTexture(shadow_map);
        } else {
            GLB_SAFE_ASSERT(false);
        }

        if (m_ShadowRenderTarget[i] != NULL) {
            m_ShadowRenderTarget[i]->AttachDepthTexture(texture::Mgr::GetTextureById(m_ShadowMap[i]));
        }
    }

    // Create shadow shader
    m_ShadowShader = shader::Mgr::AddShader("..\\glb\\shader\\shadow.vs", "..\\glb\\shader\\shadow.fs");
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
    m_DepthShader = shader::Mgr::AddShader("..\\glb\\shader\\depth.vs", "..\\glb\\shader\\depth.fs");
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
    m_AOShader = shader::Mgr::AddShader("..\\glb\\shader\\ssao.vs", "..\\glb\\shader\\ssao.fs");
    m_BiBlurHShader = shader::Mgr::AddShader("..\\glb\\shader\\biblur.vs", "..\\glb\\shader\\biblurh.fs");
    m_BiBlurVShader = shader::Mgr::AddShader("..\\glb\\shader\\biblur.vs", "..\\glb\\shader\\biblurv.fs");
}

void RenderImp::PrepareHDR() {
    // Create hdr render target
    m_HDRRenderTarget = RenderTarget::Create(static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height));
    m_BloomRenderTarget = RenderTarget::Create(static_cast<int32_t>(m_Width / 4.0f), static_cast<int32_t>(m_Height / 4.0f));
    GLB_SAFE_ASSERT(m_HDRRenderTarget != NULL);
    GLB_SAFE_ASSERT(m_BloomRenderTarget != NULL);

    // Create hdr texture
    texture::Texture* hdr_tex = texture::Texture::CreateFloat16Texture(static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height));
    texture::Texture* log_lum_tex = texture::Texture::CreateFloat16Texture(static_cast<int32_t>(m_Width / 4.0f), static_cast<int32_t>(m_Height / 4.0f));
    texture::Texture* bloom_tex = texture::Texture::CreateFloat16Texture(static_cast<int32_t>(m_Width / 4.0f), static_cast<int32_t>(m_Height / 4.0f));
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
    m_MaxMipmapLevel = static_cast<int32_t>(floor(log(max(m_Width / 4.0 * 1.0f, m_Height / 4.0 * 1.0f)) / log(2.0f)));

    // Bloom size
    m_BloomWidth = m_Width / 4.0f;
    m_BloomHeight = m_Height / 4.0f;

    // Create hdr shader
    m_LogLumShader = shader::Mgr::AddShader("..\\glb\\shader\\loglum.vs", "..\\glb\\shader\\loglum.fs");
    m_FilterBrightnessShader = shader::Mgr::AddShader("..\\glb\\shader\\brightfilter.vs", "..\\glb\\shader\\brightfilter.fs");
    m_BloomHShader = shader::Mgr::AddShader("..\\glb\\shader\\bloom.vs", "..\\glb\\shader\\bloomh.fs");
    m_BloomVShader = shader::Mgr::AddShader("..\\glb\\shader\\bloom.vs", "..\\glb\\shader\\bloomv.fs");
    m_TonemapShader = shader::Mgr::AddShader("..\\glb\\shader\\tonemap.vs", "..\\glb\\shader\\tonemap.fs");

    // Create screen mesh
    m_ScreenMesh = mesh::ScreenMesh::Create(static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height));
}

void RenderImp::PrepareEnvMap() {
}

void RenderImp::SplitFrustum() {
    // Calculate current frustum in view space
    Frustum frustum;
    CalculateFrustumInView(frustum);

    // Calculate the practical split scheme z value
    float z_near = m_Perspective[m_PerspectiveType].znear;
    float z_far = m_Perspective[m_PerspectiveType].zfar;
    float split_scheme_practicals[kPSSMSplitNum + 1];
    for (int32_t i = 0; i <= kPSSMSplitNum; i++) {
        float split_scheme_log = z_near * pow(z_far / z_near, 1.0f * i / kPSSMSplitNum);
        float split_scheme_uni = z_near + (z_far - z_near) * (1.0f * i / kPSSMSplitNum);
        float split_scheme_practical = split_scheme_log * 0.5f + split_scheme_uni * 0.5f;
        split_scheme_practicals[i] = (split_scheme_practical - z_near) / (z_far - z_near);
    }

    // Calculate shadow split value
    for (int32_t i = 0; i < kPSSMSplitNum - 1; i++) {
        m_ShadowSplitValue[i] = z_far * split_scheme_practicals[i + 1];
    }

    // Split current frustum with practical split z value
    for (int32_t i = 0; i < kPSSMSplitNum; i++) {
        frustum.Split(split_scheme_practicals[i], split_scheme_practicals[i + 1], m_SplitFrustums[i]);
    }
}

void RenderImp::SplitShadowReceivers() {
    std::vector<scene::Object*> objs;
    objs.clear();
    scene::Scene::GetAllObjects(objs);

    // Gather shadow recievers
    std::vector<scene::Object*>::iterator it = objs.begin();
    for (; it != objs.end();) {
        scene::Object* obj = *it;
        if (!obj->GetModel()->IsAcceptShadow() || !obj->IsDrawEnable()) {
            it = objs.erase(it);
        } else {
            ++it;
        }
    }

    // Split recievers into splited frustum
    math::Matrix view = scene::Scene::GetCamera(scene::PRIMIAY_CAM)->GetViewMatrix();
    for (int32_t i = 0; i < kPSSMSplitNum; i++) {
        SplitObjIntoFrustum(objs, m_SplitFrustums[i], view);
    }
}

math::Matrix RenderImp::BuildRefLightSpaceMatrix() {
    math::Vector light_dir = -scene::Scene::GetLight(0).dir;  // For opengl, +z point out of the screen
    light_dir.w = 0;

    // Build light space
    math::Vector lit_space_right = math::Cross(light_dir, math::Vector(0.0f, 1.0f, 0.0f));
    lit_space_right.Normalize();
    math::Vector lit_space_up = math::Cross(light_dir, lit_space_right);
    lit_space_up.Normalize();
    math::Matrix light_space_to_world_space;
    light_space_to_world_space.MakeRotateMatrix(lit_space_right, lit_space_up, light_dir);

    return light_space_to_world_space;
}

void RenderImp::BuildBasicLightFrustums(math::Matrix light_space_to_world_space) {
    math::Matrix view_to_world = scene::Scene::GetCamera(scene::PRIMIAY_CAM)->GetViewMatrix();
    view_to_world.Inverse();
    math::Matrix world_to_light = light_space_to_world_space;
    world_to_light.Inverse();
    math::Matrix view_to_light;
    view_to_light.MakeIdentityMatrix();
    view_to_light.Mul(world_to_light);
    view_to_light.Mul(view_to_world);
    for (int32_t i = 0; i < kPSSMSplitNum; i++) {
        math::Vector points[8];
        m_SplitFrustums[i].GetPoints(points);

        for (int32_t j = 0; j < 8; j++) {
            points[j] = view_to_light * points[j];
        }

        math::AABB bv(points);
        bv.m_Max.z += kMaxSceneSize;  // Make frustum include all potential shadow caster objects

        points[Render::NLU] = math::Vector(bv.m_Min.x, bv.m_Max.y, bv.m_Max.z);  // NLU
        points[Render::NLD] = math::Vector(bv.m_Min.x, bv.m_Min.y, bv.m_Max.z);  // NLD
        points[Render::NRU] = math::Vector(bv.m_Max.x, bv.m_Max.y, bv.m_Max.z);  // NRU
        points[Render::NRD] = math::Vector(bv.m_Max.x, bv.m_Min.y, bv.m_Max.z);  // NRD
        points[Render::FLU] = math::Vector(bv.m_Min.x, bv.m_Max.y, bv.m_Min.z);  // FLU
        points[Render::FLD] = math::Vector(bv.m_Min.x, bv.m_Min.y, bv.m_Min.z);  // FLD
        points[Render::FRU] = math::Vector(bv.m_Max.x, bv.m_Max.y, bv.m_Min.z);  // FRU
        points[Render::FRD] = math::Vector(bv.m_Max.x, bv.m_Min.y, bv.m_Min.z);  // FRD

        m_LightSpaceFrustum[i].Build(points);

        //{  // Draw light space frustum
        //    if (i != 0) continue;
        //    math::Matrix view = scene::Scene::GetCamera(scene::PRIMIAY_CAM)->GetViewMatrix();
        //    math::Matrix view_to_world = view;
        //    view_to_world.Inverse();
        //    math::Matrix trans;
        //    trans.MakeIdentityMatrix();
        //    trans.Mul(view_to_world);
        //    trans.Mul(light_space_to_view_space);
        //    for (int32_t j = 0; j < 8; j++) {
        //        points[j] = trans * points[j];
        //    }
        //    AddLine(points[render::Render::NLU], points[render::Render::NRU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NRU], points[render::Render::NRD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NRD], points[render::Render::NLD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NLD], points[render::Render::NLU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::FLU], points[render::Render::FRU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::FRU], points[render::Render::FRD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::FRD], points[render::Render::FLD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::FLD], points[render::Render::FLU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NLD], points[render::Render::FLD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NLU], points[render::Render::FLU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NRU], points[render::Render::FRU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NRD], points[render::Render::FRD], math::Vector(0.0, 1.0, 0.0));
        //}
    }
}

void RenderImp::SplitShadowCasters(math::Matrix light_space_to_world_space) {
    std::vector<scene::Object*> objs;
    objs.clear();
    scene::Scene::GetAllObjects(objs);

    // Gather shadow casters
    std::vector<scene::Object*>::iterator it = objs.begin();
    for (; it != objs.end();) {
        scene::Object* obj = *it;
        if (!obj->GetModel()->IsCastShadow() || !obj->IsDrawEnable()) {
            it = objs.erase(it);
        } else {
            ++it;
        }
    }

    // Split casters into splited frustum
    math::Matrix world_to_light = light_space_to_world_space;
    world_to_light.Inverse();
    for (int32_t i = 0; i < kPSSMSplitNum; i++) {
        SplitObjIntoFrustum(objs, m_LightSpaceFrustum[i], world_to_light);
    }
}

void RenderImp::ShrinkAllLightFrustums(math::Matrix light_space_to_world_space) {
    // Restore original frustum size
    math::Vector points[8];
    for (int32_t i = 0; i < kPSSMSplitNum; i++) {
        m_LightSpaceFrustum[i].GetPoints(points);
        points[render::Render::NLU].z -= kMaxSceneSize;
        points[render::Render::NLD].z -= kMaxSceneSize;
        points[render::Render::NRU].z -= kMaxSceneSize;
        points[render::Render::NRD].z -= kMaxSceneSize;
        m_LightSpaceFrustum[i].Build(points);
    }

    // Shrink frustum
    math::Matrix world_space_to_light_space = light_space_to_world_space;
    world_space_to_light_space.Inverse();
    for (int32_t i = 0; i < kPSSMSplitNum; i++) {
        math::AABB casters_bv = CalcAllObjectsBoundBox(world_space_to_light_space, m_LightSpaceFrustum[i]);
        math::AABB receivers_bv = CalcAllObjectsBoundBox(world_space_to_light_space, m_SplitFrustums[i]);
        ShrinkLightFrustum(casters_bv, receivers_bv, m_LightSpaceFrustum[i]);

        //{  // Draw casters_bv
        //    if (i != 1) continue;
        //    math::Vector obj_min = casters_bv.m_Min;
        //    math::Vector obj_max = casters_bv.m_Max;
        //    points[Render::NLU] = math::Vector(obj_min.x, obj_max.y, obj_max.z);  // NLU
        //    points[Render::NLD] = math::Vector(obj_min.x, obj_min.y, obj_max.z);  // NLD
        //    points[Render::NRU] = math::Vector(obj_max.x, obj_max.y, obj_max.z);  // NRU
        //    points[Render::NRD] = math::Vector(obj_max.x, obj_min.y, obj_max.z);  // NRD
        //    points[Render::FLU] = math::Vector(obj_min.x, obj_max.y, obj_min.z);  // FLU
        //    points[Render::FLD] = math::Vector(obj_min.x, obj_min.y, obj_min.z);  // FLD
        //    points[Render::FRU] = math::Vector(obj_max.x, obj_max.y, obj_min.z);  // FRU
        //    points[Render::FRD] = math::Vector(obj_max.x, obj_min.y, obj_min.z);  // FRD
        //    math::Matrix view_to_world = view;
        //    view_to_world.Inverse();
        //    math::Matrix trans;
        //    trans.MakeIdentityMatrix();
        //    trans.Mul(view_to_world);
        //    trans.Mul(light_to_view);
        //    for (int32_t j = 0; j < 8; j++) {
        //        points[j] = trans * points[j];
        //    }
        //    AddLine(points[render::Render::NLU], points[render::Render::NRU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NRU], points[render::Render::NRD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NRD], points[render::Render::NLD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NLD], points[render::Render::NLU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::FLU], points[render::Render::FRU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::FRU], points[render::Render::FRD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::FRD], points[render::Render::FLD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::FLD], points[render::Render::FLU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NLD], points[render::Render::FLD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NLU], points[render::Render::FLU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NRU], points[render::Render::FRU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NRD], points[render::Render::FRD], math::Vector(0.0, 1.0, 0.0));
        //}

        //{  // Draw receivers_bv
        //    if (i != 0) continue;
        //    math::Vector obj_min = receivers_bv.m_Min;
        //    math::Vector obj_max = receivers_bv.m_Max;
        //    points[Render::NLU] = math::Vector(obj_min.x, obj_max.y, obj_max.z);  // NLU
        //    points[Render::NLD] = math::Vector(obj_min.x, obj_min.y, obj_max.z);  // NLD
        //    points[Render::NRU] = math::Vector(obj_max.x, obj_max.y, obj_max.z);  // NRU
        //    points[Render::NRD] = math::Vector(obj_max.x, obj_min.y, obj_max.z);  // NRD
        //    points[Render::FLU] = math::Vector(obj_min.x, obj_max.y, obj_min.z);  // FLU
        //    points[Render::FLD] = math::Vector(obj_min.x, obj_min.y, obj_min.z);  // FLD
        //    points[Render::FRU] = math::Vector(obj_max.x, obj_max.y, obj_min.z);  // FRU
        //    points[Render::FRD] = math::Vector(obj_max.x, obj_min.y, obj_min.z);  // FRD
        //    math::Matrix view_to_world = view;
        //    view_to_world.Inverse();
        //    math::Matrix trans;
        //    trans.MakeIdentityMatrix();
        //    trans.Mul(view_to_world);
        //    trans.Mul(light_to_view);
        //    for (int32_t j = 0; j < 8; j++) {
        //        points[j] = trans * points[j];
        //    }
        //    AddLine(points[render::Render::NLU], points[render::Render::NRU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NRU], points[render::Render::NRD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NRD], points[render::Render::NLD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NLD], points[render::Render::NLU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::FLU], points[render::Render::FRU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::FRU], points[render::Render::FRD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::FRD], points[render::Render::FLD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::FLD], points[render::Render::FLU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NLD], points[render::Render::FLD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NLU], points[render::Render::FLU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NRU], points[render::Render::FRU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NRD], points[render::Render::FRD], math::Vector(0.0, 1.0, 0.0));
        //}

        //{  // Draw light space frustum
        //    if (i != 0 && i != 1) continue;
        //    m_LightSpaceFrustum[i].GetPoints(points);
        //    math::Matrix view_to_world = view;
        //    view_to_world.Inverse();
        //    math::Matrix trans;
        //    trans.MakeIdentityMatrix();
        //    trans.Mul(view_to_world);
        //    trans.Mul(light_to_view);
        //    for (int32_t j = 0; j < 8; j++) {
        //        points[j] = trans * points[j];
        //    }
        //    AddLine(points[render::Render::NLU], points[render::Render::NRU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NRU], points[render::Render::NRD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NRD], points[render::Render::NLD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NLD], points[render::Render::NLU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::FLU], points[render::Render::FRU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::FRU], points[render::Render::FRD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::FRD], points[render::Render::FLD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::FLD], points[render::Render::FLU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NLD], points[render::Render::FLD], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NLU], points[render::Render::FLU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NRU], points[render::Render::FRU], math::Vector(0.0, 1.0, 0.0));
        //    AddLine(points[render::Render::NRD], points[render::Render::FRD], math::Vector(0.0, 1.0, 0.0));
        //}
    }
}

void RenderImp::BuildShadowMatrixs(math::Matrix light_space_to_world_space) {
    // Build light space to world space matrix
    for (int32_t i = 0; i < kPSSMSplitNum; i++) {
        math::AABB bv = m_LightSpaceFrustum[i].GetBoundBox();

        // Size
        float width = bv.m_Max.x - bv.m_Min.x;
        float height = bv.m_Max.y - bv.m_Min.y;
        float depth = bv.m_Max.z - bv.m_Min.z;

        // Camera position in world space
        math::Vector pos = (bv.m_Max + bv.m_Min) * 0.5f;
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
        m_ShadowMatrix[i].MakeIdentityMatrix();
        m_ShadowMatrix[i].Mul(proj);
        m_ShadowMatrix[i].Mul(view);
    }
}

void RenderImp::DrawShadowMapCore() {
    for (int32_t i = 0; i < kPSSMSplitNum; i++) {
        //if ( i != 0) continue;
        m_ShadowMapIndex = i;

        // Render Target
        render::Device::SetRenderTarget(m_ShadowRenderTarget[i]);

        // Viewport
        render::Device::SetViewport(0, 0, app::Application::GetShadowMapWidth(), app::Application::GetShadowMapHeight());

        // Draw Buffer
        render::Device::SetDrawColorBuffer(render::COLORBUF_NONE);

        // Clear
        render::Device::SetClearDepth(1.0f);
        render::Device::Clear(CLEAR_DEPTH);

        // Shader
        shader::Program* program = shader::Mgr::GetShader(m_ShadowShader);
        std::vector<uniform::UniformEntry>& uniforms = program->GetUniforms();
        render::Device::SetShader(program);
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

        std::vector<scene::Object*> objs;
        scene::Object* sky_obj = scene::Scene::GetSkyObject();
        if (sky_obj != NULL) {
            objs.push_back(sky_obj);
        }

        scene::Scene::GetAllObjects(objs);

        // Objects
        for (int32_t j = 0; j < static_cast<int32_t>(objs.size()); j++) {
            scene::Object* obj = objs[j];

            // Check if enable draw
            if (!obj->IsDrawEnable()) {
                continue;
            }

            // Check if cast shadow & enable depth
            if (obj->GetModel()->IsCastShadow() && obj->IsDepthTestEnable()) {
                // scene::Object Uniform
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
                VertexLayout layout = mesh::Mgr::GetMeshById(mesh_id)->GetVertexLayout();
                int32_t num = mesh::Mgr::GetMeshById(mesh_id)->GetVertexNum();
                render::Device::SetVertexBuffer(mesh::Mgr::GetMeshById(mesh_id)->GetVertexBuffer());
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

        // Reset
        render::Device::SetViewport(0, 0, static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height));
        render::Device::SetRenderTarget(NULL);
    }
}

math::AABB RenderImp::CalcAllObjectsBoundBox(math::Matrix trans, Frustum& frustum) {
    std::vector<scene::Object*> objs;
    objs.clear();
    frustum.GetAllObjects(objs);

    int32_t num = objs.size();
    math::AABB bv;

    for (int32_t i = 0; i < num; i++) {
        math::Vector obj_max = objs[i]->GetBoundBoxMax();
        math::Vector obj_min = objs[i]->GetBoundBoxMin();
        math::Vector points[8];
        points[Render::NLU] = math::Vector(obj_min.x, obj_max.y, obj_max.z);  // NLU
        points[Render::NLD] = math::Vector(obj_min.x, obj_min.y, obj_max.z);  // NLD
        points[Render::NRU] = math::Vector(obj_max.x, obj_max.y, obj_max.z);  // NRU
        points[Render::NRD] = math::Vector(obj_max.x, obj_min.y, obj_max.z);  // NRD
        points[Render::FLU] = math::Vector(obj_min.x, obj_max.y, obj_min.z);  // FLU
        points[Render::FLD] = math::Vector(obj_min.x, obj_min.y, obj_min.z);  // FLD
        points[Render::FRU] = math::Vector(obj_max.x, obj_max.y, obj_min.z);  // FRU
        points[Render::FRD] = math::Vector(obj_max.x, obj_min.y, obj_min.z);  // FRD

        // Transform into reference space
        for (int32_t j = 0; j < 8; j++) {
            points[j] = trans * points[j];
        }

        math::AABB obj_bv(points);
        if (obj_bv.m_Min.x < bv.m_Min.x) {
            bv.m_Min.x = obj_bv.m_Min.x;
        }
        if (obj_bv.m_Min.y < bv.m_Min.y) {
            bv.m_Min.y = obj_bv.m_Min.y;
        }
        if (obj_bv.m_Min.z < bv.m_Min.z) {
            bv.m_Min.z = obj_bv.m_Min.z;
        }
        if (obj_bv.m_Max.x > bv.m_Max.x) {
            bv.m_Max.x = obj_bv.m_Max.x;
        }
        if (obj_bv.m_Max.y > bv.m_Max.y) {
            bv.m_Max.y = obj_bv.m_Max.y;
        }
        if (obj_bv.m_Max.z > bv.m_Max.z) {
            bv.m_Max.z = obj_bv.m_Max.z;
        }
    }

    return bv;
}

void RenderImp::ShrinkLightFrustum(math::AABB casters, math::AABB receivers, Frustum& frustum) {
    math::AABB frustum_bv = frustum.GetBoundBox();

    frustum_bv.m_Min.x = std::max<float>(std::max<float>(receivers.m_Min.x, casters.m_Min.x), frustum_bv.m_Min.x);
    frustum_bv.m_Max.x = std::min<float>(std::min<float>(receivers.m_Max.x, casters.m_Max.x), frustum_bv.m_Max.x);

    frustum_bv.m_Min.y = std::max<float>(std::max<float>(receivers.m_Min.y, casters.m_Min.y), frustum_bv.m_Min.y);
    frustum_bv.m_Max.y = std::min<float>(std::min<float>(receivers.m_Max.y, casters.m_Max.y), frustum_bv.m_Max.y);

    frustum_bv.m_Min.z = std::max<float>(receivers.m_Min.z, frustum_bv.m_Min.z);
    frustum_bv.m_Max.z = std::max<float>(casters.m_Max.z, frustum_bv.m_Max.z);

    math::Vector points[8];
    points[Render::NLU] = math::Vector(frustum_bv.m_Min.x, frustum_bv.m_Max.y, frustum_bv.m_Max.z);  // NLU
    points[Render::NLD] = math::Vector(frustum_bv.m_Min.x, frustum_bv.m_Min.y, frustum_bv.m_Max.z);  // NLD
    points[Render::NRU] = math::Vector(frustum_bv.m_Max.x, frustum_bv.m_Max.y, frustum_bv.m_Max.z);  // NRU
    points[Render::NRD] = math::Vector(frustum_bv.m_Max.x, frustum_bv.m_Min.y, frustum_bv.m_Max.z);  // NRD
    points[Render::FLU] = math::Vector(frustum_bv.m_Min.x, frustum_bv.m_Max.y, frustum_bv.m_Min.z);  // FLU
    points[Render::FLD] = math::Vector(frustum_bv.m_Min.x, frustum_bv.m_Min.y, frustum_bv.m_Min.z);  // FLD
    points[Render::FRU] = math::Vector(frustum_bv.m_Max.x, frustum_bv.m_Max.y, frustum_bv.m_Min.z);  // FRU
    points[Render::FRD] = math::Vector(frustum_bv.m_Max.x, frustum_bv.m_Min.y, frustum_bv.m_Min.z);  // FRD
    frustum.Build(points);
}

void RenderImp::CalculateFrustumInView(Frustum& frustum) {
    math::Vector points[8];

    math::Vector v(0.0f, 0.0f, 0.0f);

    float fov_half = m_Perspective[Render::PRIMARY_PERS].fov / 2.0f;
    float tan_fov_half = tan(fov_half / 180.0f * 3.1415926f);
    float near_height_half = m_Perspective[Render::PRIMARY_PERS].znear * tan_fov_half;
    float far_height_half = m_Perspective[Render::PRIMARY_PERS].zfar * tan_fov_half;
    float near_width_half = near_height_half * m_Perspective[Render::PRIMARY_PERS].aspect;
    float far_width_half = far_height_half * m_Perspective[Render::PRIMARY_PERS].aspect;

    // For opengl, camera look at negative z-axis
    v.x = -near_width_half;
    v.y = near_height_half;
    v.z = -m_Perspective[Render::PRIMARY_PERS].znear;
    points[Render::NLU] = v;

    v.x = -near_width_half;
    v.y = -near_height_half;
    v.z = -m_Perspective[Render::PRIMARY_PERS].znear;
    points[Render::NLD] = v;

    v.x = near_width_half;
    v.y = near_height_half;
    v.z = -m_Perspective[Render::PRIMARY_PERS].znear;
    points[Render::NRU] = v;

    v.x = near_width_half;
    v.y = -near_height_half;
    v.z = -m_Perspective[Render::PRIMARY_PERS].znear;
    points[Render::NRD] = v;

    v.x = -far_width_half;
    v.y = far_height_half;
    v.z = -m_Perspective[Render::PRIMARY_PERS].zfar;
    points[Render::FLU] = v;

    v.x = -far_width_half;
    v.y = -far_height_half;
    v.z = -m_Perspective[Render::PRIMARY_PERS].zfar;
    points[Render::FLD] = v;

    v.x = far_width_half;
    v.y = far_height_half;
    v.z = -m_Perspective[Render::PRIMARY_PERS].zfar;
    points[Render::FRU] = v;

    v.x = far_width_half;
    v.y = -far_height_half;
    v.z = -m_Perspective[Render::PRIMARY_PERS].zfar;
    points[Render::FRD] = v;

    frustum.Build(points);
}

void RenderImp::SplitObjIntoFrustum(std::vector<scene::Object*>& objs, Frustum& frustum, math::Matrix trans) {
    math::AABB frustum_bv = frustum.GetBoundBox();
    frustum.RemoveAllObjects();

    int32_t objs_num = objs.size();
    for (int32_t i = 0; i < objs_num; i++) {
        math::Vector obj_max = objs[i]->GetBoundBoxMax();
        math::Vector obj_min = objs[i]->GetBoundBoxMin();
        math::Vector points[8];
        points[Render::NLU] = math::Vector(obj_min.x, obj_max.y, obj_max.z);  // NLU
        points[Render::NLD] = math::Vector(obj_min.x, obj_min.y, obj_max.z);  // NLD
        points[Render::NRU] = math::Vector(obj_max.x, obj_max.y, obj_max.z);  // NRU
        points[Render::NRD] = math::Vector(obj_max.x, obj_min.y, obj_max.z);  // NRD
        points[Render::FLU] = math::Vector(obj_min.x, obj_max.y, obj_min.z);  // FLU
        points[Render::FLD] = math::Vector(obj_min.x, obj_min.y, obj_min.z);  // FLD
        points[Render::FRU] = math::Vector(obj_max.x, obj_max.y, obj_min.z);  // FRU
        points[Render::FRD] = math::Vector(obj_max.x, obj_min.y, obj_min.z);  // FRD

        // Transform into reference space
        for (int32_t j = 0; j < 8; j++) {
            points[j] = trans * points[j];
        }

        math::AABB obj_bv(points);

        if (obj_bv.HasIntersect(frustum_bv)) {
            frustum.AddObject(objs[i]);
        }
    }
}

void RenderImp::PreDrawLightLoop() {
    std::vector<scene::Object*> objs;
    m_ShaderGroups.clear();

    // Add sky object
    scene::Object* obj = glb::scene::Scene::GetSkyObject();
    if (obj != NULL) {
        objs.push_back(obj);
    }

    // Add normal object
    glb::scene::Scene::GetAllObjects(objs);

    // Sort object by shader
    std::map<std::string, ShaderGroup> opaque_group;
    std::vector<scene::Object*> transparent_objs;
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
                shader::Program* program = shader::Mgr::GetShader(shader::Mgr::GetShader(desc));
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
                    scene::Object* temp = transparent_objs[j];
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
                shader::Descriptor desc = transparent_objs[i]->GetShaderDesc();
                ShaderGroup new_group(desc, shader::Mgr::GetShader(shader::Mgr::GetShader(desc)));
                new_group.AddObject(transparent_objs[i]);
                transparent_group.push_back(new_group);
            } else {
                shader::Descriptor desc = transparent_objs[i]->GetShaderDesc();
                if (transparent_group[transparent_group.size() - 1].GetShaderDesc().Equal(desc)) {
                    transparent_group[transparent_group.size() - 1].AddObject(transparent_objs[i]);
                } else {
                    shader::Descriptor desc = transparent_objs[i]->GetShaderDesc();
                    ShaderGroup new_group(desc, shader::Mgr::GetShader(shader::Mgr::GetShader(desc)));
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

void RenderImp::DrawLightLoopCore() {
    // Render Target
    render::Device::SetRenderTarget(m_HDRRenderTarget);

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

    // Clear
    render::Device::SetClearColor(1.0f, 1.0f, 1.0f);
    render::Device::SetClearDepth(1.0f);
    render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

    for (int32_t i = 0; i < static_cast<int32_t>(m_ShaderGroups.size()); i++) {
        std::vector<scene::Object*> objs = m_ShaderGroups[i].GetObjects();
        shader::Descriptor desc = m_ShaderGroups[i].GetShaderDesc();
        shader::Program* program = m_ShaderGroups[i].GetShaderProgram();
        std::vector<uniform::UniformEntry>& uniforms = program->GetUniforms();

        // Shader
        render::Device::SetShader(program);
        render::Device::SetShaderLayout(program->GetShaderLayout());

        // Common Texture
        int32_t tex_unit = 0;
        render::Device::SetTexture(render::TS_SHADOW0, texture::Mgr::GetTextureById(m_ShadowMap[0]), tex_unit++);
        render::Device::SetTexture(render::TS_SHADOW1, texture::Mgr::GetTextureById(m_ShadowMap[1]), tex_unit++);
        render::Device::SetTexture(render::TS_SHADOW2, texture::Mgr::GetTextureById(m_ShadowMap[2]), tex_unit++);
        render::Device::SetTexture(render::TS_SHADOW3, texture::Mgr::GetTextureById(m_ShadowMap[3]), tex_unit++);
        render::Device::SetTexture(render::TS_AO_MAP, texture::Mgr::GetTextureById(m_AOMap), tex_unit++);

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
            scene::Object* obj = objs[j];

            // Check if enable draw
            if (!obj->IsDrawEnable()) {
                continue;
            }

            // Textures
            if (obj->GetModel()->HasDiffuseTexture()) {
                render::Device::SetTexture(render::TS_DIFFUSE, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_DIFFUSE)), tex_unit++);
            }
            if (obj->GetModel()->HasAlphaTexture()) {
                render::Device::SetTexture(render::TS_ALPHA, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_ALPHA)), tex_unit++);
            }
            if (obj->GetModel()->HasNormalTexture()) {
                render::Device::SetTexture(render::TS_NORMAL, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_NORMAL)), tex_unit++);
            }
            if (obj->GetModel()->HasReflectTexture()) {
                render::Device::SetTexture(render::TS_REFLECT, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_REFLECT)), tex_unit++);
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
            VertexLayout layout = mesh::Mgr::GetMeshById(mesh_id)->GetVertexLayout();
            int32_t num = mesh::Mgr::GetMeshById(mesh_id)->GetVertexNum();
            render::Device::SetVertexBuffer(mesh::Mgr::GetMeshById(mesh_id)->GetVertexBuffer());
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

void RenderImp::DownsamplerHDRScene() {
    texture::Mgr::GetTextureById(m_HDRSceneTex)->GenerateMipmap();
}

void RenderImp::CalcLogLum() {
    // Change viewport to match render target
    render::Device::SetViewport(0, 0, m_Width / 4, m_Height / 4);

    // Render Target
    render::Device::SetRenderTarget(m_BloomRenderTarget);

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

    // Clear
    render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
    render::Device::SetClearDepth(1.0f);
    render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

    // Shader
    shader::Program* program = shader::Mgr::GetShader(m_LogLumShader);
    render::Device::SetShader(program);
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Set texture
    render::Device::ClearTexture();
    render::Device::SetTexture(render::TS_HDRSCENE, texture::Mgr::GetTextureById(m_HDRSceneTex), 0);

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
    VertexLayout layout = m_ScreenMesh->GetVertexLayout();
    int32_t num = m_ScreenMesh->GetVertexNum();
    render::Device::SetVertexBuffer(m_ScreenMesh->GetVertexBuffer());
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
    render::Device::SetViewport(0, 0, m_Width, m_Height);
}

void RenderImp::CalcAverageLum() {
    texture::Texture* tex = texture::Mgr::GetTextureById(m_LogLumTex);
    tex->GenerateMipmap();

    float pixel[4];
    memset(pixel, 0, sizeof(pixel));
    tex->GetTextureData(pixel, m_MaxMipmapLevel);

    float cur_scene_average_lum = exp(pixel[0]) - 0.0001f;
    if (m_LightAdaption > 0.0f) {
        m_AverageLum = m_PreAverageLum + m_LightAdaption;
        if (m_AverageLum >= cur_scene_average_lum) {
            m_AverageLum = cur_scene_average_lum;
        }
        m_PreAverageLum = m_AverageLum;
    } else {
        m_AverageLum = cur_scene_average_lum;
    }
}

void RenderImp::FilterBrightness() {
    // Change viewport to match render target
    render::Device::SetViewport(0, 0, m_Width / 4, m_Height / 4);

    // Render Target
    render::Device::SetRenderTarget(m_BloomRenderTarget);

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

    // Clear
    render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
    render::Device::SetClearDepth(1.0f);
    render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

    // Shader
    shader::Program* program = shader::Mgr::GetShader(m_FilterBrightnessShader);
    render::Device::SetShader(program);
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Set texture
    render::Device::ClearTexture();
    render::Device::SetTexture(render::TS_HDRSCENE, texture::Mgr::GetTextureById(m_HDRSceneTex), 0);

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
    VertexLayout layout = m_ScreenMesh->GetVertexLayout();
    int32_t num = m_ScreenMesh->GetVertexNum();
    render::Device::SetVertexBuffer(m_ScreenMesh->GetVertexBuffer());
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
    render::Device::SetViewport(0, 0, m_Width, m_Height);
}

void RenderImp::DownsamplerBrightness() {
    texture::Mgr::GetTextureById(m_HighLightSceneTex)->GenerateMipmap();
}

void RenderImp::BloomH() {
    // Change viewport to match render target
    render::Device::SetViewport(0, 0, m_Width / 4, m_Height / 4);

    // Render Target
    render::Device::SetRenderTarget(m_BloomRenderTarget);

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT1);

    // Clear
    render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
    render::Device::SetClearDepth(1.0f);
    render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

    // Shader
    shader::Program* program = shader::Mgr::GetShader(m_BloomHShader);
    render::Device::SetShader(program);
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Set texture
    render::Device::ClearTexture();
    render::Device::SetTexture(render::TS_LOG_LUM, texture::Mgr::GetTextureById(m_LogLumTex), 0);

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
    VertexLayout layout = m_ScreenMesh->GetVertexLayout();
    int32_t num = m_ScreenMesh->GetVertexNum();
    render::Device::SetVertexBuffer(m_ScreenMesh->GetVertexBuffer());
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
    render::Device::SetViewport(0, 0, m_Width, m_Height);
}

void RenderImp::BloomV() {
    // Change viewport to match render target
    render::Device::SetViewport(0, 0, m_Width / 4, m_Height / 4);

    // Render Target
    render::Device::SetRenderTarget(m_BloomRenderTarget);

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

    // Clear
    render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
    render::Device::SetClearDepth(1.0f);
    render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

    // Shader
    shader::Program* program = shader::Mgr::GetShader(m_BloomVShader);
    render::Device::SetShader(program);
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Set texture
    render::Device::ClearTexture();
    render::Device::SetTexture(render::TS_HDR_BLOOM, texture::Mgr::GetTextureById(m_BloomTex), 0);

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
    VertexLayout layout = m_ScreenMesh->GetVertexLayout();
    int32_t num = m_ScreenMesh->GetVertexNum();
    render::Device::SetVertexBuffer(m_ScreenMesh->GetVertexBuffer());
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
    render::Device::SetViewport(0, 0, m_Width, m_Height);
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
    render::Device::SetShader(program);
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Set texture
    render::Device::ClearTexture();
    render::Device::SetTexture(render::TS_LOG_LUM, texture::Mgr::GetTextureById(m_LogLumTex), 0);
    render::Device::SetTexture(render::TS_HDRSCENE, texture::Mgr::GetTextureById(m_HDRSceneTex), 1);

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
    VertexLayout layout = m_ScreenMesh->GetVertexLayout();
    int32_t num = m_ScreenMesh->GetVertexNum();
    render::Device::SetVertexBuffer(m_ScreenMesh->GetVertexBuffer());
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
    render::Device::SetRenderTarget(m_DepthTarget);

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_NONE);

    // Clear
    render::Device::SetClearDepth(1.0f);
    render::Device::Clear(CLEAR_DEPTH);

    // Shader
    shader::Program* program = shader::Mgr::GetShader(m_DepthShader);
    std::vector<uniform::UniformEntry>& uniforms = program->GetUniforms();
    render::Device::SetShader(program);
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

    std::vector<scene::Object*> objs;
    scene::Object* sky_obj = scene::Scene::GetSkyObject();
    if (sky_obj != NULL) {
        objs.push_back(sky_obj);
    }

    scene::Scene::GetAllObjects(objs);

    // scene::Objects
    for (int32_t j = 0; j < static_cast<int32_t>(objs.size()); j++) {
        scene::Object* obj = objs[j];

        // Check enable draw
        if (!obj->IsDrawEnable()) {
            continue;
        }

        // Check if cast shadow & enable depth
        if (obj->GetModel()->IsCastShadow() && obj->IsDepthTestEnable()) {
            // scene::Object Uniform
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
            VertexLayout layout = mesh::Mgr::GetMeshById(mesh_id)->GetVertexLayout();
            int32_t num = mesh::Mgr::GetMeshById(mesh_id)->GetVertexNum();
            render::Device::SetVertexBuffer(mesh::Mgr::GetMeshById(mesh_id)->GetVertexBuffer());
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

    // Reset render target
    render::Device::SetRenderTarget(0);
}

void RenderImp::GenRandRotateMap() {
    // Build rand seed
    float seed = 0.0f;
    math::Matrix proj = m_Perspective[Render::PRIMARY_PERS].m;
    math::Matrix view = scene::Scene::GetCamera(scene::PRIMIAY_CAM)->GetViewMatrix();
    for (int32_t i = 0; i < 16; i++) {
        seed += proj.GetData()[i];
        seed += view.GetData()[i];
    }
    srand(static_cast<unsigned int>(floor(seed * 1000)));

    // Build random rotate vector
    math::Vector rand_rotate_v[16];
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
    texture::Mgr::GetTextureById(m_RandRotateMap)->UpdateTextureData(pixels);
}

void RenderImp::DrawAO() {
    // Render Target
    render::Device::SetRenderTarget(m_AORenderTarget);

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

    // Clear
    render::Device::SetClearDepth(1.0f);
    render::Device::SetClearColor(1.0f, 1.0f, 1.0f);
    render::Device::Clear(CLEAR_DEPTH | CLEAR_COLOR);

    // Shader
    shader::Program* program = shader::Mgr::GetShader(m_AOShader);
    std::vector<uniform::UniformEntry>& uniforms = program->GetUniforms();
    render::Device::SetShader(program);
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Texture
    render::Device::SetTexture(render::TS_DEPTH, texture::Mgr::GetTextureById(m_DepthMap), 0);
    render::Device::SetTexture(render::TS_RANDOM_ROTATE, texture::Mgr::GetTextureById(m_RandRotateMap), 1);

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
    VertexLayout layout = m_ScreenMesh->GetVertexLayout();
    int32_t num = m_ScreenMesh->GetVertexNum();
    render::Device::SetVertexBuffer(m_ScreenMesh->GetVertexBuffer());
    render::Device::SetVertexLayout(layout);

    // Draw
    render::Device::Draw(render::PT_TRIANGLES, 0, num);

    // Reset render target
    render::Device::SetRenderTarget(0);
}

void RenderImp::BiBlurH() {
    // Render Target
    render::Device::SetRenderTarget(m_AORenderTarget);

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT1);

    // Clear
    render::Device::SetClearDepth(1.0f);
    render::Device::SetClearColor(1.0f, 1.0f, 1.0f);
    render::Device::Clear(CLEAR_DEPTH | CLEAR_COLOR);

    // Shader
    shader::Program* program = shader::Mgr::GetShader(m_BiBlurHShader);
    std::vector<uniform::UniformEntry>& uniforms = program->GetUniforms();
    render::Device::SetShader(program);
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Texture
    render::Device::SetTexture(render::TS_DEPTH, texture::Mgr::GetTextureById(m_DepthMap), 0);
    render::Device::SetTexture(render::TS_AO_MAP, texture::Mgr::GetTextureById(m_AOMap), 1);

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
    VertexLayout layout = m_ScreenMesh->GetVertexLayout();
    int32_t num = m_ScreenMesh->GetVertexNum();
    render::Device::SetVertexBuffer(m_ScreenMesh->GetVertexBuffer());
    render::Device::SetVertexLayout(layout);

    // Draw
    render::Device::Draw(render::PT_TRIANGLES, 0, num);

    // Reset render target
    render::Device::SetRenderTarget(0);
}

void RenderImp::BiBlurV() {
    // Render Target
    render::Device::SetRenderTarget(m_AORenderTarget);

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

    // Clear
    render::Device::SetClearDepth(1.0f);
    render::Device::SetClearColor(1.0f, 1.0f, 1.0f);
    render::Device::Clear(CLEAR_DEPTH | CLEAR_COLOR);

    // Shader
    shader::Program* program = shader::Mgr::GetShader(m_BiBlurVShader);
    std::vector<uniform::UniformEntry>& uniforms = program->GetUniforms();
    render::Device::SetShader(program);
    render::Device::SetShaderLayout(program->GetShaderLayout());

    // Texture
    render::Device::SetTexture(render::TS_DEPTH, texture::Mgr::GetTextureById(m_DepthMap), 0);
    render::Device::SetTexture(render::TS_BI_BLUR_MAP, texture::Mgr::GetTextureById(m_BiBlurMap), 1);

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
    VertexLayout layout = m_ScreenMesh->GetVertexLayout();
    int32_t num = m_ScreenMesh->GetVertexNum();
    render::Device::SetVertexBuffer(m_ScreenMesh->GetVertexBuffer());
    render::Device::SetVertexLayout(layout);

    // Draw
    render::Device::Draw(render::PT_TRIANGLES, 0, num);

    // Reset render target
    render::Device::SetRenderTarget(0);
}

void RenderImp::PreDrawEnvMap() {
    std::vector<scene::Object*> objs;
    m_ShaderGroups.clear();

    // Add sky object
    scene::Object* obj = glb::scene::Scene::GetSkyObject();
    if (obj != NULL) {
        objs.push_back(obj);
    }

    // Add normal object
    glb::scene::Scene::GetAllObjects(objs);

    // Sort object by shader
    std::map<std::string, ShaderGroup> opaque_group;
    std::vector<scene::Object*> transparent_objs;
    for (int32_t i = 0; i < static_cast<int32_t>(objs.size()); i++) {
        shader::Descriptor desc = objs[i]->GetShaderDesc();
        desc.SetFlag(shader::GLB_ENABLE_REFLECT_TEX, false);
        bool is_transparent = desc.GetFlag(shader::GLB_ENABLE_ALPHA_TEX);
        if (is_transparent) {
            transparent_objs.push_back(objs[i]);
        } else {
            std::map<std::string, ShaderGroup>::iterator it = opaque_group.find(desc.GetString());
            if (it != opaque_group.end()) {
                it->second.AddObject(objs[i]);
            } else {
                shader::Program* program = shader::Mgr::GetShader(shader::Mgr::GetShader(desc));
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
                    scene::Object* temp = transparent_objs[j];
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
                shader::Descriptor desc = transparent_objs[i]->GetShaderDesc();
                desc.SetFlag(shader::GLB_ENABLE_REFLECT_TEX, false);
                ShaderGroup new_group(desc, shader::Mgr::GetShader(shader::Mgr::GetShader(desc)));
                new_group.AddObject(transparent_objs[i]);
                transparent_group.push_back(new_group);
            } else {
                shader::Descriptor desc = transparent_objs[i]->GetShaderDesc();
                desc.SetFlag(shader::GLB_ENABLE_REFLECT_TEX, false);
                shader::Descriptor desc2 = transparent_group[transparent_group.size() - 1].GetShaderDesc();
                desc2.SetFlag(shader::GLB_ENABLE_REFLECT_TEX, false);
                if (desc2.Equal(desc)) {
                    transparent_group[transparent_group.size() - 1].AddObject(transparent_objs[i]);
                } else {
                    ShaderGroup new_group(desc, shader::Mgr::GetShader(shader::Mgr::GetShader(desc)));
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

void RenderImp::DrawEnvMapCore() {
    // Save original proj-view matrix information
    PerspectiveProj old_proj = m_Perspective[Render::PRIMARY_PERS];
    scene::CameraBase* old_camera = NULL;
    scene::Scene::GetCurCamera()->Clone(&old_camera);
    scene::Object* sky_obj = scene::Scene::GetSkyObject();
    math::Vector old_sky_object_pos(0.0f, 0.0f, 0.0f);
    if (sky_obj) {
        old_sky_object_pos = sky_obj->GetPos();
    }

    // New perspective matrix
    m_Perspective[Render::PRIMARY_PERS].znear = 0.1f;
    m_Perspective[Render::PRIMARY_PERS].zfar = 1000.0f;
    m_Perspective[Render::PRIMARY_PERS].aspect = 1.0f;
    m_Perspective[Render::PRIMARY_PERS].fov = 90.0f;
    m_Perspective[Render::PRIMARY_PERS].m.MakeProjectionMatrix(1.0f, 90.0f, 0.1f, 1000.0f);

    int32_t size = m_EnvBakers.size();
    for (int32_t i = 0; i < size; i++) {
        scene::Object* ref_obj = m_EnvBakers[i].obj;
        math::Vector ref_pos = ref_obj->GetPos();

        scene::Scene::GetSkyObject()->SetPos(ref_pos);
        scene::Scene::GetSkyObject()->Update();

        // 6 View matrix(+X,-X,+Y,-Y,+Z,-Z)
        math::Matrix views[6];
        views[0].MakeViewMatrix(ref_pos, ref_pos + math::Vector(1.0f, 0.0f, 0.0f));
        views[1].MakeViewMatrix(ref_pos, ref_pos + math::Vector(-1.0f, 0.0f, 0.0f));
        views[2].MakeViewMatrix(ref_pos, math::Vector(1.0f, 0.0f, 0.0f), math::Vector(0.0f, 0.0f, 1.0f), math::Vector(0.0f, -1.0f, 0.0f));
        views[3].MakeViewMatrix(ref_pos, math::Vector(1.0f, 0.0f, 0.0f), math::Vector(0.0f, 0.0f, -1.0f), math::Vector(0.0f, 1.0f, 0.0f));
        views[4].MakeViewMatrix(ref_pos, ref_pos + math::Vector(0.0f, 0.0f, 1.0f));
        views[5].MakeViewMatrix(ref_pos, ref_pos + math::Vector(0.0f, 0.0f, -1.0f));

        // Render Target
        render::Device::SetRenderTarget(m_EnvBakers[i].target);

        // View port
        render::Device::SetViewport(0, 0, m_EnvBakers[i].width, m_EnvBakers[i].height);

        for (int32_t j = 0; j < 6; j++) {
            scene::Scene::GetCurCamera()->SetViewMatrix(views[j]);

            // Draw Buffer
            render::Device::SetDrawColorBuffer(static_cast<render::DrawColorBuffer>(render::COLORBUF_COLOR_ATTACHMENT0 + j));

            // Clear
            render::Device::SetClearColor(1.0f, 1.0f, 1.0f);
            render::Device::SetClearDepth(1.0f);
            render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

            for (int32_t k = 0; k < static_cast<int32_t>(m_ShaderGroups.size()); k++) {
                std::vector<scene::Object*> objs = m_ShaderGroups[k].GetObjects();
                shader::Descriptor desc = m_ShaderGroups[k].GetShaderDesc();
                shader::Program* program = m_ShaderGroups[k].GetShaderProgram();
                std::vector<uniform::UniformEntry>& uniforms = program->GetUniforms();

                // Shader
                render::Device::SetShader(program);
                render::Device::SetShaderLayout(program->GetShaderLayout());

                // Common Texture
                int32_t tex_unit = 0;
                render::Device::SetTexture(render::TS_SHADOW0, texture::Mgr::GetTextureById(m_ShadowMap[0]), tex_unit++);
                render::Device::SetTexture(render::TS_SHADOW1, texture::Mgr::GetTextureById(m_ShadowMap[1]), tex_unit++);
                render::Device::SetTexture(render::TS_SHADOW2, texture::Mgr::GetTextureById(m_ShadowMap[2]), tex_unit++);
                render::Device::SetTexture(render::TS_SHADOW3, texture::Mgr::GetTextureById(m_ShadowMap[3]), tex_unit++);
                render::Device::SetTexture(render::TS_AO_MAP, texture::Mgr::GetTextureById(m_AOMap), tex_unit++);

                // Scene uniforms
                for (int32_t l = 0; l < static_cast<int32_t>(uniforms.size()); l++) {
                    uniform::UniformEntry entry = uniforms[l];
                    if (entry.flag) {
                        // TODO: for now, id is the index of the uniform picker table
                        uniform::Wrapper uniform_wrapper = uniform::kUniformPickers[entry.id].picker(NULL);
                        SetUniform(entry.location, uniform_wrapper);
                    }
                }

                // scene::Objects
                for (int32_t l = 0; l < static_cast<int32_t>(objs.size()); l++) {
                    scene::Object* obj = objs[l];
                    if (obj == ref_obj || !obj->IsDrawEnable()) {
                        continue;
                    }

                    // Textures
                    if (obj->GetModel()->HasDiffuseTexture()) {
                        render::Device::SetTexture(render::TS_DIFFUSE, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_DIFFUSE)), tex_unit++);
                    }
                    if (obj->GetModel()->HasAlphaTexture()) {
                        render::Device::SetTexture(render::TS_ALPHA, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_ALPHA)), tex_unit++);
                    }
                    if (obj->GetModel()->HasNormalTexture()) {
                        render::Device::SetTexture(render::TS_NORMAL, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_NORMAL)), tex_unit++);
                    }

                    // scene::Object Uniform
                    for (int32_t m = 0; m < static_cast<int32_t>(uniforms.size()); m++) {
                        uniform::UniformEntry entry = uniforms[m];
                        if (!entry.flag) {
                            // TODO: for now, id is the index of the uniform picker table
                            uniform::Wrapper uniform_wrapper = uniform::kUniformPickers[entry.id].picker(obj);
                            SetUniform(entry.location, uniform_wrapper);
                        }
                    }

                    // Vertex Buffer
                    int32_t mesh_id = obj->GetModel()->GetMeshId();
                    VertexLayout layout = mesh::Mgr::GetMeshById(mesh_id)->GetVertexLayout();
                    int32_t num = mesh::Mgr::GetMeshById(mesh_id)->GetVertexNum();
                    render::Device::SetVertexBuffer(mesh::Mgr::GetMeshById(mesh_id)->GetVertexBuffer());
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
        }

        // Reset render target
        render::Device::SetRenderTarget(0);
    }

    // Restore original proj-view matrix information
    m_Perspective[Render::PRIMARY_PERS] = old_proj;
    scene::Scene::GetCurCamera()->Restore(&old_camera);
    if (sky_obj) {
        sky_obj->SetPos(old_sky_object_pos);
        sky_obj->Update();
    }
    render::Device::SetViewport(0, 0, m_Width, m_Height);
}

void RenderImp::SetUniform(int32_t location, uniform::Wrapper& wrapper) {
    switch (wrapper.GetFormat()) {
    case uniform::Wrapper::FMT_INT:
        {
            int32_t value = wrapper.GetInt();
            render::Device::SetUniform1i(location, value);
        }
        break;

    case uniform::Wrapper::FMT_SAMPLER2D:
        {
            int32_t sampler = wrapper.GetSampler2D();
            render::Device::SetUniformSampler2D(location, sampler);
        }
        break;

    case uniform::Wrapper::FMT_SAMPLERCUBE:
        {
            int32_t sampler = wrapper.GetSamplerCube();
            render::Device::SetUniformSamplerCube(location, sampler);
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
            math::Vector values = wrapper.GetVector();
            render::Device::SetUniform3f(location, values);
        }
        break;

    case uniform::Wrapper::FMT_MATRIX:
        {
            math::Matrix m = wrapper.GetMatrix();
            render::Device::SetUniformMatrix(location, m);
        }
        break;
    }
}

float RenderImp::ZValueFromCamera(scene::Object* obj) {
    float result = 0.0f;

    scene::CameraBase* cam = scene::Scene::GetCamera(scene::PRIMIAY_CAM);
    if (obj != NULL && cam != NULL) {
        math::Vector cam_pos = cam->GetPos();
        math::Vector cam_target = cam->GetTarget();
        math::Vector obj_pos = obj->GetPos();
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

void Render::InitializeAfterUserApp() {
    if (s_RenderImp != NULL) {
        s_RenderImp->InitializeAfterUserApp();
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

math::Matrix Render::GetPerspective(int32_t type) {
    math::Matrix proj;

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

math::Vector Render::GetFrustumPointInView(int32_t index) {
    math::Vector v;

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

int32_t Render::GetScreenWidth() {
    int32_t result = 0;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetScreenWidth();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Render::GetScreenHeight() {
    int32_t result = 0;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetScreenHeight();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

math::Matrix Render::GetShadowMapMatrix(int32_t index) {
    math::Matrix mat;

    if (s_RenderImp != NULL) {
        mat = s_RenderImp->GetShadowMapMatrix(index);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return mat;
}

int32_t Render::GetCurShadowMapIndex() {
    int32_t result = -1;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetCurShadowMapIndex();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

float Render::GetShadowSplitValue(int32_t index) {
    float result = 0.0f;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetShadowSplitValue(index);
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

void Render::SetHighLightBase(float base) {
    if (s_RenderImp != NULL) {
        s_RenderImp->SetHighLightBase(base);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

float Render::GetHighLightBase() {
    float result = 0.0f;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetHighLightBase();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

int32_t Render::RequestBakeEnvMap(int32_t width, int32_t height, scene::Object* obj) {
    int32_t result = -1;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->RequestBakeEnvMap(width, height, obj);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

void Render::CancleBakeEnvMap(scene::Object* obj) {
    if (s_RenderImp != NULL) {
        s_RenderImp->CancleBakeEnvMap(obj);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Render::AddLine(math::Vector start, math::Vector end, math::Vector color) {
    if (s_RenderImp != NULL) {
        s_RenderImp->AddLine(start, end, color);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void Render::AddBoundBox(math::AABB bv, math::Vector color) {
    if (s_RenderImp != NULL) {
        s_RenderImp->AddBoundBox(bv, color);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}
};  // namespace render

};  // namespace glb