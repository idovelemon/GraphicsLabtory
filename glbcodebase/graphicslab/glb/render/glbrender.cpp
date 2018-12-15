//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 10 / 28
// Brief: Draw the entire scene
//------------------------------------------------------------------
#include "glbrender.h"

#include <map>

#include "glbapplication.h"
#include "glbdebugmenu.h"
#include "glbfont.h"
#include "glbmaterial.h"
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
static const int32_t kMaxDebugMenuTriangleNum = 128;

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

//-----------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// ShaderGroup DECLARATION
//----------------------------------------------------------------------------------
class ShaderGroup {
public:
    ShaderGroup(shader::Program* shader_program);
    virtual ~ShaderGroup();

public:
    void AddObject(scene::Object* obj);
    std::vector<scene::Object*>& GetObjects();
    shader::Program* GetShaderProgram();

private:
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

    int32_t GetBRDFMap();

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

    math::Matrix GetDecalViewMatrix();
    math::Matrix GetDecalProjMatrix();

    void SetHighLightBase(float base);
    float GetHighLightBase();
    void SetBloomWeights(float w0, float w1, float w2, float w3);

    void AddLine(math::Vector start, math::Vector end, math::Vector color);
    void AddMenuMesh(math::Vector lt, math::Vector rb, math::Vector color);
    void AddBoundBox(math::AABB bv, math::Vector color);
    void AddText(const char* text, math::Vector pos, math::Vector color, float scale);

protected:
    void PreDraw();
    void DrawShadowMap();
    void DrawDecalMap();
    void DrawAOMap();
    void DrawLightLoop();
    void DrawDebugLine();
    void DrawDebugMenu();
    void DrawFont();
    void DrawHDR();
    void AfterDraw();

    void PrepareFont();
    void PrepareShadowMap();
    void PrepareDecalMap();
    void PrepareDebugLine();
    void PrepareDebugMenu();
    void PrepareAOMap();
    void PrepareHDR();
    void PrepareEnvMap();

    void SetupInternalMaterialParameters();

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
    void DrawShadowMapNormal();
    void DrawShadowMapInstance();

    // Decal
    void PreDrawDecalMap();
    void BuildDecalViewProjectionMatrix();
    void DrawDecalMapCore();

    // Light Loop
    void PreDrawLightLoop();
    void DrawLightLoopCore();

    // HDR
    void DownsampleTex();
    void FilterBrightness();
    void BloomTex();
    void BloomHTex();
    void BloomVTex();
    void Tonemapping();

    void SetUniform(int32_t location, uniform::Wrapper& wrapper);
    inline float ZValueFromCamera(scene::Object* obj);

private:
    int32_t                                 m_Width;
    int32_t                                 m_Height;
    std::vector<ShaderGroup>                m_ShaderGroups;

    // Image based Lighting
    int32_t                                 m_BRDFPFTMap;

    // Perspective
    int32_t                                 m_PerspectiveType;
    PerspectiveProj                         m_Perspective[Render::MAX_PERS];

    // Shadow
    RenderTarget*                           m_ShadowRenderTarget[kPSSMSplitNum];
    int32_t                                 m_ShadowMap[kPSSMSplitNum];
    int32_t                                 m_ShadowShader;
    int32_t                                 m_InstanceShadowShader;
    Frustum                                 m_SplitFrustums[kPSSMSplitNum];
    Frustum                                 m_LightSpaceFrustum[kPSSMSplitNum];
    math::Matrix                            m_ShadowMatrix[kPSSMSplitNum];
    int32_t                                 m_ShadowMapIndex;
    float                                   m_ShadowSplitValue[kPSSMSplitNum - 1];

    // Decal
    RenderTarget*                           m_DecalRenderTarget;
    int32_t                                 m_DecalMap;
    math::Matrix                            m_DecalViewMatrix;
    math::Matrix                            m_DecalProjMatrix;

    // HDR
    float                                   m_HightLightBase;
    RenderTarget*                           m_HDRRenderTarget;
    int32_t                                 m_DepthMap;
    int32_t                                 m_HDRTex;
    RenderTarget*                           m_FilterBrightnessRT;
    int32_t                                 m_FilterBrightnessTex;
    RenderTarget*                           m_DownsamplerRenderTarget[4];
    int32_t                                 m_DownsamplerTex[4];
    shader::UserProgram*                    m_FilterBrightnessShader;
    int32_t                                 m_FBTexLoc;
    int32_t                                 m_FBHighBaseLoc;
    shader::UserProgram*                    m_DownsamplerShader;
    int32_t                                 m_DSTexLoc;
    RenderTarget*                           m_BlurRenderTarget[4];
    int32_t                                 m_BlurTex[4];
    shader::UserProgram*                    m_BlurHShader;
    shader::UserProgram*                    m_BlurVShader;
    int32_t                                 m_BlurHTexLoc;
    int32_t                                 m_BlurVTexLoc;
    int32_t                                 m_BlurWidthLoc;
    int32_t                                 m_BlurHeightLoc;
    shader::UserProgram*                    m_TonemapShader;
    int32_t                                 m_TonemapTexLoc;
    int32_t                                 m_Bloom0TexLoc;
    int32_t                                 m_Bloom1TexLoc;
    int32_t                                 m_Bloom2TexLoc;
    int32_t                                 m_Bloom3TexLoc;
    int32_t                                 m_Bloom0WeightLoc;
    int32_t                                 m_Bloom1WeightLoc;
    int32_t                                 m_Bloom2WeightLoc;
    int32_t                                 m_Bloom3WeightLoc;
    float                                   m_Bloom0Weight;
    float                                   m_Bloom1Weight;
    float                                   m_Bloom2Weight;
    float                                   m_Bloom3Weight;

    // Debug
    shader::UserProgram*                    m_DebugLineShader;
    shader::UserProgram*                    m_DebugMenuShader;
    shader::UserProgram*                    m_FontShader;

    mesh::ScreenMesh*                       m_ScreenMesh;
    mesh::DebugMesh*                        m_DebugMesh;
    mesh::DynamicTriangleMesh*              m_DebugMenuMesh;
    mesh::FontMesh*                         m_FontMesh;
};

//-----------------------------------------------------------------------------------
// CLASS DEFINITION
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
// ShaderGroup DEFINITION
//-----------------------------------------------------------------------------------
ShaderGroup::ShaderGroup(shader::Program* shaderProgram)
: m_ShaderProgram(shaderProgram) {
    GLB_SAFE_ASSERT(shaderProgram != nullptr);
}

ShaderGroup::~ShaderGroup() {
    m_ShaderProgram = NULL;
    m_Objects.clear();
}

void ShaderGroup::AddObject(scene::Object* obj) {
    m_Objects.push_back(obj);
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

// Image based Lighting
, m_BRDFPFTMap(-1)

// Shadow
, m_ShadowShader(-1)
, m_InstanceShadowShader(-1)
, m_ShadowMapIndex(-1)

// Decal
, m_DecalRenderTarget(NULL)
, m_DecalMap(-1)

// HDR
, m_HightLightBase(1.0f)
, m_HDRRenderTarget(NULL)
, m_HDRTex(-1)
, m_DepthMap(-1)
, m_FilterBrightnessRT(NULL)
, m_FilterBrightnessTex(-1)
, m_FilterBrightnessShader(NULL)
, m_FBTexLoc(-1)
, m_FBHighBaseLoc(-1)
, m_DownsamplerShader(NULL)
, m_DSTexLoc(-1)
, m_BlurHShader(NULL)
, m_BlurVShader(NULL)
, m_BlurHTexLoc(-1)
, m_BlurWidthLoc(-1)
, m_BlurHeightLoc(-1)
, m_TonemapShader(NULL)
, m_TonemapTexLoc(-1)
, m_Bloom0TexLoc(-1)
, m_Bloom1TexLoc(-1)
, m_Bloom2TexLoc(-1)
, m_Bloom3TexLoc(-1)
, m_Bloom0WeightLoc(-1)
, m_Bloom1WeightLoc(-1)
, m_Bloom2WeightLoc(-1)
, m_Bloom3WeightLoc(-1)
, m_Bloom0Weight(0.05f)
, m_Bloom1Weight(0.05f)
, m_Bloom2Weight(0.05f)
, m_Bloom3Weight(0.01f)

// Debug
, m_DebugLineShader(NULL)
, m_DebugMenuShader(nullptr)
, m_FontShader(NULL)

// Env

, m_DebugMesh(NULL)
, m_DebugMenuMesh(nullptr)
, m_ScreenMesh(NULL)
, m_FontMesh(NULL) {
    memset(m_Perspective, 0, sizeof(m_Perspective));
    m_ShaderGroups.clear();
    memset(m_ShadowRenderTarget, 0, sizeof(m_ShadowRenderTarget));
    memset(m_ShadowMap, 0, sizeof(m_ShadowMap));
    memset(m_SplitFrustums, 0, sizeof(m_SplitFrustums));
    memset(m_LightSpaceFrustum, 0, sizeof(m_LightSpaceFrustum));
    memset(m_ShadowMatrix, 0, sizeof(m_ShadowMatrix));
    memset(m_ShadowSplitValue, 0, sizeof(m_ShadowSplitValue));
    memset(m_DownsamplerRenderTarget, 0, sizeof(m_DownsamplerRenderTarget));
    memset(m_DownsamplerTex, 0, sizeof(m_DownsamplerTex));
    memset(m_BlurRenderTarget, 0, sizeof(m_BlurRenderTarget));
    memset(m_BlurTex, 0, sizeof(m_BlurTex));
}

RenderImp::~RenderImp() {
    Destroy();
}

void RenderImp::Initialize(int32_t width, int32_t height) {
    m_ShaderGroups.clear();

    m_Width = width;
    m_Height = height;

    PrepareFont();
    PrepareShadowMap();
    PrepareDecalMap();
    PrepareDebugLine();
    PrepareDebugMenu();
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

    // Decal
    GLB_SAFE_DELETE(m_DecalRenderTarget);
    m_DecalMap = -1;

    // Depth
    m_DepthMap = -1;

    // HDR
    GLB_SAFE_DELETE(m_FilterBrightnessRT);
    GLB_SAFE_DELETE(m_FilterBrightnessShader);
    m_FilterBrightnessTex = -1;
    for (int32_t i = 0; i < 4; i++) {
        GLB_SAFE_DELETE(m_DownsamplerRenderTarget[i]);
        m_DownsamplerTex[i] = -1;
    }
    GLB_SAFE_DELETE(m_DownsamplerShader);
    for (int32_t i = 0; i < 4; i++) {
        GLB_SAFE_DELETE(m_BlurRenderTarget[i]);
        m_BlurTex[i] = -1;
    }
    GLB_SAFE_DELETE(m_BlurHShader);
    GLB_SAFE_DELETE(m_BlurVShader);

    // Debug
    GLB_SAFE_DELETE(m_DebugLineShader);
    GLB_SAFE_DELETE(m_DebugMenuShader);
    GLB_SAFE_DELETE(m_FontShader);

    GLB_SAFE_DELETE(m_DebugMesh);
    GLB_SAFE_DELETE(m_DebugMenuMesh);
    GLB_SAFE_DELETE(m_ScreenMesh);
    GLB_SAFE_DELETE(m_FontMesh);
}

void RenderImp::Draw() {
    PreDraw();
    DrawShadowMap();
    DrawDecalMap();
    //DrawDepthMap();
    DrawLightLoop();
    DrawHDR();
    DrawDebugLine();
    DrawDebugMenu();
    DrawFont();
    AfterDraw();

#ifdef GLB_PLATFORM_OPENGL
    GLB_CHECK_GL_ERROR;
#endif
}

int32_t RenderImp::GetBRDFMap() {
    return m_BRDFPFTMap;
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

math::Matrix RenderImp::GetDecalViewMatrix() {
    return m_DecalViewMatrix;
}

math::Matrix RenderImp::GetDecalProjMatrix() {
    return m_DecalProjMatrix;
}

void RenderImp::SetHighLightBase(float base) {
    m_HightLightBase = base;
}

float RenderImp::GetHighLightBase() {
    return m_HightLightBase;
}

void RenderImp::SetBloomWeights(float w0, float w1, float w2, float w3) {
    m_Bloom0Weight = w0;
    m_Bloom1Weight = w1;
    m_Bloom2Weight = w2;
    m_Bloom3Weight = w3;
}

void RenderImp::AddLine(math::Vector start, math::Vector end, math::Vector color) {
    if (m_DebugMesh != NULL) {
        m_DebugMesh->AddLine(start, end, color);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

void RenderImp::AddMenuMesh(math::Vector lt, math::Vector rb, math::Vector color) {
    if (m_DebugMenuMesh != nullptr) {
        m_DebugMenuMesh->AddRect(lt, rb, color);
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

void RenderImp::AddText(const char* text, math::Vector pos, math::Vector color, float scale) {
    math::Vector preCharNDCSize(0.0f, 0.0f, 0.0f);
    math::Vector preCharPos = pos;

    for (int32_t i = 0; i < strlen(text); i++) {
        float ltU = 0, ltV = 0, rbU = 0, rbV = 0;
        float ndcSizeX = 0.0f, ndcSizeY = 0.0f;
        font::Mgr::GetCharacter(text[i], ltU, ltV, rbU, rbV, ndcSizeX, ndcSizeY);

        math::Vector newPos = preCharPos + math::Vector(preCharNDCSize.x, 0.0f, 0.0f);
        if (text[i] != ' ') {
            m_FontMesh->AddChar(math::Vector(ltU, ltV, 0.0f), math::Vector(rbU, rbV, 0.0f), newPos, math::Vector(ndcSizeX, ndcSizeY, 0.0f) * scale, color);
        }

        preCharNDCSize = math::Vector(ndcSizeX, ndcSizeY, 0.0f) * scale;
        preCharPos = newPos;
    }
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
    SetupInternalMaterialParameters();
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

void RenderImp::DrawDecalMap() {
    PreDrawDecalMap();
    BuildDecalViewProjectionMatrix();
    DrawDecalMapCore();
    texture::Mgr::GetTextureById(m_DecalMap)->GenerateMipmap();
}

void RenderImp::DrawLightLoop() {
    PreDrawLightLoop();
    DrawLightLoopCore();
}

void RenderImp::DrawDebugLine() {
    // Render Target
    render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_BACK);

    // Clear
    render::Device::SetClearDepth(1.0f);
    render::Device::Clear(render::CLEAR_DEPTH);

    // Shader
    render::Device::SetShader(m_DebugLineShader);
    render::Device::SetShaderLayout(m_DebugLineShader->GetShaderLayout());

    // Texture
    render::Device::ClearTexture();
    render::Device::SetTexture(0, texture::Mgr::GetTextureById(m_DepthMap), 0);

    // Uniforms
    math::Matrix proj = render::Render::GetPerspective(render::Render::GetCurPerspectiveType());
    scene::CameraBase* cam = scene::Scene::GetCamera(scene::Scene::GetCurCameraType());
    math::Matrix view = cam->GetViewMatrix();

    render::Device::SetUniformMatrix(m_DebugLineShader->GetUniformLocation("glb_unif_WVP"), proj * view);
    render::Device::SetUniformSampler2D(m_DebugLineShader->GetUniformLocation("glb_unif_DepthMap"), 0);

    // Vertex Buffer
    VertexLayout layout = m_DebugMesh->GetVertexLayout();
    int32_t num = m_DebugMesh->GetVertexNum();
    render::Device::SetVertexBuffer(m_DebugMesh->GetVertexBuffer());
    render::Device::SetVertexLayout(layout);

    // Draw
    render::Device::Draw(render::PT_LINES, 0, num);

    // Reset debug mesh
    m_DebugMesh->ClearAllLines();

    // Reset render target
    render::Device::SetRenderTarget(0);
}

void RenderImp::DrawDebugMenu() {
    // Setup Render State
    render::Device::SetCullFaceEnable(false);
    render::Device::SetDepthTestEnable(false);
    render::Device::SetAlphaBlendEnable(true);
    render::Device::SetAlphaBlendFunc(render::FACTOR_SRC, render::FUNC_SRC_ALPHA);
    render::Device::SetAlphaBlendFunc(render::FACTOR_DST, render::FUNC_ONE_MINUS_SRC_ALPHA);

    // Setup Shader
    render::Device::SetShader(m_DebugMenuShader);
    render::Device::SetShaderLayout(m_DebugMenuShader->GetShaderLayout());

    // Setup Vertex
    render::Device::SetVertexBuffer(m_DebugMenuMesh->GetVertexBuffer());
    render::Device::SetVertexLayout(m_DebugMenuMesh->GetVertexLayout());

    // Draw
    render::Device::Draw(render::PT_TRIANGLES, 0, m_DebugMenuMesh->GetVertexNum());

    // Reset debug menu
    m_DebugMenuMesh->Clear();
}

void RenderImp::DrawFont() {
    // Render Target
    render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_BACK);

    // Clear
    render::Device::SetCullFaceEnable(false);
    render::Device::SetDepthTestEnable(true);
    render::Device::SetAlphaBlendEnable(true);
    render::Device::SetAlphaBlendFunc(render::FACTOR_SRC, render::FUNC_SRC_ALPHA);
    render::Device::SetAlphaBlendFunc(render::FACTOR_DST, render::FUNC_ONE_MINUS_SRC_ALPHA);

    // Setup Shader
    render::Device::SetShader(m_FontShader);
    render::Device::SetShaderLayout(m_FontShader->GetShaderLayout());

    // Setup Vertex
    render::Device::SetVertexBuffer(m_FontMesh->GetVertexBuffer());
    render::Device::SetVertexLayout(m_FontMesh->GetVertexLayout());

    // Setup Texture
    render::Device::SetTexture(0, render::texture::Mgr::GetTextureById(font::Mgr::GetFontTexture()), 0);

    // Setup Uniform
    render::Device::SetUniformSampler2D(m_FontShader->GetUniformLocation("glb_FontMap"), 0);

    // Draw
    render::Device::Draw(render::PT_TRIANGLES, 0, m_FontMesh->GetVertexNum());

    // Reset debug mesh
    m_FontMesh->ClearAllChars();

    // Reset render target
    render::Device::SetRenderTarget(0);

    // Reset alpha blend
    render::Device::SetAlphaBlendEnable(false);
}

void RenderImp::DrawHDR() {
    FilterBrightness();
    DownsampleTex();
    BloomTex();
    Tonemapping();
}

void RenderImp::AfterDraw() {
    render::Device::SwapBuffer();
}

void RenderImp::PrepareFont() {
    m_FontShader = render::shader::UserProgram::Create("..\\glb\\shader\\font.vs", "..\\glb\\shader\\font.fs");
    m_FontMesh = render::mesh::FontMesh::Create();
}

void RenderImp::PrepareShadowMap() {
    int32_t shadow_map_width = app::Application::GetShadowMapWidth();
    int32_t shadow_map_height = app::Application::GetShadowMapHeight();

    for (int32_t i = 0; i < kPSSMSplitNum; i++) {
        // Create shadow render target
        m_ShadowRenderTarget[i] = RenderTarget::Create(shadow_map_width, shadow_map_height);
        GLB_SAFE_ASSERT(m_ShadowRenderTarget[i] != NULL);

        // Create shadow map
        texture::Texture* shadow_map = texture::Texture::CreateFloat32DepthTexture(shadow_map_width, shadow_map_height, false);
        if (shadow_map != NULL) {
            m_ShadowMap[i] = texture::Mgr::AddTexture(shadow_map);
        } else {
            GLB_SAFE_ASSERT(false);
        }

        if (m_ShadowRenderTarget[i] != NULL) {
            m_ShadowRenderTarget[i]->AttachDepthTexture(texture::Mgr::GetTextureById(m_ShadowMap[i]));
        }
    }

    // Create shadow shader TODO: Use UserShader
    m_ShadowShader = shader::Mgr::AddUberShader("..\\glb\\shader\\shadow.vs", "..\\glb\\shader\\shadow.fs");
    m_InstanceShadowShader = shader::Mgr::AddUberShader("..\\glb\\shader\\instanceShadow.vs", "..\\glb\\shader\\shadow.fs");
}

void RenderImp::PrepareDecalMap() {
    int32_t decalMapWidth = app::Application::GetDecalMapWidth();
    int32_t decalMapHeight = app::Application::GetDecalMapHeight();

    // Create decal render target
    m_DecalRenderTarget = RenderTarget::Create(decalMapWidth, decalMapHeight);
    GLB_SAFE_ASSERT(m_DecalRenderTarget != NULL);

    // Create decal map
    texture::Texture* decalMap = texture::Texture::CreateFloat32Texture(decalMapWidth, decalMapHeight, true);
    if (decalMap != NULL) {
        m_DecalMap = texture::Mgr::AddTexture(decalMap);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    // Bind texture
    if (m_DecalRenderTarget != NULL) {
        m_DecalRenderTarget->AttachColorTexture(COLORBUF_COLOR_ATTACHMENT0, decalMap);
    }
}

void RenderImp::PrepareDebugLine() {
    // Create depth map
    texture::Texture* depth_map = texture::Texture::CreateFloat32DepthTexture(static_cast<int32_t>(m_Width), static_cast<int32_t>(m_Height), false);
    if (depth_map != NULL) {
        m_DepthMap = texture::Mgr::AddTexture(depth_map);
    } else {
        GLB_SAFE_ASSERT(false);
    }

    // Create Shader
    m_DebugLineShader = render::shader::UserProgram::Create("..\\glb\\shader\\debugline.vs", "..\\glb\\shader\\debugline.fs");
}

void RenderImp::PrepareDebugMenu() {
    // Create Shader
    m_DebugMenuShader = render::shader::UserProgram::Create("..\\glb\\shader\\menu.vs", "..\\glb\\shader\\menu.fs");

    // Create Mesh
    m_DebugMenuMesh = render::mesh::DynamicTriangleMesh::Create(kMaxDebugMenuTriangleNum);
}

void RenderImp::PrepareHDR() {
    // Create Render Target
    m_HDRRenderTarget = RenderTarget::Create(m_Width, m_Height);
    m_HDRTex = texture::Mgr::AddTexture(texture::Texture::CreateFloat32Texture(m_Width, m_Height, false));
    m_HDRRenderTarget->AttachColorTexture(render::COLORBUF_COLOR_ATTACHMENT0, texture::Mgr::GetTextureById(m_HDRTex));
    m_HDRRenderTarget->AttachDepthTexture(texture::Mgr::GetTextureById(m_DepthMap));

    m_FilterBrightnessRT = RenderTarget::Create(m_Width, m_Height);
    m_FilterBrightnessTex = texture::Mgr::AddTexture(texture::Texture::CreateFloat32Texture(m_Width, m_Height, false));
    m_FilterBrightnessRT->AttachColorTexture(render::COLORBUF_COLOR_ATTACHMENT0, texture::Mgr::GetTextureById(m_FilterBrightnessTex));

    for (int32_t i = 0; i < 4; i++) {
        m_DownsamplerRenderTarget[i] = RenderTarget::Create(static_cast<int32_t>(m_Width / pow(2, i + 1)), static_cast<int32_t>(m_Height / pow(2, i + 1)));
        m_DownsamplerTex[i] = texture::Mgr::AddTexture(texture::Texture::CreateFloat32Texture(static_cast<int32_t>(m_Width / pow(2, i + 1)), static_cast<int32_t>(m_Height / pow(2, i + 1)), false));
        m_DownsamplerRenderTarget[i]->AttachColorTexture(render::COLORBUF_COLOR_ATTACHMENT0, texture::Mgr::GetTextureById(m_DownsamplerTex[i]));
    }

    for (int32_t i = 0; i < 4; i++) {
        m_BlurRenderTarget[i] = RenderTarget::Create(static_cast<int32_t>(m_Width / pow(2, i + 1)), static_cast<int32_t>(m_Height / pow(2, i + 1)));
        m_BlurTex[i] = texture::Mgr::AddTexture(texture::Texture::CreateFloat32Texture(static_cast<int32_t>(m_Width / pow(2, i + 1)), static_cast<int32_t>(m_Height / pow(2, i + 1)), false));
        m_BlurRenderTarget[i]->AttachColorTexture(render::COLORBUF_COLOR_ATTACHMENT0, texture::Mgr::GetTextureById(m_BlurTex[i]));
    }

    // Create Shader
    m_FilterBrightnessShader = shader::UserProgram::Create("..\\glb\\shader\\brightfilter.vs", "..\\glb\\shader\\brightfilter.fs");
    m_FBTexLoc = m_FilterBrightnessShader->GetUniformLocation("glb_unif_HDRSceneTex");
    m_FBHighBaseLoc = m_FilterBrightnessShader->GetUniformLocation("glb_unif_HighLightBase");

    m_DownsamplerShader = shader::UserProgram::Create("..\\glb\\shader\\downsample.vs", "..\\glb\\shader\\downsample.fs");
    m_DSTexLoc = m_DownsamplerShader->GetUniformLocation("glb_unif_Tex");

    m_BlurHShader = shader::UserProgram::Create("..\\glb\\shader\\bloom.vs", "..\\glb\\shader\\bloomh.fs");
    m_BlurVShader = shader::UserProgram::Create("..\\glb\\shader\\bloom.vs", "..\\glb\\shader\\bloomv.fs");
    m_BlurHTexLoc = m_BlurHShader->GetUniformLocation("glb_unif_BloomTex");
    m_BlurVTexLoc = m_BlurVShader->GetUniformLocation("glb_unif_BloomTex");
    m_BlurWidthLoc = m_BlurHShader->GetUniformLocation("glb_unif_BloomTexWidth");
    m_BlurHeightLoc = m_BlurVShader->GetUniformLocation("glb_unif_BloomTexHeight");

    m_TonemapShader = shader::UserProgram::Create("..\\glb\\shader\\tonemap.vs", "..\\glb\\shader\\tonemap.fs");
    m_TonemapTexLoc = m_TonemapShader->GetUniformLocation("glb_unif_Tex");
    m_Bloom0TexLoc = m_TonemapShader->GetUniformLocation("glb_unif_BloomTex[0]");
    m_Bloom1TexLoc = m_TonemapShader->GetUniformLocation("glb_unif_BloomTex[1]");
    m_Bloom2TexLoc = m_TonemapShader->GetUniformLocation("glb_unif_BloomTex[2]");
    m_Bloom3TexLoc = m_TonemapShader->GetUniformLocation("glb_unif_BloomTex[3]");
    m_Bloom0WeightLoc = m_TonemapShader->GetUniformLocation("glb_unif_BloomWeight[0]");
    m_Bloom1WeightLoc = m_TonemapShader->GetUniformLocation("glb_unif_BloomWeight[1]");
    m_Bloom2WeightLoc = m_TonemapShader->GetUniformLocation("glb_unif_BloomWeight[2]");
    m_Bloom3WeightLoc = m_TonemapShader->GetUniformLocation("glb_unif_BloomWeight[3]");

    // Create screen mesh
    m_ScreenMesh = mesh::ScreenMesh::Create();
}

void RenderImp::PrepareEnvMap() {
    m_BRDFPFTMap = texture::Mgr::LoadPFTTexture("..\\glb\\resource\\texture\\dfg.pft");
}

void RenderImp::SetupInternalMaterialParameters() {
    std::vector<scene::Object*> objs;
    scene::Scene::GetAllObjects(objs);

    // Loop every objects
    for (auto obj : objs) {
        if (obj) {
            if (obj->GetObjectType() == scene::Object::OBJECT_TYPE_NORMAL) {
                material::MaterialGroup group = obj->GetMaterialGroup();

                // Loop every pass material
                std::vector<material::MaterialGroup::Entry> allPassMaterials = group.GetAllPassMaterial();
                for (auto& pass : allPassMaterials) {
                    material::Material* mat = material::Mgr::GetMaterial(pass.materialID);

                    // Loop every internal parameters
                    std::vector<material::Material::ParameterEntry>& parameters = mat->GetAllParameters();
                    for (auto& entry : parameters) {
                        if (entry.type == material::Material::PARAMETER_TYPE_INTERNAL) {
                            // Find match picker
                            for (auto& pick : uniform::kEngineUniforms) {
                                if (!strcmp(entry.name, pick.name)) {
                                    uniform::Wrapper data = uniform::kUniformPickers[pick.id].picker(obj);

                                    // Fill data
                                    if (entry.format == PARAMETER_FORMAT_FLOAT) {
                                        entry.floatValue = data.GetFloat();
                                    } else if (entry.format == PARAMETER_FORMAT_FLOAT3) {
                                        entry.vecValue = data.GetVector();
                                    } else if (entry.format == PARAMETER_FORMAT_FLOAT4) {
                                        entry.vecValue = data.GetVector();
                                    } else if (entry.format == PARAMETER_FORMAT_INT) {
                                        entry.intValue = data.GetInt();
                                    } else if (entry.format == PARAMETER_FORMAT_MATRIX) {
                                        entry.matValue = data.GetMatrix();
                                    } else if (entry.format == PARAMETER_FORMAT_TEXTURE_2D) {
                                        entry.intValue = data.GetSampler2D();  // TODO:
                                    } else if (entry.format == PARAMETER_FORMAT_TEXTURE_3D) {
                                        entry.intValue = data.GetSampler3D();  // TODO:
                                    } else if (entry.format == PARAMETER_FORMAT_TEXTURE_CUBE) {
                                        entry.intValue = data.GetSamplerCube();  // TODO:
                                    } else {
                                        GLB_SAFE_ASSERT(false);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
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
    math::Vector light_dir(0.0f, 0.0f, 0.0f);
    for (int32_t i = 0; i < scene::kMaxLight; i++) {
        if (scene::Scene::GetLight(i).type == scene::PARALLEL_LIGHT) {
            // Assume only one parallel light exsit
            light_dir = -scene::Scene::GetLight(i).dir;  // For opengl, +z point out of the screen
            break;
        }
    }
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
        //    //if (i != 0) continue;
        //    math::Matrix view = scene::Scene::GetCamera(scene::PRIMIAY_CAM)->GetViewMatrix();
        //    math::Matrix view_to_world = view;
        //    view_to_world.Inverse();
        //    math::Matrix trans;
        //    trans.MakeIdentityMatrix();
        //    trans = light_space_to_world_space;
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
        if (!obj->GetModel()->IsCastShadow() || !obj->IsDrawEnable() || obj->GetObjectType() == scene::Object::OBJECT_TYPE_INSTANCE_RENDER) {
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
        //    math::Matrix trans = light_space_to_world_space;
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
        //    math::Matrix trans = light_space_to_world_space;
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
        //    if (i != 0) continue;
        //    m_LightSpaceFrustum[i].GetPoints(points);
        //    math::Matrix trans;
        //    trans.MakeIdentityMatrix();
        //    trans = light_space_to_world_space;
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

        DrawShadowMapNormal();

        DrawShadowMapInstance();

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
    bv.m_Max = -math::Vector(FLT_MAX, FLT_MAX, FLT_MAX);
    bv.m_Min = math::Vector(FLT_MAX, FLT_MAX, FLT_MAX);

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

void RenderImp::DrawShadowMapNormal() {
    // Shader
    shader::UberProgram* program = static_cast<shader::UberProgram*>(shader::Mgr::GetShader(m_ShadowShader));
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
        if (!obj->IsDrawEnable() || obj->GetObjectType() != scene::Object::OBJECT_TYPE_NORMAL) {
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
}

void RenderImp::DrawShadowMapInstance() {
    // Shader
    shader::UberProgram* program = static_cast<shader::UberProgram*>(shader::Mgr::GetShader(m_InstanceShadowShader));
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
        if (!obj->IsDrawEnable() || obj->GetObjectType() != scene::Object::OBJECT_TYPE_INSTANCE_RENDER) {
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
            int32_t instance = reinterpret_cast<scene::InstanceRenderObject*>(obj)->GetCurInstanceNum();
            render::Device::DrawInstance(render::PT_TRIANGLES, 0, num, instance);
        }
    }
}

void RenderImp::PreDrawDecalMap() {
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
    std::map<int32_t, ShaderGroup> opaqueGroup;
    for (int32_t i = 0; i < static_cast<int32_t>(objs.size()); i++) {
        if (objs[i]->GetObjectType() != scene::Object::OBJECT_TYPE_DECAL) continue;

        material::MaterialGroup group = objs[i]->GetMaterialGroup();

        // Only deal with light loop material
        int32_t materialID = group.GetPassMaterial(kDecalPassName);
        if (materialID != -1) {
            material::Material* material = material::Mgr::GetMaterial(materialID);
            if (material) {
                int32_t shaderID = material->GetShaderID();
                std::map<int32_t, ShaderGroup>::iterator it = opaqueGroup.find(shaderID);
                if (it != opaqueGroup.end()) {
                    it->second.AddObject(objs[i]);
                } else {
                    ShaderGroup newGroup(shader::Mgr::GetShader(shaderID));
                    newGroup.AddObject(objs[i]);
                    opaqueGroup.insert(std::pair<int32_t, ShaderGroup>(shaderID, newGroup));
                }
            } else {
                GLB_SAFE_ASSERT(false);
            }
        }
    }

    // Copy shader group
    for (std::map<int32_t, ShaderGroup>::iterator it = opaqueGroup.begin(); it != opaqueGroup.end(); ++it) {
        m_ShaderGroups.push_back(it->second);
    }
}

void RenderImp::BuildDecalViewProjectionMatrix() {
    math::Vector maxPos(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    math::Vector minPos(FLT_MAX, FLT_MAX, FLT_MAX);

    for (ShaderGroup& shaderGroup : m_ShaderGroups) {
        for (scene::Object* object : shaderGroup.GetObjects()) {
            if (object) {
                math::Vector boundMax = object->GetBoundBoxMax();
                math::Vector boundMin = object->GetBoundBoxMin();

                if (maxPos.x < boundMax.x) maxPos.x = boundMax.x;
                if (maxPos.y < boundMax.y) maxPos.y = boundMax.y;
                if (maxPos.z < boundMax.z) maxPos.z = boundMax.z;
                if (minPos.x > boundMin.x) minPos.x = boundMin.x;
                if (minPos.y > boundMin.y) minPos.y = boundMin.y;
                if (minPos.z > boundMin.z) minPos.z = boundMin.z;
            }
        }
    }

    float width = maxPos.x - minPos.x;
    float height = maxPos.z - minPos.z;
    width = height = max(width, height);
    float depth = 10.0f + maxPos.y - minPos.y;

    math::Vector cameraPos = (maxPos + minPos) * 0.5f;
    math::Vector lookAt = math::Vector(0.0f, 1.0f, 0.01f);
    lookAt.w = 0.0f;
    math::Vector target = cameraPos + lookAt;

    m_DecalViewMatrix.MakeViewMatrix(cameraPos, target);
    m_DecalProjMatrix.MakeOrthogonalMatrix(-width / 2.0f, width / 2.0f, - height / 2.0f, height / 2.0f, - depth / 2.0f, depth / 2.0f);
}

void RenderImp::DrawDecalMapCore() {
    // Render Target
    render::Device::SetRenderTarget(m_DecalRenderTarget);

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

    // Viewport
    render::Device::SetViewport(0, 0, app::Application::GetDecalMapWidth(), app::Application::GetDecalMapHeight());

    // Clear
    render::Device::SetClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    render::Device::SetClearDepth(1.0f);
    render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

    for (int32_t i = 0; i < static_cast<int32_t>(m_ShaderGroups.size()); i++) {
        std::vector<scene::Object*> objs = m_ShaderGroups[i].GetObjects();
        shader::UberProgram* program = static_cast<shader::UberProgram*>(m_ShaderGroups[i].GetShaderProgram());
        std::vector<uniform::UniformEntry>& uniforms = program->GetUniforms();

        // Shader
        render::Device::SetShader(program);
        render::Device::SetShaderLayout(program->GetShaderLayout());

        // Common Texture
        int32_t texUnit = 0;
        render::Device::SetTexture(render::TS_SHADOW0, texture::Mgr::GetTextureById(m_ShadowMap[0]), texUnit++);
        render::Device::SetTexture(render::TS_SHADOW1, texture::Mgr::GetTextureById(m_ShadowMap[1]), texUnit++);
        render::Device::SetTexture(render::TS_SHADOW2, texture::Mgr::GetTextureById(m_ShadowMap[2]), texUnit++);
        render::Device::SetTexture(render::TS_SHADOW3, texture::Mgr::GetTextureById(m_ShadowMap[3]), texUnit++);
        render::Device::SetTexture(render::TS_BRDF_PFT, texture::Mgr::GetTextureById(m_BRDFPFTMap), texUnit++);

        // Scene uniforms
        for (int32_t j = 0; j < static_cast<int32_t>(uniforms.size()); j++) {
            uniform::UniformEntry entry = uniforms[j];
            if (entry.flag) {
                // TODO: for now, id is the index of the uniform picker table
                if (entry.id == uniform::GLB_VIEWM) {
                    uniform::Wrapper uniformWrapper;
                    uniformWrapper.SetFormat(uniform::Wrapper::FMT_MATRIX);
                    uniformWrapper.SetMatrix(m_DecalViewMatrix);
                    SetUniform(entry.location, uniformWrapper);
                } else if (entry.id == uniform::GLB_PROJM) {
                    uniform::Wrapper uniformWrapper;
                    uniformWrapper.SetFormat(uniform::Wrapper::FMT_MATRIX);
                    uniformWrapper.SetMatrix(m_DecalProjMatrix);
                    SetUniform(entry.location, uniformWrapper);
                } else {
                    uniform::Wrapper uniform_wrapper = uniform::kUniformPickers[entry.id].picker(NULL);
                    SetUniform(entry.location, uniform_wrapper);
                }
            }
        }

        // Objects
        int32_t objectTexUnitStart = texUnit;
        for (int32_t j = 0; j < static_cast<int32_t>(objs.size()); j++) {
            scene::Object* obj = objs[j];

            // Check if enable draw
            if (!obj->IsDrawEnable() || obj->GetObjectType() != scene::Object::OBJECT_TYPE_DECAL) {
                continue;
            }

            // Reset object texture unit index
            texUnit = objectTexUnitStart;

            // Textures
            if (obj->GetModel()->HasAlbedoTexture()) {
                render::Device::SetTexture(render::TS_ALBEDO, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_ALBEDO)), texUnit++);
            }
            if (obj->GetModel()->HasRoughnessTexture()) {
                render::Device::SetTexture(render::TS_ROUGHNESS, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_ROUGHNESS)), texUnit++);
            }
            if (obj->GetModel()->HasMettalicTexture()) {
                render::Device::SetTexture(render::TS_METALLIC, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_METALLIC)), texUnit++);
            }
            if (obj->GetModel()->HasAlphaTexture()) {
                render::Device::SetTexture(render::TS_ALPHA, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_ALPHA)), texUnit++);
            }
            if (obj->GetModel()->HasNormalTexture()) {
                render::Device::SetTexture(render::TS_NORMAL, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_NORMAL)), texUnit++);
            }
            if (obj->GetModel()->HasEmissionTexture()) {
                render::Device::SetTexture(render::TS_EMISSION, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_EMISSION)), texUnit++);
            }
            if (obj->GetModel()->HasReflectTexture()) {
                render::Device::SetTexture(render::TS_REFLECT, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_REFLECT)), texUnit++);
            }
            if (obj->GetModel()->HasDiffusePFCTexture()) {
                render::Device::SetTexture(render::TS_DIFFUSE_PFC, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_DIFFUSE_PFC)), texUnit++);
            }
            if (obj->GetModel()->HasSpecularPFCTexture()) {
                render::Device::SetTexture(render::TS_SPECULAR_PFC, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_SPECULAR_PFC)), texUnit++);
            }
            if (obj->GetModel()->HasLightTexture()) {
                render::Device::SetTexture(render::TS_LIGHT0, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_LIGHT0)), texUnit++);
                render::Device::SetTexture(render::TS_LIGHT1, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_LIGHT1)), texUnit++);
                render::Device::SetTexture(render::TS_LIGHT2, texture::Mgr::GetTextureById(obj->GetModel()->GetTexId(scene::Model::MT_LIGHT2)), texUnit++);
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

    // Viewport
    render::Device::SetViewport(0, 0, app::Application::GetWindowWidth(), app::Application::GetWindowHeight());

    // Reset render target
    render::Device::SetRenderTarget(0);
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
    std::map<int32_t, ShaderGroup> opaqueGroup;
    for (int32_t i = 0; i < static_cast<int32_t>(objs.size()); i++) {
        if (objs[i]->GetObjectType() == scene::Object::OBJECT_TYPE_INSTANCE) continue;
        if (objs[i]->GetObjectType() == scene::Object::OBJECT_TYPE_DECAL) continue;

        material::MaterialGroup group = objs[i]->GetMaterialGroup();

        // Only deal with light loop material
        int32_t materialID = group.GetPassMaterial(kLightLoopPassName);
        if (materialID != -1) {
            material::Material* material = material::Mgr::GetMaterial(materialID);
            if (material) {
                int32_t shaderID = material->GetShaderID();
                std::map<int32_t, ShaderGroup>::iterator it = opaqueGroup.find(shaderID);
                if (it != opaqueGroup.end()) {
                    it->second.AddObject(objs[i]);
                } else {
                    ShaderGroup newGroup(shader::Mgr::GetShader(shaderID));
                    newGroup.AddObject(objs[i]);
                    opaqueGroup.insert(std::pair<int32_t, ShaderGroup>(shaderID, newGroup));
                }
            } else {
                GLB_SAFE_ASSERT(false);
            }
        }
    }

    // Copy shader group
    for (std::map<int32_t, ShaderGroup>::iterator it = opaqueGroup.begin(); it != opaqueGroup.end(); ++it) {
        m_ShaderGroups.push_back(it->second);
    }
}

void RenderImp::DrawLightLoopCore() {
    // Render Target
    render::Device::SetRenderTarget(m_HDRRenderTarget);

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

    // Clear
    render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
    render::Device::SetClearDepth(1.0f);
    render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

    for (int32_t i = 0; i < static_cast<int32_t>(m_ShaderGroups.size()); i++) {
        std::vector<scene::Object*> objs = m_ShaderGroups[i].GetObjects();
        shader::UberProgram* program = static_cast<shader::UberProgram*>(m_ShaderGroups[i].GetShaderProgram());

        // Shader
        render::Device::SetShader(program);
        render::Device::SetShaderLayout(program->GetShaderLayout());

        // Objects
        for (int32_t j = 0; j < static_cast<int32_t>(objs.size()); j++) {
            scene::Object* obj = objs[j];

            // Check if enable draw
            if (!obj->IsDrawEnable() || obj->GetObjectType() == scene::Object::OBJECT_TYPE_INSTANCE
                || obj->GetObjectType() == scene::Object::OBJECT_TYPE_DECAL) {
                continue;
            }

            // Material Parameters
            material::Material* material = material::Mgr::GetMaterial(obj->GetMaterialGroup().GetPassMaterial(kLightLoopPassName));
            int32_t texUnit = 0;
            for (auto& param : material->GetAllParameters()) {
                if (param.format == PARAMETER_FORMAT_FLOAT) {
                    render::Device::SetUniform1f(param.name, param.floatValue);
                } else if (param.format == PARAMETER_FORMAT_FLOAT3) {
                    render::Device::SetUniform3f(param.name, param.vecValue);
                } else if (param.format == PARAMETER_FORMAT_FLOAT4) {
                    render::Device::SetUniform4f(param.name, param.vecValue);
                } else if (param.format == PARAMETER_FORMAT_INT) {
                    render::Device::SetUniform1i(param.name, param.intValue);
                } else if (param.format == PARAMETER_FORMAT_MATRIX) {
                    render::Device::SetUniformMatrix(param.name, param.matValue);
                } else if (param.format == PARAMETER_FORMAT_TEXTURE_2D) {
                    render::Device::SetUniformSampler2D(param.name, texture::Mgr::GetTextureById(param.intValue), texUnit++);
                } else if (param.format == PARAMETER_FORMAT_TEXTURE_3D) {
                    render::Device::SetUniformSampler3D(param.name, texture::Mgr::GetTextureById(param.intValue), texUnit++);
                } else if (param.format == PARAMETER_FORMAT_TEXTURE_CUBE) {
                    render::Device::SetUniformSamplerCube(param.name, texture::Mgr::GetTextureById(param.intValue), texUnit++);
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
            if (obj->GetObjectType() == scene::Object::OBJECT_TYPE_NORMAL) {
                render::Device::Draw(render::PT_TRIANGLES, 0, num);
            } else if (obj->GetObjectType() == scene::Object::OBJECT_TYPE_INSTANCE_RENDER) {
                int32_t instanceNum = reinterpret_cast<scene::InstanceRenderObject*>(obj)->GetCurInstanceNum();
                render::Device::DrawInstance(render::PT_TRIANGLES, 0, num, instanceNum);
            }
        }
    }

    // Reset render target
    render::Device::SetRenderTarget(0);
}

void RenderImp::FilterBrightness() {
    // Render Target
    render::Device::SetRenderTarget(m_FilterBrightnessRT);

    // Viewport
    render::Device::SetViewport(0, 0, m_FilterBrightnessRT->GetWidth(), m_FilterBrightnessRT->GetHeight());

    // Draw Buffer
    render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

    // Clear
    render::Device::SetClearDepth(1.0f);
    render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
    render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

    // Shader
    render::Device::SetShader(m_FilterBrightnessShader);
    render::Device::SetShaderLayout(m_FilterBrightnessShader->GetShaderLayout());

    // Vertex
    render::Device::SetVertexBuffer(m_ScreenMesh->GetVertexBuffer());
    render::Device::SetVertexLayout(m_ScreenMesh->GetVertexLayout());

    // Texture
    render::Device::ClearTexture();
    render::Device::SetTexture(0, texture::Mgr::GetTextureById(m_HDRTex), 0);

    // Uniform
    render::Device::SetUniformSampler2D(m_FBTexLoc, 0);
    render::Device::SetUniform1f(m_FBHighBaseLoc, m_HightLightBase);

    // Draw
    render::Device::Draw(render::PT_TRIANGLES, 0, m_ScreenMesh->GetVertexNum());

    render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());
    render::Device::SetViewport(0, 0, m_Width, m_Height);
}

void RenderImp::DownsampleTex() {
    for (int32_t i = 0; i < 4; i++) {
        // Render Target
        render::Device::SetRenderTarget(m_DownsamplerRenderTarget[i]);

        // Viewport
        render::Device::SetViewport(0, 0, m_DownsamplerRenderTarget[i]->GetWidth(), m_DownsamplerRenderTarget[i]->GetHeight());

        // Draw Buffer
        render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

        // Clear
        render::Device::SetClearDepth(1.0f);
        render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
        render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

        // Shader
        render::Device::SetShader(m_DownsamplerShader);
        render::Device::SetShaderLayout(m_DownsamplerShader->GetShaderLayout());

        // Vertex
        render::Device::SetVertexBuffer(m_ScreenMesh->GetVertexBuffer());
        render::Device::SetVertexLayout(m_ScreenMesh->GetVertexLayout());

        // Texture
        render::Device::ClearTexture();
        if (i == 0) {
            render::Device::SetTexture(0, texture::Mgr::GetTextureById(m_FilterBrightnessTex), 0);
        } else {
            render::Device::SetTexture(0, texture::Mgr::GetTextureById(m_DownsamplerTex[i - 1]), 0);
        }

        // Uniform
        render::Device::SetUniformSampler2D(m_DSTexLoc, 0);

        // Draw
        render::Device::Draw(render::PT_TRIANGLES, 0, m_ScreenMesh->GetVertexNum());
    }

    render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());
    render::Device::SetViewport(0, 0, m_Width, m_Height);
}

void RenderImp::BloomTex() {
    BloomVTex();
    BloomHTex();
}

void RenderImp::BloomVTex() {
    for (int32_t i = 0; i < 4; i++) {
        // Render Target
        render::Device::SetRenderTarget(m_BlurRenderTarget[i]);

        // Viewport
        render::Device::SetViewport(0, 0, m_BlurRenderTarget[i]->GetWidth(), m_BlurRenderTarget[i]->GetHeight());

        // Draw Buffer
        render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

        // Clear
        render::Device::SetClearDepth(1.0f);
        render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
        render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

        // Shader
        render::Device::SetShader(m_BlurVShader);
        render::Device::SetShaderLayout(m_BlurVShader->GetShaderLayout());

        // Vertex
        render::Device::SetVertexBuffer(m_ScreenMesh->GetVertexBuffer());
        render::Device::SetVertexLayout(m_ScreenMesh->GetVertexLayout());

        // Texture
        render::Device::ClearTexture();
        render::Device::SetTexture(0, texture::Mgr::GetTextureById(m_DownsamplerTex[i]), 0);

        // Uniform
        render::Device::SetUniformSampler2D(m_BlurVTexLoc, 0);
        render::Device::SetUniform1f(m_BlurHeightLoc, m_BlurRenderTarget[i]->GetHeight());

        // Draw
        render::Device::Draw(render::PT_TRIANGLES, 0, m_ScreenMesh->GetVertexNum());
    }

    render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());
    render::Device::SetViewport(0, 0, m_Width, m_Height);
}

void RenderImp::BloomHTex() {
    for (int32_t i = 0; i < 4; i++) {
        // Render Target
        render::Device::SetRenderTarget(m_DownsamplerRenderTarget[i]);

        // Viewport
        render::Device::SetViewport(0, 0, m_DownsamplerRenderTarget[i]->GetWidth(), m_DownsamplerRenderTarget[i]->GetHeight());

        // Draw Buffer
        render::Device::SetDrawColorBuffer(render::COLORBUF_COLOR_ATTACHMENT0);

        // Clear
        render::Device::SetClearDepth(1.0f);
        render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
        render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

        // Shader
        render::Device::SetShader(m_BlurHShader);
        render::Device::SetShaderLayout(m_BlurHShader->GetShaderLayout());

        // Vertex
        render::Device::SetVertexBuffer(m_ScreenMesh->GetVertexBuffer());
        render::Device::SetVertexLayout(m_ScreenMesh->GetVertexLayout());

        // Texture
        render::Device::ClearTexture();
        render::Device::SetTexture(0, texture::Mgr::GetTextureById(m_BlurTex[i]), 0);

        // Uniform
        render::Device::SetUniformSampler2D(m_BlurHTexLoc, 0);
        render::Device::SetUniform1f(m_BlurWidthLoc, m_DownsamplerRenderTarget[i]->GetWidth());

        // Draw
        render::Device::Draw(render::PT_TRIANGLES, 0, m_ScreenMesh->GetVertexNum());
    }

    render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());
    render::Device::SetViewport(0, 0, m_Width, m_Height);
}

void RenderImp::Tonemapping() {
    // Render Target
    render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());

    // Clear
    render::Device::SetClearDepth(1.0f);
    render::Device::SetClearColor(0.0f, 0.0f, 0.0f);
    render::Device::Clear(CLEAR_COLOR | CLEAR_DEPTH);

    // Shader
    render::Device::SetShader(m_TonemapShader);
    render::Device::SetShaderLayout(m_TonemapShader->GetShaderLayout());

    // Vertex
    render::Device::SetVertexBuffer(m_ScreenMesh->GetVertexBuffer());
    render::Device::SetVertexLayout(m_ScreenMesh->GetVertexLayout());

    // Texture
    render::Device::ClearTexture();
    render::Device::SetTexture(0, texture::Mgr::GetTextureById(m_HDRTex), 0);
    render::Device::SetTexture(1, texture::Mgr::GetTextureById(m_DownsamplerTex[0]), 1);
    render::Device::SetTexture(2, texture::Mgr::GetTextureById(m_DownsamplerTex[1]), 2);
    render::Device::SetTexture(3, texture::Mgr::GetTextureById(m_DownsamplerTex[2]), 3);
    render::Device::SetTexture(4, texture::Mgr::GetTextureById(m_DownsamplerTex[3]), 4);

    // Uniform
    render::Device::SetUniformSampler2D(m_TonemapTexLoc, 0);
    render::Device::SetUniformSampler2D(m_Bloom0TexLoc, 1);
    render::Device::SetUniformSampler2D(m_Bloom1TexLoc, 2);
    render::Device::SetUniformSampler2D(m_Bloom2TexLoc, 3);
    render::Device::SetUniformSampler2D(m_Bloom3TexLoc, 4);
    render::Device::SetUniform1f(m_Bloom0WeightLoc, m_Bloom0Weight);
    render::Device::SetUniform1f(m_Bloom1WeightLoc, m_Bloom1Weight);
    render::Device::SetUniform1f(m_Bloom2WeightLoc, m_Bloom2Weight);
    render::Device::SetUniform1f(m_Bloom3WeightLoc, m_Bloom3Weight);

    // Draw
    render::Device::Draw(render::PT_TRIANGLES, 0, m_ScreenMesh->GetVertexNum());

    render::Device::SetRenderTarget(render::RenderTarget::DefaultRenderTarget());
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

int32_t Render::GetBRDFMap() {
    int32_t result = -1;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetBRDFMap();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
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

math::Matrix Render::GetDecalViewMatrix() {
    math::Matrix result;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetDecalViewMatrix();
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

math::Matrix Render::GetDecalProjMatrix() {
    math::Matrix result;

    if (s_RenderImp != NULL) {
        result = s_RenderImp->GetDecalProjMatrix();
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

void Render::SetBloomWeights(float w0, float w1, float w2, float w3) {
    if (s_RenderImp != NULL) {
        s_RenderImp->SetBloomWeights(w0, w1, w2, w3);
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

void Render::AddMenuMesh(math::Vector lt, math::Vector rb, math::Vector color) {
    if (s_RenderImp != NULL) {
        s_RenderImp->AddMenuMesh(lt, rb, color);
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

void Render::AddText(const char* text, math::Vector pos, math::Vector color, float scale) {
    if (s_RenderImp != NULL) {
        s_RenderImp->AddText(text, pos, color, scale);
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

};  // namespace render

};  // namespace glb