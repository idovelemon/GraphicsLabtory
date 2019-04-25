//------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2018. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2018 / 06 / 11
// Brief: Core routine for model editor
//------------------------------------------------------------------
#ifndef GLB_GLB_MODEL_EDITOR_CORE_H_
#define GLB_GLB_MODEL_EDITOR_CORE_H_

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

    bool AddModel(const char* name);
    bool SaveModel(const char* name);
    bool Preview(const char* name);
    bool AddEmptyMaterial(const char* name);
    bool AddMaterial(const char* name);
    bool SaveMaterial(const char* name);
    bool TryCompileShader(const char* passName, const char* vertexShaderName, const char* fragmentShaderName);
    std::vector<glb::render::material::PassMaterial::ParameterEntry>& GetPassParameters(const char* passName);
    std::vector<std::vector<glb::render::material::PassMaterial::ParameterEntry>> GetAllPassParameters();
    void SetPassParameterInt(const char* passName, const char* parameterName, int32_t value);
    void SetPassParameterFloat(const char* passName, const char* parameterName, float value);
    void SetPassParameterVec(const char* passName, const char* parameterName, glb::math::Vector value);
    void SetPassParameterTex(const char* passName, const char* parameterName, int32_t value);
    bool IsCastShadow();
    void SetCastShadow(bool bCastShadow);
    bool IsReceiveShadow();
    void SetReceiveShadow(bool bReceiveShadow);

protected:
    void UpdateCamera();

private:
    static ApplicationCore*             s_Instance;
    static int                          sIdGen;

    int32_t                             m_DefaultTexture2D;
    int32_t                             m_DefaultTextureCube;

    int32_t                             m_DefaultPlane;

    char                                m_SceneMeshName[256];
    int32_t                             m_SceneMesh;
    int32_t                             m_Material;
};

#endif  // GLB_GLB_MODEL_EDITOR_CORE_H_