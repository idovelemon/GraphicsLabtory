//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 06 / 07
// Brief: Core routine for light studio
//------------------------------------------------------------------
#ifndef GLB_GLB_LIGHT_STUDIO_CORE_H_
#define GLB_GLB_LIGHT_STUDIO_CORE_H_

#include "glb.h"

#include <map>

class ApplicationCore : public glb::app::ApplicationBase {
public:
    ApplicationCore();
    virtual ~ApplicationCore();

public:
    static glb::app::ApplicationBase* Create();
    static ApplicationCore* GetInstance();

public:
    virtual bool Initialize();
    virtual void Update(float dt);
    virtual void Destroy();

    bool AddSceneMesh(const char* name);
    int AddLightMesh(const char* name);
    void ChangeLightMapSize(int width, int height);
    void SetBakeIterate(int iterate);
    void Bake();
    void CancelBake();
    float GetCurProgress();
    bool IsBaking() const;
    void SetLightSourcePos(int id, float px, float py, float pz);
    void SetLightSourceRot(int id, float rx, float ry, float rz);
    void SetLightSourceScale(int id, float sx, float sy, float sz);
    void SetLightSourceColor(int id, float cx, float cy, float cz);

protected:
    void PreGenerate();
    void UpdateCamera();
    void BakeLightMap();
    void DrawScene();

    void DrawWeightMap();
    void CalcWeightMapColor();
    void DrawNormalizeWeightMap();

    void DrawHemiCube();
    void CalcLightPatchColor();
    bool FindNextValidLightPatch();
    void BuildLightMap();

    void PrepareBake();
    void ResetLightPatch();
    void PrepareLightPatch();

private:
    static ApplicationCore*             s_Instance;
    static int                          sIdGen;

    glb::scene::CameraBase*             m_Camera;
    glb::render::mesh::ScreenMesh*      m_ScreenMesh;

    struct LightSourceEntry {
        glb::math::Vector color;
        glb::scene::Object* obj;
        int id;

        LightSourceEntry()
        : color(0.0f, 0.0f, 0.0f)
        , obj(NULL)
        , id(-1) {
        }
    };
    typedef std::map<int, LightSourceEntry> LightSourceArray;
    LightSourceArray                    m_LightSource;
    glb::render::shader::UserProgram*   m_LightProgram;

    char                                m_SceneMeshName[256];
    glb::scene::Model*                  m_SceneMesh;
    glb::render::shader::UserProgram*   m_SceneProgram;

    glb::render::texture::Texture*      m_LightMap[3];
    int32_t                             m_LightMapWidth;
    int32_t                             m_LightMapHeight;
    int32_t                             m_TotalBakeIterate;
    int32_t                             m_CurBakeIterate;
    bool                                m_EnableBake;
    int32_t                             m_CurValidPatchIndex;

    glb::render::RenderTarget*          m_WeightRT;
    glb::render::texture::Texture*      m_WeightMap[5];
    glb::render::shader::UserProgram*   m_WeightProgram;
    int32_t                             m_WeightFaceLoc;
    int32_t                             m_WeightLightPatchWidthLoc;
    int32_t                             m_WeightLightPatchHeightLoc;

    glb::math::Vector                   m_TotalWeightColor;

    glb::render::RenderTarget*          m_NormalizeWeightRT;
    glb::render::texture::Texture*      m_NormalizeWeightMap[5];
    glb::render::shader::UserProgram*   m_NormalizeWeightProgram;
    int32_t                             m_TotalWeightColorLoc;
    int32_t                             m_WeightMapLoc;

    struct LightPatch {
        glb::math::Vector uv;
        glb::math::Vector pos;
        glb::math::Vector bais[3];
        glb::math::Vector color[3];
        bool valid;  // Some of light patches is not valid, because model's UV coordinates may not cover all range of uv.
    };
    LightPatch*                         m_Patch;
    std::vector<LightPatch*>            m_ValidPatch;
    glb::render::RenderTarget*          m_LightPatchRT[3];
    glb::render::texture::Texture*      m_LightPatchMap[3][5];
    glb::render::shader::UserProgram*   m_LightPatchSceneProgram;
    glb::render::shader::UserProgram*   m_LightPatchLightProgram;
    int32_t                             m_LightPatchMVPLoc;
    int32_t                             m_NormlaizeWeightMapLoc;
    int32_t                             m_LightMapLoc[3];
    int32_t                             m_LightPatchAlbedoMapLoc;
    int32_t                             m_LightPatchNormalMapLoc;
    int32_t                             m_LightPatchFaceLoc;
};

#endif  // GLB_GLB_LIGHT_STUDIO_CORE_H_