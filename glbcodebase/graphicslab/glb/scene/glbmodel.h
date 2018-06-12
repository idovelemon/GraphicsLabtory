//----------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 09 / 11
// Brief: Model manage all the meshes,materials and textures.
//----------------------------------------------------------------------
#ifndef GLB_GLBMODEL_H_
#define GLB_GLBMODEL_H_

#include "render/glbmesh.h"
#include "scene/glbmodelfile.h"

namespace glb {

namespace scene {

class Model {
public:
    enum {
        MT_ALBEDO = 0,
        MT_ROUGHNESS,
        MT_METALLIC,
        MT_ALPHA,
        MT_NORMAL,
        MT_EMISSION,
        MT_DIFFUSE_PFC,
        MT_SPECULAR_PFC,
        MT_REFLECT,
        MT_MAX,
    };
public:
    Model();
    virtual~Model();

public:
    static Model* Create(const char* file_name);
    static Model* Create(int32_t numTriangles, float* vertexBuf, float* texBuf = NULL, float* lightMapTexCoordBuf = NULL, float* normalBuf = NULL, float* tangentBuf = NULL, float* binormalBuf = NULL);

public:
    std::string GetName() const;
    int32_t GetMeshId() const;
    int32_t GetTexId(int32_t slot) const;
    int32_t GetMaterial() const;
    math::Vector GetBoundBoxMax() const;
    math::Vector GetBoundBoxMin() const;

    bool HasAlbedoTexture() const;
    bool HasRoughnessTexture() const;
    bool HasMettalicTexture() const;
    bool HasAlphaTexture() const;
    bool HasNormalTexture() const;
    bool HasEmissionTexture() const;
    bool HasReflectTexture() const;
    bool HasDiffusePFCTexture() const;
    bool HasSpecularPFCTexture() const;
    bool HasLightMapTexCoord() const;
    bool HasTexCoord() const;
    bool HasNormal() const;
    bool HasTangent() const;
    bool HasBinormal() const;
    bool IsAcceptLight() const;
    void SetAcceptLight(bool accept);
    bool IsAcceptShadow() const;
    void SetAcceptShadow(bool accept);
    bool IsCastShadow() const;
    void SetCastShadow(bool cast);
    bool IsUseAO() const;

    void SetTexWithId(int32_t slot, int32_t tex_id);

private:
    std::string         m_Name;
    int32_t             m_Mesh;
    int32_t             m_Tex[MT_MAX];
    int32_t             m_Material;
    math::Vector        m_BoundBoxMax;
    math::Vector        m_BoundBoxMin;
    ModelEffectParam    m_ModelEffectParam;
};

class ModelMgr {
public:
    static void Initialize();
    static void Destroy();
    static Model* AddModel(const char* file_name);
    static void AddModel(Model* model);
    static Model* GetModelByName(const char* file_name);
};

};  // namespace scene

};  // namespace glb

#endif  // GLB_GLBMODEL_H_