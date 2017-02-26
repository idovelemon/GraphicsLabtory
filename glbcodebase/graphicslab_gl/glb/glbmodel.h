//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 09 / 11
// Brief: Model manage all the meshes,materials and textures.
//----------------------------------------------------------------------
#ifndef GLB_GLBMODEL_H_
#define GLB_GLBMODEL_H_

#include "glbmesh.h"
#include "glbmodelfile.h"

namespace glb {

class Model {
public:
    Model();
    virtual~Model();

public:
    static Model* Create(const char* file_name);

public:
    std::string GetName();
    int32_t GetMeshId();
    int32_t GetTexId(int32_t slot);
    int32_t GetMaterial();
    Vector GetBoundBoxMax();
    Vector GetBoundBoxMin();

    bool HasDiffuseTexture();
    bool HasAlphaTexture();
    bool HasNormal();
    bool IsAcceptLight();
    void SetAcceptLight(bool accept);
    bool IsAcceptShadow();
    void SetAcceptShadow(bool accept);
    bool IsCastShadow();
    void SetCastShadow(bool cast);
    bool IsUseAO();

private:
    std::string m_Name;
    int32_t m_Mesh;
    int32_t m_Tex[render::TS_MAX];
    int32_t m_Material;
    Vector  m_BoundBoxMax;
    Vector  m_BoundBoxMin;
    ModelEffectParam m_ModefEffectParam;
};  // class Model

class ModelMgr {
public:
    static void Initialize();
    static void Destroy();
    static Model* AddModel(const char* file_name);
    static Model* GetModelByName(const char* file_name);
};  // class ModelMgr

};  // namespace glb

#endif  // GLB_GLBMODEL_H_