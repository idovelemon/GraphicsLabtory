//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016/06/19
// Brief: Define a interface to analysis the model file
//--------------------------------------------------------------------
#include "glbmodelfile.h"

#include <fstream>
#include <vector>

#include "math/glbvector.h"

#include "util/glbmacro.h"

namespace glb {

namespace scene {

//----------------------------------------------------------------------------------------
// CONSTANT VALUE
//----------------------------------------------------------------------------------------
const int32_t kMaxFilePostFixLength = 8;
const int32_t kVertexPerTri = 3;
const int32_t kObjFloatPerVertex = 3;
const int32_t kObjFloatPerTexcoord = 2;
const int32_t kObjFloatPerNormal = 3;
const int32_t kObjFloatPerTangent = 3;
const int32_t kObjFloatPerBinormal = 3;
const int32_t kObjMaxPrefixLength = 32;
const int32_t kObjMaxLineLength = 256;

//----------------------------------------------------------------------------------------
// CLASS DECLARATION
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
// ModelFileBase DECLARATION
//----------------------------------------------------------------------------------------
class ModelFileBase {
public:
    ModelFileBase() {
    }

    virtual ~ModelFileBase() {
    }

public:
    virtual int32_t ExtractModelData(
        const char* model_file_name, 
        ModelEffectParam& effect_param,
        ModelMaterialParam& material_param,
        float** vertex_buf,
        float** texcoord_buf = NULL,
        float** lightMapTexCoordBuf = NULL,
        float** normal_buf = NULL,
        float** tangent_buf = NULL,
        float** binormal_buf = NULL
        ) = 0;
};

//----------------------------------------------------------------------------------------
// ObjModelFile DECLARATION
//----------------------------------------------------------------------------------------
class ObjModelFile:public ModelFileBase {
 public:
     ObjModelFile();
     virtual ~ObjModelFile();

 public:
     int32_t ExtractModelData(
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

 protected:
     void ExtractFaceData(const char* buffer, int32_t& vertex_index, int32_t& texcoord_index, int32_t& light_map_tex_index, int32_t& normal_index, int32_t& tanget_index, int32_t& binormal_index);
};

//----------------------------------------------------------------------------------------
// CLASS DEFINITION
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
// ObjModelFile DEFINITION
//----------------------------------------------------------------------------------------
class ObjFace {
public:
    int32_t vertex_index[3];
    int32_t texcoord_index[3];
    int32_t lightmap_texcoord_index[3];
    int32_t normal_index[3];
    int32_t tangent_index[3];
    int32_t binormal_index[3];

    ObjFace() {
        memset(vertex_index, 0, sizeof(vertex_index));
        memset(texcoord_index, 0, sizeof(texcoord_index));
        memset(lightmap_texcoord_index, 0, sizeof(lightmap_texcoord_index));
        memset(normal_index, 0, sizeof(normal_index));
        memset(tangent_index, 0, sizeof(tangent_index));
        memset(binormal_index, 0, sizeof(binormal_index));
    }
};

ObjModelFile::ObjModelFile() {
}
ObjModelFile::~ObjModelFile() {
}

int32_t ObjModelFile::ExtractModelData(
        const char* modelFileName, 
        ModelEffectParam& effectParam,
        ModelMaterialParam& materialParam,
        float** vertexBuf,
        float** texcoordBuf,
        float** lightMapTexCoordBuf,
        float** normalBuf,
        float** tangentBuf,
        float** binormalBuf
        ) {
    int32_t result = 0;

    if (modelFileName != NULL && vertexBuf != NULL) {
        std::ifstream input;
        input.open(modelFileName);

        if (!input.fail()) {
            std::vector<math::Vector> vertexArray;
            std::vector<math::Vector> texArray;
            std::vector<math::Vector> lightMapTexArray;
            std::vector<math::Vector> normalArray;
            std::vector<math::Vector> tangentArray;
            std::vector<math::Vector> binormalArray;
            std::vector<ObjFace> faceArray;

            char prefix[kObjMaxPrefixLength];
            char buffer[kObjMaxLineLength];

            materialParam.boundboxMin.x = FLT_MAX;
            materialParam.boundboxMin.y = FLT_MAX;
            materialParam.boundboxMin.z = FLT_MAX;
            materialParam.boundboxMax.x = 0.0f;
            materialParam.boundboxMax.y = 0.0f;
            materialParam.boundboxMax.z = 0.0f;

            // Read the data from file
            while (!input.eof()) {
                input >> prefix;

                if (!strcmp(prefix, "v")) {
                    // Vertex
                    math::Vector vertex;
                    input >> vertex.x >> vertex.y >> vertex.z;
                    vertexArray.push_back(vertex);

                    if (vertex.x < materialParam.boundboxMin.x) {
                        materialParam.boundboxMin.x = vertex.x;
                    } else if (vertex.x > materialParam.boundboxMax.x) {
                        materialParam.boundboxMax.x = vertex.x;
                    }

                    if (vertex.y < materialParam.boundboxMin.y) {
                        materialParam.boundboxMin.y = vertex.y;
                    } else if (vertex.y > materialParam.boundboxMax.y) {
                        materialParam.boundboxMax.y = vertex.y;
                    }

                    if (vertex.z < materialParam.boundboxMin.z) {
                        materialParam.boundboxMin.z = vertex.z;
                    } else if (vertex.z > materialParam.boundboxMax.z) {
                        materialParam.boundboxMax.z = vertex.z;
                    }
                } else if (!strcmp(prefix, "vt")) {
                    // Texture Coordinate
                    math::Vector texcoord;
                    input >> texcoord.x >> texcoord.y;
                    texArray.push_back(texcoord);

                    effectParam.hasTexcoord = true;
                } else if (!strcmp(prefix, "vlightmapt")) {
                    // Light Map Texture Coordinate
                    math::Vector texcoord;
                    input >> texcoord.x >> texcoord.y;
                    lightMapTexArray.push_back(texcoord);

                    effectParam.hasLightMapTexCoord = true;
                } else if (!strcmp(prefix, "vn")) {
                    // Normal
                    math::Vector normal;
                    input >> normal.x >> normal.y >> normal.z;
                    normalArray.push_back(normal);
                    effectParam.hasNormal = true;
                } else if (!strcmp(prefix, "vtan")) {
                    // Tangent
                    math::Vector tanget;
                    input >> tanget.x >> tanget.y >> tanget.z;
                    tangentArray.push_back(tanget);
                    effectParam.hasTanget = true;
                } else if (!strcmp(prefix, "vbi")) {
                    // Binormal
                    math::Vector binormal;
                    input >> binormal.x >> binormal.y >> binormal.z;
                    binormalArray.push_back(binormal);
                    effectParam.hasBinormal = true;
                } else if (!strcmp(prefix, "f")) {
                    // Triangle
                    ObjFace face;

                    if (!effectParam.hasTexcoord) {
                        for (int32_t i = 0; i < kVertexPerTri; i++) {
                            face.texcoord_index[i] = -1;  // Indicate do not have texture coordinate
                        }
                    }
                    if (!effectParam.hasLightMapTexCoord) {
                        for (int32_t i = 0; i < kVertexPerTri; i++) {
                            face.lightmap_texcoord_index[i] = -1;  // Indicate do not have light map texture coordinate
                        }
                    }
                    if (!effectParam.hasNormal) {
                        for (int32_t i = 0; i < kVertexPerTri; i++) {
                            face.normal_index[i] = -1;  // Indicate do not have normal
                        }
                    }
                    if (!effectParam.hasTanget) {
                        for (int32_t i = 0; i < kVertexPerTri; i++) {
                            face.tangent_index[i] = -1;  // Indicate do not have tangent
                        }
                    }
                    if (!effectParam.hasBinormal) {
                        for (int32_t i = 0; i < kVertexPerTri; i++) {
                            face.binormal_index[i] = -1;  // Indicate do not have binormal
                        }
                    }
                    for (int32_t i = 0; i < kVertexPerTri; i++) {
                        input >> buffer;
                        ExtractFaceData(buffer, face.vertex_index[i], face.texcoord_index[i], face.lightmap_texcoord_index[i], face.normal_index[i], face.tangent_index[i], face.binormal_index[i]);
                    }

                    faceArray.push_back(face);
                } else if (!strcmp(prefix, "talbedo")) {
                    // Albedo texture
                    input >> buffer;
                    effectParam.hasAlbedoTex = true;

                    int32_t len = strlen(buffer);
                    memcpy(materialParam.albedoTexName, buffer, len);
                    materialParam.albedoTexName[len] = '\0';
                } else if (!strcmp(prefix, "troughness")) {
                    // Roughness texture
                    input >> buffer;
                    effectParam.hasRoughnessTex = true;

                    int32_t len = strlen(buffer);
                    memcpy(materialParam.roughnessTexName, buffer, len);
                    materialParam.roughnessTexName[len] = '\0';
                } else if (!strcmp(prefix, "tmetallic")) {
                    // Metallic texture
                    input >> buffer;
                    effectParam.hasMetallicTex = true;

                    int32_t len = strlen(buffer);
                    memcpy(materialParam.metallicTexName, buffer, len);
                    materialParam.metallicTexName[len] = '\0';
                } else if (!strcmp(prefix, "talpha")) {
                    // Alpha texture
                    input >> buffer;
                    effectParam.hasAlphaTex = true;

                    int32_t len = strlen(buffer);
                    memcpy(materialParam.alphaTexName, buffer, len);
                    materialParam.alphaTexName[len] = '\0';
                } else if (!strcmp(prefix, "tnormal")) {
                    // Normal texture
                    input >> buffer;
                    effectParam.hasNormalTex = true;

                    int32_t len = strlen(buffer);
                    memcpy(materialParam.normalTexName, buffer, len);
                    materialParam.normalTexName[len] = '\0';
                } else if (!strcmp(prefix, "temission")) {
                    // Emission texture
                    input >> buffer;
                    effectParam.hasEmissionTex = true;

                    int32_t len = strlen(buffer);
                    memcpy(materialParam.emissionTexName, buffer, len);
                    materialParam.emissionTexName[len] = '\0';
                } else if (!strcmp(prefix, "tdiffusepfc")) {
                    // Diffuse Prefilter CubeMap texture
                    input >> buffer;
                    effectParam.hasDiffusePFCTex = true;

                    int32_t len = strlen(buffer);
                    memcpy(materialParam.diffusePFCTexName, buffer, len);
                    materialParam.diffusePFCTexName[len] = '\0';
                } else if (!strcmp(prefix, "tspecularpfc")) {
                    // Specular Prefilter CubeMap texture
                    input >> buffer;
                    effectParam.hasSpecularPFCTex = true;

                    int32_t len = strlen(buffer);
                    memcpy(materialParam.specularPFCTexName, buffer, len);
                    materialParam.specularPFCTexName[len] = '\0';
                } else if (!strcmp(prefix, "al")) {
                    // Accept light
                    int32_t temp = 0;
                    input >> temp;
                    if (temp != 0) {
                        effectParam.acceptLight = true;
                    } else {
                        effectParam.acceptLight = false;
                    }
                } else if (!strcmp(prefix, "as")) {
                    // Accept shadow
                    int32_t temp = 0;
                    input >> temp;
                    if (temp != 0) {
                        effectParam.acceptShadow = true;
                    } else {
                        effectParam.acceptShadow = false;
                    }
                } else if (!strcmp(prefix, "ao")) {
                    // Use AO
                    int32_t temp = 0;
                    input >> temp;
                    if (temp != 0) {
                        effectParam.useAO = true;
                    } else {
                        effectParam.useAO = false;
                    }
                } else if (!strcmp(prefix, "cs")) {
                    // Cast shadow
                    int32_t temp = 0;
                    input >> temp;
                    if (temp != 0) {
                        effectParam.castShadow = true;
                    } else {
                        effectParam.castShadow = false;
                    }
                } else if (!strcmp(prefix, "ma")) {
                    // Material ambient
                    math::Vector ambient;
                    input >> ambient.x >> ambient.y >> ambient.z;
                    materialParam.ambient = ambient;
                } else if (!strcmp(prefix, "md")) {
                    // Material diffuse
                    math::Vector diffuse;
                    input >> diffuse.x >> diffuse.y >> diffuse.z;
                    materialParam.diffuse = diffuse;
                } else if (!strcmp(prefix, "ms")) {
                    // Material specular
                    math::Vector specular;
                    input >> specular.x >> specular.y >> specular.z;
                    materialParam.specular = specular;
                } else if (!strcmp(prefix, "me")) {
                    // Material emission
                    math::Vector emission;
                    input >> emission.x >> emission.y >> emission.z;
                    materialParam.emission = emission;
                } else if (!strcmp(prefix, "mp")) {
                    // Material pow
                    float pow = 0.0f;
                    input >> pow;
                    materialParam.pow = pow;
                } else if (!strcmp(prefix, "malbedo")) {
                    // Material base color(or reflectance for metal)
                    math::Vector albedo;
                    input >> albedo.x;
                    input >> albedo.y;
                    input >> albedo.z;
                    materialParam.albedo = albedo;
                } else if (!strcmp(prefix, "mroughness")) {
                    // Material roughness
                    float roughness = 0.0f;
                    input >> roughness;
                    materialParam.roughness = roughness;
                } else if (!strcmp(prefix, "mmetallic")) {
                    // Material metallic
                    float metallic = 0.0f;
                    input >> metallic;
                    materialParam.metallic = metallic;
                } else {
                    input.getline(buffer, sizeof(buffer));
                    memset(buffer, 0, sizeof(buffer));
                }
            }

            input.close();

            // Generate the buffer
            int32_t triangle_num = faceArray.size();
            *vertexBuf = new float[kVertexPerTri * triangle_num * kObjFloatPerVertex];
            for (int32_t i = 0; i < triangle_num; i++) {
                for (int32_t j = 0; j < kVertexPerTri; j++) {
                    (*vertexBuf)[i * kVertexPerTri * kObjFloatPerVertex + j * kObjFloatPerVertex + 0] = vertexArray[faceArray[i].vertex_index[j] - 1].x;
                    (*vertexBuf)[i * kVertexPerTri * kObjFloatPerVertex + j * kObjFloatPerVertex + 1] = vertexArray[faceArray[i].vertex_index[j] - 1].y;
                    (*vertexBuf)[i * kVertexPerTri * kObjFloatPerVertex + j * kObjFloatPerVertex + 2] = vertexArray[faceArray[i].vertex_index[j] - 1].z;
                }
            }

            if (texArray.size() > 0 && texcoordBuf != NULL) {
                *texcoordBuf = new float[kVertexPerTri * triangle_num * kObjFloatPerTexcoord];
                for (int32_t i = 0; i < triangle_num; i++) {
                    for (int32_t j = 0; j < kVertexPerTri; j++) {
                        (*texcoordBuf)[i * kVertexPerTri * kObjFloatPerTexcoord + j * kObjFloatPerTexcoord + 0] = texArray[faceArray[i].texcoord_index[j] - 1].x;
                        (*texcoordBuf)[i * kVertexPerTri * kObjFloatPerTexcoord + j * kObjFloatPerTexcoord + 1] = texArray[faceArray[i].texcoord_index[j] - 1].y;
                    }
                }
            }

            if (lightMapTexArray.size() > 0 && lightMapTexCoordBuf != NULL) {
                *lightMapTexCoordBuf = new float[kVertexPerTri * triangle_num * kObjFloatPerTexcoord];
                for (int32_t i = 0; i < triangle_num; i++) {
                    for (int32_t j = 0; j < kVertexPerTri; j++) {
                        (*lightMapTexCoordBuf)[i * kVertexPerTri * kObjFloatPerTexcoord + j * kObjFloatPerTexcoord + 0] = lightMapTexArray[faceArray[i].lightmap_texcoord_index[j] - 1].x;
                        (*lightMapTexCoordBuf)[i * kVertexPerTri * kObjFloatPerTexcoord + j * kObjFloatPerTexcoord + 1] = lightMapTexArray[faceArray[i].lightmap_texcoord_index[j] - 1].y;
                    }
                }
            }

            if (normalArray.size() > 0 && normalBuf != NULL) {
                *normalBuf = new float[kVertexPerTri * triangle_num * kObjFloatPerNormal];
                for (int32_t i = 0; i < triangle_num; i++) {
                    for (int32_t j = 0; j < kVertexPerTri; j++) {
                        (*normalBuf)[i * kVertexPerTri * kObjFloatPerNormal + j * kObjFloatPerNormal + 0] = normalArray[faceArray[i].normal_index[j] - 1].x;
                        (*normalBuf)[i * kVertexPerTri * kObjFloatPerNormal + j * kObjFloatPerNormal + 1] = normalArray[faceArray[i].normal_index[j] - 1].y;
                        (*normalBuf)[i * kVertexPerTri * kObjFloatPerNormal + j * kObjFloatPerNormal + 2] = normalArray[faceArray[i].normal_index[j] - 1].z;
                    }
                }
            }

            if (tangentArray.size() > 0 && tangentBuf != NULL) {
                *tangentBuf = new float[kVertexPerTri * triangle_num * kObjFloatPerTangent];
                for (int32_t i = 0; i < triangle_num; i++) {
                    for (int32_t j = 0; j < kVertexPerTri; j++) {
                        (*tangentBuf)[i * kVertexPerTri * kObjFloatPerTangent + j * kObjFloatPerTangent + 0] = tangentArray[faceArray[i].tangent_index[j] - 1].x;
                        (*tangentBuf)[i * kVertexPerTri * kObjFloatPerTangent + j * kObjFloatPerTangent + 1] = tangentArray[faceArray[i].tangent_index[j] - 1].y;
                        (*tangentBuf)[i * kVertexPerTri * kObjFloatPerTangent + j * kObjFloatPerTangent + 2] = tangentArray[faceArray[i].tangent_index[j] - 1].z;
                    }
                }
            }

            if (binormalArray.size() > 0 && binormalBuf != NULL) {
                *binormalBuf = new float[kVertexPerTri * triangle_num * kObjFloatPerBinormal];
                for (int32_t i = 0; i < triangle_num; i++) {
                    for (int32_t j = 0; j < kVertexPerTri; j++) {
                        (*binormalBuf)[i * kVertexPerTri * kObjFloatPerBinormal + j * kObjFloatPerBinormal + 0] = binormalArray[faceArray[i].binormal_index[j] - 1].x;
                        (*binormalBuf)[i * kVertexPerTri * kObjFloatPerBinormal + j * kObjFloatPerBinormal + 1] = binormalArray[faceArray[i].binormal_index[j] - 1].y;
                        (*binormalBuf)[i * kVertexPerTri * kObjFloatPerBinormal + j * kObjFloatPerBinormal + 2] = binormalArray[faceArray[i].binormal_index[j] - 1].z;
                    }
                }
            }

            // Save the number of triangles
            result = faceArray.size();
        } else {
            input.close();
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

void ObjModelFile::ExtractFaceData(const char* buffer, int32_t& vertex_index, int32_t& texcoord_index, int32_t& light_map_tex_index, int32_t& normal_index, int32_t& tanget_index, int32_t& binormal_index) {
    if (buffer != NULL) {
        // Extract vertex index
        vertex_index = atoi(buffer);

        // Has texture coordinate index ?
        int32_t index = 0;
        int32_t buf_len = strlen(buffer);

        if (texcoord_index != -1) {
            for (index = 0; index < buf_len; index++) {
                if (buffer[index] == '/') {
                    break;
                }
            }

            if (index < buf_len - 1) {
                if (buffer[index + 1] != '/') {
                    const char* tex_index_buf = reinterpret_cast<const char*>(buffer + index + 1);
                    texcoord_index = atoi(tex_index_buf);
                }
            }
        }

        // Has light map texture coordinate index ?
        if (light_map_tex_index != -1) {
            for (index = index+ 1; index < buf_len; index++) {
                if (buffer[index] == '/') {
                    break;
                }
            }

            if (index < buf_len - 1) {
                const char* light_map_tex_index_buf = reinterpret_cast<const char*>(buffer + index + 1);
                light_map_tex_index = atoi(light_map_tex_index_buf);
            }
        }

        // Has normal index ?
        if (normal_index != -1) {
            for (index = index + 1; index < buf_len; index++) {
                if (buffer[index] == '/') {
                    break;
                }
            }

            if (index < buf_len - 1) {
                const char* normal_index_buf = reinterpret_cast<const char*>(buffer + index + 1);
                normal_index = atoi(normal_index_buf);
            }
        }

        // Has tanget index ?
        if (tanget_index != -1) {
            for (index = index + 1; index < buf_len; index++) {
                if (buffer[index] == '/') {
                    break;
                }
            }

            if (index < buf_len - 1) {
                const char* tanget_index_buf = reinterpret_cast<const char*>(buffer + index + 1);
                tanget_index = atoi(tanget_index_buf);
            }
        }

        // Has binormal index ?
        if (binormal_index != -1) {
            for (index = index + 1; index < buf_len; index++) {
                if (buffer[index] == '/') {
                    break;
                }
            }

            if (index < buf_len - 1) {
                const char* binormal_index_buf = reinterpret_cast<const char*>(buffer + index + 1);
                binormal_index = atoi(binormal_index_buf);
            }
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

//----------------------------------------------------------------------------------------
// ModelFile DEFINITION
//----------------------------------------------------------------------------------------
int32_t ModelFile::ExtractModelData(const char* model_file_name, 
                                    ModelEffectParam& effect_param,
                                    ModelMaterialParam& material_param,
                                    float** vertex_buf,
                                    float** texcoord_buf,
                                    float** lightMapTexCoordBuf,
                                    float** normal_buf,
                                    float** tanget_buf,
                                    float** binormal_buf) {
    int32_t result = 0;

    if (model_file_name != NULL && vertex_buf != NULL) {
        char postfix[kMaxFilePostFixLength];
        int32_t len = strlen(model_file_name);
        int32_t dot_index = 0;
        for (; dot_index < len; dot_index++) {
            if (model_file_name[dot_index] == '.') {
                break;
            }
        }

        // Has dot?
        if (dot_index < len - 1) {
            memcpy(postfix, model_file_name + dot_index, len - dot_index);
            postfix[len - dot_index] = 0;

            if (!strcmp(postfix, ".obj")) {
                ObjModelFile obj_model;
                result = obj_model.ExtractModelData(
                    model_file_name,
                    effect_param,
                    material_param,
                    vertex_buf,
                    texcoord_buf,
                    lightMapTexCoordBuf,
                    normal_buf,
                    tanget_buf,
                    binormal_buf);
            } else {
                GLB_SAFE_ASSERT(false);
            }
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

void ModelFile::RelaseBuf(float** vertex_buf, float** texcoord_buf, float** lightMapTexCoordBuf, float** normal_buf, float** tanget_buf, float** binormal_buf) {
    if (vertex_buf != NULL) {
        float* buf = *vertex_buf;
        if (buf != NULL) {
            delete[] buf;
            buf = NULL;
        }
        *vertex_buf = NULL;
    }

    if (texcoord_buf != NULL) {
        float* buf = *texcoord_buf;
        if (buf != NULL) {
            delete[] buf;
            buf = NULL;
        }
        *texcoord_buf = NULL;
    }

    if (lightMapTexCoordBuf != NULL) {
        float* buf = *lightMapTexCoordBuf;
        if (buf != NULL) {
            delete[] buf;
            buf = NULL;
        }
        *lightMapTexCoordBuf = NULL;
    }

    if (normal_buf != NULL) {
        float* buf = *normal_buf;
        if (buf != NULL) {
            delete[] buf;
            buf = NULL;
        }
        *normal_buf = NULL;
    }

    if (tanget_buf != NULL) {
        float* buf = *tanget_buf;
        if (buf != NULL) {
            delete[] buf;
            buf = NULL;
        }
        *tanget_buf = NULL;
    }

    if (binormal_buf != NULL) {
        float* buf = *binormal_buf;
        if (buf != NULL) {
            delete[] buf;
            buf = NULL;
        }
        *binormal_buf = NULL;
    }
}

};  // namespace scene

};  // namespace glb