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
    bool SetModelAlbedoTexture(const char* name);
    bool SetModelMetallicTexture(const char* name);
    bool SetModelRoughnessTexture(const char* name);
    bool SetModelNormalTexture(const char* name);
    bool SetModelAlphaTexture(const char* name);
    bool SetModelEmissionTexture(const char* name);
    const char* GetModelAlbedoTextureName();
    const char* GetModelMetallicTextureName();
    const char* GetModelRoughnessTextureName();
    const char* GetModelNormalTextureName();
    const char* GetModelAlphaTextureName();
    const char* GetModelEmissionTextureName();
    const char* GetModelDiffusePFCTextureName();
    const char* GetModelSpecularPFCTextureName();

protected:
    void UpdateCamera();

private:
    static ApplicationCore*             s_Instance;
    static int                          sIdGen;

    char                                m_SceneMeshName[256];
    int32_t                             m_SceneMesh;
};

#endif  // GLB_GLB_MODEL_EDITOR_CORE_H_