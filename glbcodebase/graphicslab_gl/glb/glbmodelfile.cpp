//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016/06/19
// Brief: Define a interface to analysis the model file
//--------------------------------------------------------------------
#include "glbmodelfile.h"

#include <fstream>
#include <vector>

#include "glbmacro.h"
#include "glbvector.h"

namespace glb {

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
const int32_t kObjMaxPrefixLength = 8;
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
        float** normal_buf = NULL,
        float** tangent_buf = NULL,
        float** binormal_buf = NULL
        );

 protected:
     void ExtractFaceData(const char* buffer, int32_t& vertex_index, int32_t& texcoord_index, int32_t& normal_index, int32_t& tangent_index, int32_t& binormal_index);
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
    int32_t normal_index[3];
    int32_t tangent_index[3];
    int32_t binormal_index[3];

    ObjFace() {
        memset(vertex_index, 0, sizeof(vertex_index));
        memset(texcoord_index, 0, sizeof(texcoord_index));
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
        const char* model_file_name, 
        ModelEffectParam& effect_param,
        ModelMaterialParam& material_param,
        float** vertex_buf,
        float** texcoord_buf,
        float** normal_buf,
        float** tangent_buf,
        float** binormal_buf
        ) {
    int32_t result = 0;

    if (model_file_name != NULL && vertex_buf != NULL) {
        std::ifstream input;
        input.open(model_file_name);

        if (!input.fail()) {
            std::vector<Vector> vertex_array;
            std::vector<Vector> tex_array;
            std::vector<Vector> normal_array;
            std::vector<Vector> tangent_array;
            std::vector<Vector> binormal_array;
            std::vector<ObjFace> face_array;

            char prefix[kObjMaxPrefixLength];
            char buffer[kObjMaxLineLength];

            material_param.boundbox_min.x = FLT_MAX;
            material_param.boundbox_min.y = FLT_MAX;
            material_param.boundbox_min.z = FLT_MAX;
            material_param.boundbox_max.x = 0.0f;
            material_param.boundbox_max.y = 0.0f;
            material_param.boundbox_max.z = 0.0f;

            // Read the data from file
            while (!input.eof()) {    
                input >> prefix;

                if (!strcmp(prefix, "v")) {
                    // Vertex
                    Vector vertex;
                    input >> vertex.x >> vertex.y >> vertex.z;
                    vertex_array.push_back(vertex);

                    if (vertex.x < material_param.boundbox_min.x) {
                        material_param.boundbox_min.x = vertex.x;
                    } else if (vertex.x > material_param.boundbox_max.x) {
                        material_param.boundbox_max.x = vertex.x;
                    }

                    if (vertex.y < material_param.boundbox_min.y) {
                        material_param.boundbox_min.y = vertex.y;
                    } else if (vertex.y > material_param.boundbox_max.y) {
                        material_param.boundbox_max.y = vertex.y;
                    }

                    if (vertex.z < material_param.boundbox_min.z) {
                        material_param.boundbox_min.z = vertex.z;
                    } else if (vertex.z > material_param.boundbox_max.z) {
                        material_param.boundbox_max.z = vertex.z;
                    }
                } else if (!strcmp(prefix, "vt")) {
                    // Texture Coordinate
                    Vector texcoord;
                    input >> texcoord.x >> texcoord.y >> texcoord.z;
                    tex_array.push_back(texcoord);
                } else if (!strcmp(prefix, "vn")) {
                    // Normal
                    Vector normal;
                    input >> normal.x >> normal.y >> normal.z;
                    normal_array.push_back(normal);
                    effect_param.has_normal = true;
                } else if (!strcmp(prefix, "vtan")) {
                    // Tangent
                    Vector tanget;
                    input >> tanget.x >> tanget.y >> tanget.z;
                    tangent_array.push_back(tanget);
                    effect_param.has_tanget = true;
                } else if (!strcmp(prefix, "vbi")) {
                    // Binormal
                    Vector binormal;
                    input >> binormal.x >> binormal.y >> binormal.z;
                    binormal_array.push_back(binormal);
                    effect_param.has_binormal = true;
                } else if (!strcmp(prefix, "f")) {
                    // Triangle
                    ObjFace face;
                    
                    for (int32_t i = 0; i < kVertexPerTri; i++) {
                        input >> buffer;
                        ExtractFaceData(buffer, face.vertex_index[i], face.texcoord_index[i], face.normal_index[i], face.tangent_index[i], face.binormal_index[i]);
                    }

                    face_array.push_back(face);
                } else if (!strcmp(prefix, "td")) {
                    // Diffuse texture
                    input >> buffer;
                    effect_param.has_diffuse_tex = true;

                    int32_t len = strlen(buffer);
                    memcpy(material_param.diffuse_tex_name, buffer, len);
                    material_param.diffuse_tex_name[len] = '\0';
                } else if (!strcmp(prefix, "ta")) {
                    // Alpha texture
                    input >> buffer;
                    effect_param.has_alpha_tex = true;

                    int32_t len = strlen(buffer);
                    memcpy(material_param.alpha_tex_name, buffer, len);
                    material_param.alpha_tex_name[len] = '\0';
                } else if (!strcmp(prefix, "tn")) {
                    // Normal texture
                    input >> buffer;
                    effect_param.has_normal_tex = true;

                    int32_t len = strlen(buffer);
                    memcpy(material_param.normal_tex_name, buffer, len);
                    material_param.normal_tex_name[len] = '\0';
                } else if (!strcmp(prefix, "al")) {
                    // Accept light
                    int32_t temp = 0;
                    input >> temp;
                    if (temp != 0) {
                        effect_param.accept_light = true;
                    } else {
                        effect_param.accept_light = false;
                    }
                } else if (!strcmp(prefix, "as")) {
                    // Accept shadow
                    int32_t temp = 0;
                    input >> temp;
                    if (temp != 0) {
                        effect_param.accept_shadow = true;
                    } else {
                        effect_param.accept_shadow = false;
                    }
                } else if (!strcmp(prefix, "ao")) {
                    // Use AO
                    int32_t temp = 0;
                    input >> temp;
                    if (temp != 0) {
                        effect_param.use_ao = true;
                    } else {
                        effect_param.use_ao = false;
                    }
                } else if (!strcmp(prefix, "cs")) {
                    // Cast shadow
                    int32_t temp = 0;
                    input >> temp;
                    if (temp != 0) {
                        effect_param.cast_shadow = true;
                    } else {
                        effect_param.cast_shadow = false;
                    }
                } else if (!strcmp(prefix, "ma")) {
                    // Material ambient
                    Vector ambient;
                    input >> ambient.x >> ambient.y >> ambient.z;
                    material_param.ambient = ambient;
                } else if (!strcmp(prefix, "md")) {
                    // Material diffuse
                    Vector diffuse;
                    input >> diffuse.x >> diffuse.y >> diffuse.z;
                    material_param.diffuse = diffuse;
                } else if (!strcmp(prefix, "ms")) {
                    // Material specular
                    Vector specular;
                    input >> specular.x >> specular.y >> specular.z;
                    material_param.specular = specular;
                } else if (!strcmp(prefix, "me")) {
                    // Material emission
                    Vector emission;
                    input >> emission.x >> emission.y >> emission.z;
                    material_param.emission = emission;
                } else if (!strcmp(prefix, "mp")) {
                    // Material pow
                    float pow = 0.0f;
                    input >> pow;
                    material_param.pow = pow;
                } else {
                    input.getline(buffer, sizeof(buffer));
                    memset(buffer, 0, sizeof(buffer));
                }
            }

            input.close();

            // Generate the buffer
            int32_t triangle_num = face_array.size();
            *vertex_buf = new float[kVertexPerTri * triangle_num * kObjFloatPerVertex];
            for (int32_t i = 0; i < triangle_num; i++) {
                for (int32_t j = 0; j < kVertexPerTri; j++) {
                    (*vertex_buf)[i * kVertexPerTri * kObjFloatPerVertex + j * kObjFloatPerVertex + 0] = vertex_array[face_array[i].vertex_index[j] - 1].x;
                    (*vertex_buf)[i * kVertexPerTri * kObjFloatPerVertex + j * kObjFloatPerVertex + 1] = vertex_array[face_array[i].vertex_index[j] - 1].y;
                    (*vertex_buf)[i * kVertexPerTri * kObjFloatPerVertex + j * kObjFloatPerVertex + 2] = vertex_array[face_array[i].vertex_index[j] - 1].z;
                }
            }

            if (tex_array.size() > 0 && texcoord_buf != NULL) {
                *texcoord_buf = new float[kVertexPerTri * triangle_num * kObjFloatPerTexcoord];
                for (int32_t i = 0; i < triangle_num; i++) {
                    for (int32_t j = 0; j < kVertexPerTri; j++) {
                        (*texcoord_buf)[i * kVertexPerTri * kObjFloatPerTexcoord + j * kObjFloatPerTexcoord + 0] = tex_array[face_array[i].texcoord_index[j] - 1].x;
                        (*texcoord_buf)[i * kVertexPerTri * kObjFloatPerTexcoord + j * kObjFloatPerTexcoord + 1] = tex_array[face_array[i].texcoord_index[j] - 1].y;
                    }
                }
            }

            if (normal_array.size() > 0 && normal_buf != NULL) {
                *normal_buf = new float[kVertexPerTri * triangle_num * kObjFloatPerNormal];
                for (int32_t i = 0; i < triangle_num; i++) {
                    for (int32_t j = 0; j < kVertexPerTri; j++) {
                        (*normal_buf)[i * kVertexPerTri * kObjFloatPerNormal + j * kObjFloatPerNormal + 0] = normal_array[face_array[i].normal_index[j] - 1].x;
                        (*normal_buf)[i * kVertexPerTri * kObjFloatPerNormal + j * kObjFloatPerNormal + 1] = normal_array[face_array[i].normal_index[j] - 1].y;
                        (*normal_buf)[i * kVertexPerTri * kObjFloatPerNormal + j * kObjFloatPerNormal + 2] = normal_array[face_array[i].normal_index[j] - 1].z;
                    }
                }
            }

            if (tangent_array.size() > 0 && tangent_buf != NULL) {
                *tangent_buf = new float[kVertexPerTri * triangle_num * kObjFloatPerTangent];
                for (int32_t i = 0; i < triangle_num; i++) {
                    for (int32_t j = 0; j < kVertexPerTri; j++) {
                        (*tangent_buf)[i * kVertexPerTri * kObjFloatPerTangent + j * kObjFloatPerTangent + 0] = tangent_array[face_array[i].tangent_index[j] - 1].x;
                        (*tangent_buf)[i * kVertexPerTri * kObjFloatPerTangent + j * kObjFloatPerTangent + 1] = tangent_array[face_array[i].tangent_index[j] - 1].y;
                        (*tangent_buf)[i * kVertexPerTri * kObjFloatPerTangent + j * kObjFloatPerTangent + 2] = tangent_array[face_array[i].tangent_index[j] - 1].z;
                    }
                }
            }

            if (binormal_array.size() > 0 && binormal_buf != NULL) {
                *binormal_buf = new float[kVertexPerTri * triangle_num * kObjFloatPerBinormal];
                for (int32_t i = 0; i < triangle_num; i++) {
                    for (int32_t j = 0; j < kVertexPerTri; j++) {
                        (*binormal_buf)[i * kVertexPerTri * kObjFloatPerBinormal + j * kObjFloatPerBinormal + 0] = binormal_array[face_array[i].binormal_index[j] - 1].x;
                        (*binormal_buf)[i * kVertexPerTri * kObjFloatPerBinormal + j * kObjFloatPerBinormal + 1] = binormal_array[face_array[i].binormal_index[j] - 1].y;
                        (*binormal_buf)[i * kVertexPerTri * kObjFloatPerBinormal + j * kObjFloatPerBinormal + 2] = binormal_array[face_array[i].binormal_index[j] - 1].z;
                    }
                }
            }

            // Save the number of triangles
            result = face_array.size();
        } else {
            input.close();
            GLB_SAFE_ASSERT(false);
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

void ObjModelFile::ExtractFaceData(const char* buffer, int32_t& vertex_index, int32_t& texcoord_index, int32_t& normal_index, int32_t& tanget_index, int32_t& binormal_index) {
    if (buffer != NULL) {
        // Extract vertex index
        vertex_index = atoi(buffer);

        // Has texture coordinate index ?
        int32_t index = 0;
        int32_t buf_len = strlen(buffer);
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

        // Has normal index ?
        for (index = index + 1; index < buf_len; index++) {
            if (buffer[index] == '/') {
                break;
            }
        }

        if (index < buf_len - 1) {
            const char* normal_index_buf = reinterpret_cast<const char*>(buffer + index + 1);
            normal_index = atoi(normal_index_buf);
        }

        // Has tanget index ?
        for (index = index + 1; index < buf_len; index++) {
            if (buffer[index] == '/') {
                break;
            }
        }

        if (index < buf_len - 1) {
            const char* tanget_index_buf = reinterpret_cast<const char*>(buffer + index + 1);
            tanget_index = atoi(tanget_index_buf);
        }

        // Has binormal index ?
        for (index = index + 1; index < buf_len; index++) {
            if (buffer[index] == '/') {
                break;
            }
        }

        if (index < buf_len - 1) {
            const char* binormal_index_buf = reinterpret_cast<const char*>(buffer + index + 1);
            binormal_index = atoi(binormal_index_buf);
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

void ModelFile::RelaseBuf(float** vertex_buf, float** texcoord_buf, float** normal_buf, float** tanget_buf, float** binormal_buf) {
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
};  // namespace glb