//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016/06/19
// Brief: Define a interface to analysis the model file
// The format that supported by this interfact has showed below:
// .obj -- Exported by 3DMax2009
//--------------------------------------------------------------------
#ifndef GLB_GLBMODELFILE_H_
#define GLB_GLBMODELFILE_H_

#include <memory.h>
#include <stdint.h>
#include <stdio.h>

#include "math/glbvector.h"

#include "render/glbtexture.h"

namespace glb {

namespace scene {

class ModelEffectParam {
public:
    ModelEffectParam()
    : hasAlbedoTex(false)
    , hasRoughnessTex(false)
    , hasMetallicTex(false)
    , hasAlphaTex(false)
    , hasNormalTex(false)
    , hasEmissionTex(false)
    , hasDiffusePFCTex(false)
    , hasSpecularPFCTex(false)
    , hasTexcoord(false)
    , hasLightMapTexCoord(false)
    , hasNormal(false)
    , hasTanget(false)
    , hasBinormal(false)
    , acceptLight(false)
    , acceptShadow(false)
    , castShadow(false)
    , useAO(false) {
    }

    virtual ~ModelEffectParam() {
    }

public:
    bool    hasAlbedoTex;
    bool    hasRoughnessTex;
    bool    hasMetallicTex;
    bool    hasAlphaTex;
    bool    hasNormalTex;
    bool    hasEmissionTex;
    bool    hasDiffusePFCTex;
    bool    hasSpecularPFCTex;
    bool    hasTexcoord;
    bool    hasLightMapTexCoord;
    bool    hasNormal;
    bool    hasTanget;
    bool    hasBinormal;
    bool    acceptLight;
    bool    acceptShadow;
    bool    castShadow;
    bool    useAO;
};

class ModelMaterialParam {
public:
    ModelMaterialParam()
    : ambient(0.0f, 0.0f, 0.0)
    , diffuse(0.0f, 0.0f, 0.0f)
    , specular(0.0f, 0.0f, 0.0f)
    , emission(0.0f, 0.0f, 0.0f)
    , pow(0.0f)
    , albedo(0.0f, 0.0f, 0.0f)
    , roughness(0.0f)
    , metallic(0.0f)
    , boundboxMin(0.0f, 0.0f, 0.0f)
    , boundboxMax(0.0f, 0.0f, 0.0f) {
        memset(albedoTexName, 0, sizeof(albedoTexName));
        memset(roughnessTexName, 0, sizeof(roughnessTexName));
        memset(metallicTexName, 0, sizeof(metallicTexName));
        memset(alphaTexName, 0, sizeof(alphaTexName));
        memset(normalTexName, 0, sizeof(normalTexName));
        memset(emissionTexName, 0, sizeof(emissionTexName));
        memset(diffusePFCTexName, 0, sizeof(diffusePFCTexName));
        memset(specularPFCTexName, 0, sizeof(specularPFCTexName));
    }

    virtual ~ModelMaterialParam() {
    }

public:
    math::Vector ambient;
    math::Vector diffuse;
    math::Vector specular;
    math::Vector emission;
    math::Vector albedo;
    float        pow;
    float        roughness;
    float        metallic;
    char         albedoTexName[render::texture::kMaxTexNameLen];
    char         roughnessTexName[render::texture::kMaxTexNameLen];
    char         metallicTexName[render::texture::kMaxTexNameLen];
    char         alphaTexName[render::texture::kMaxTexNameLen];
    char         normalTexName[render::texture::kMaxTexNameLen];
    char         emissionTexName[render::texture::kMaxTexNameLen];
    char         diffusePFCTexName[render::texture::kMaxTexNameLen];
    char         specularPFCTexName[render::texture::kMaxTexNameLen];
    math::Vector boundboxMin;
    math::Vector boundboxMax;
};

class ModelFile {
public:
    //----------------------------------------------------------------------------------------------
    // @brief: Extract the vertex and texture coordinate data from the model file
    // @param: model_file_name The name of the model's file
    // @param: effect_param The effect parameter to show if accept lignt, cast shadow, accept shadow and so on
    // @param: material_param The material parameter
    // @param: vertex_buf A pointer to recieve the vertex data. This can not be null.
    // @param: texcoord_buf A pointer to recieve the texture coordinate data. This is an optional parameter
    // @param: normal_buf A pointer to recieve the normal data. This is an optional parameter
    // @param: tangent_buf A pointer to recieve the tangent data. This is an optional parameter
    // @param: binormal_buf A pointter to recieve the binormal data. This is an optional parameter
    // @return: If successfully, return the number of triangles. Otherwise retur 0.
    //----------------------------------------------------------------------------------------------
    static int32_t ExtractModelData(
        const char* model_file_name, 
        ModelEffectParam& effect_param,
        ModelMaterialParam& material_param,
        float** vertex_buf,
        float** texcoord_buf = NULL,
        float** lightMapTexCoordBuf = NULL,
        float** normal_buf = NULL,
        float** tangent_buf = NULL,
        float** binormal_buf = NULL
        );

    //----------------------------------------------------------------------------------------------
    // @brief: Release the buffer memory that returned by ExtractModelData method
    // @param: vertex_buf The buffer pointer hold the vertex data
    // @param: texcoord_buf The buffer pointer hold the texcoord data
    // @param: normal_buf The buffer pointer hold the normal data
    // @param: tangent_buf The buffer pointer hold the tangent data
    // @param: binormal_buf The buffer pointer hold the binormal data
    //----------------------------------------------------------------------------------------------
    static void RelaseBuf(float** vertex_buf, float** texcoord_buf = NULL, float** lightMapTexCoordBuf = NULL, float** normal_buf = NULL
        , float** tangent_buf = NULL, float** binormal_buf = NULL);
};

};  // namespace scene

};  // namespace glb

#endif  // GLB_GLB_MODELFILE_H_
