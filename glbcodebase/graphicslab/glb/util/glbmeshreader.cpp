//--------------------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016/06/19
// Brief: Define a interface to analysis the mesh file
//--------------------------------------------------------------------
#include "glbmeshreader.h"

#include <fstream>
#include <vector>

#include "math/glbvector.h"

#include "util/glbmacro.h"

namespace glb {

namespace util {

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
// MeshReaderBase DECLARATION
//----------------------------------------------------------------------------------------
class MeshReaderBase {
public:
    MeshReaderBase() {
    }

    virtual ~MeshReaderBase() {
    }

public:
    virtual int32_t ExtractModelData(
        const char* meshFile, 
        math::Vector& boundMin,
        math::Vector& boundMax,
        float** vertexBuf,
        float** texcoordBuf = nullptr,
        float** lightMapTexCoordBuf = nullptr,
        float** normalBuf = nullptr,
        float** tangentBuf = nullptr,
        float** binormalBuf = nullptr
        ) = 0;
};

//----------------------------------------------------------------------------------------
// ObjMeshReader DECLARATION
//----------------------------------------------------------------------------------------
class ObjMeshReader : public MeshReaderBase {
public:
    ObjMeshReader();
    virtual ~ObjMeshReader();

public:
    int32_t ExtractModelData(
                const char* meshFile, 
                math::Vector& boundMin,
                math::Vector& boundMax,
                float** vertexBuf,
                float** texcoordBuf = nullptr,
                float** lightMapTexCoordBuf = nullptr,
                float** normalBuf = nullptr,
                float** tangentBuf = nullptr,
                float** binormalBuf = nullptr
                ) override;

protected:
    void ExtractFaceData(const char* buffer, int32_t& vertex_index, int32_t& texcoord_index, int32_t& light_map_tex_index, int32_t& normal_index, int32_t& tanget_index, int32_t& binormal_index);
};

//----------------------------------------------------------------------------------------
// CLASS DEFINITION
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
// ObjMeshReader DEFINITION
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

ObjMeshReader::ObjMeshReader() {
}
ObjMeshReader::~ObjMeshReader() {
}

int32_t ObjMeshReader::ExtractModelData(
                        const char* meshFile, 
                        math::Vector& boundMin,
                        math::Vector& boundMax,
                        float** vertexBuf,
                        float** texcoordBuf,
                        float** lightMapTexCoordBuf,
                        float** normalBuf,
                        float** tangentBuf,
                        float** binormalBuf
                        ) {
    int32_t result = 0;

    if (meshFile != nullptr && vertexBuf != nullptr) {
        std::ifstream input;
        input.open(meshFile);

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

            bool hasTextureCoordinate = false;
            bool hasLightTextureCoordinate = false;
            bool hasNormal = false, hasTangent = false, hasBinormal = false;

            // Read the data from file
            while (!input.eof()) {
                input >> prefix;

                if (!strcmp(prefix, "v")) {
                    // Vertex
                    math::Vector vertex;
                    input >> vertex.x >> vertex.y >> vertex.z;
                    vertexArray.push_back(vertex);

                    if (vertex.x < boundMin.x) {
                        boundMin.x = vertex.x;
                    } else if (vertex.x > boundMax.x) {
                        boundMax.x = vertex.x;
                    }

                    if (vertex.y < boundMin.y) {
                        boundMin.y = vertex.y;
                    } else if (vertex.y > boundMax.y) {
                        boundMax.y = vertex.y;
                    }

                    if (vertex.z < boundMin.z) {
                        boundMin.z = vertex.z;
                    } else if (vertex.z > boundMax.z) {
                        boundMax.z = vertex.z;
                    }
                } else if (!strcmp(prefix, "vt")) {
                    // Texture Coordinate
                    math::Vector texcoord;
                    input >> texcoord.x >> texcoord.y;
                    texArray.push_back(texcoord);

                    hasTextureCoordinate = true;
                } else if (!strcmp(prefix, "vlightmapt")) {
                    // Light Map Texture Coordinate
                    math::Vector texcoord;
                    input >> texcoord.x >> texcoord.y;
                    lightMapTexArray.push_back(texcoord);

                    hasLightTextureCoordinate = true;
                } else if (!strcmp(prefix, "vn")) {
                    // Normal
                    math::Vector normal;
                    input >> normal.x >> normal.y >> normal.z;
                    normalArray.push_back(normal);

                    hasNormal = true;
                } else if (!strcmp(prefix, "vtan")) {
                    // Tangent
                    math::Vector tanget;
                    input >> tanget.x >> tanget.y >> tanget.z;
                    tangentArray.push_back(tanget);

                    hasTangent = true;
                } else if (!strcmp(prefix, "vbi")) {
                    // Binormal
                    math::Vector binormal;
                    input >> binormal.x >> binormal.y >> binormal.z;
                    binormalArray.push_back(binormal);

                    hasBinormal = true;
                } else if (!strcmp(prefix, "f")) {
                    // Triangle
                    ObjFace face;

                    if (!hasTextureCoordinate) {
                        for (int32_t i = 0; i < kVertexPerTri; i++) {
                            face.texcoord_index[i] = -1;  // Indicate do not have texture coordinate
                        }
                    }
                    if (!hasLightTextureCoordinate) {
                        for (int32_t i = 0; i < kVertexPerTri; i++) {
                            face.lightmap_texcoord_index[i] = -1;  // Indicate do not have light map texture coordinate
                        }
                    }
                    if (!hasNormal) {
                        for (int32_t i = 0; i < kVertexPerTri; i++) {
                            face.normal_index[i] = -1;  // Indicate do not have normal
                        }
                    }
                    if (!hasTangent) {
                        for (int32_t i = 0; i < kVertexPerTri; i++) {
                            face.tangent_index[i] = -1;  // Indicate do not have tangent
                        }
                    }
                    if (!hasBinormal) {
                        for (int32_t i = 0; i < kVertexPerTri; i++) {
                            face.binormal_index[i] = -1;  // Indicate do not have binormal
                        }
                    }
                    for (int32_t i = 0; i < kVertexPerTri; i++) {
                        input >> buffer;
                        ExtractFaceData(buffer, face.vertex_index[i], face.texcoord_index[i], face.lightmap_texcoord_index[i], face.normal_index[i], face.tangent_index[i], face.binormal_index[i]);
                    }

                    faceArray.push_back(face);
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

            if (texArray.size() > 0 && texcoordBuf != nullptr) {
                *texcoordBuf = new float[kVertexPerTri * triangle_num * kObjFloatPerTexcoord];
                for (int32_t i = 0; i < triangle_num; i++) {
                    for (int32_t j = 0; j < kVertexPerTri; j++) {
                        (*texcoordBuf)[i * kVertexPerTri * kObjFloatPerTexcoord + j * kObjFloatPerTexcoord + 0] = texArray[faceArray[i].texcoord_index[j] - 1].x;
                        (*texcoordBuf)[i * kVertexPerTri * kObjFloatPerTexcoord + j * kObjFloatPerTexcoord + 1] = texArray[faceArray[i].texcoord_index[j] - 1].y;
                    }
                }
            }

            if (lightMapTexArray.size() > 0 && lightMapTexCoordBuf != nullptr) {
                *lightMapTexCoordBuf = new float[kVertexPerTri * triangle_num * kObjFloatPerTexcoord];
                for (int32_t i = 0; i < triangle_num; i++) {
                    for (int32_t j = 0; j < kVertexPerTri; j++) {
                        (*lightMapTexCoordBuf)[i * kVertexPerTri * kObjFloatPerTexcoord + j * kObjFloatPerTexcoord + 0] = lightMapTexArray[faceArray[i].lightmap_texcoord_index[j] - 1].x;
                        (*lightMapTexCoordBuf)[i * kVertexPerTri * kObjFloatPerTexcoord + j * kObjFloatPerTexcoord + 1] = lightMapTexArray[faceArray[i].lightmap_texcoord_index[j] - 1].y;
                    }
                }
            }

            if (normalArray.size() > 0 && normalBuf != nullptr) {
                *normalBuf = new float[kVertexPerTri * triangle_num * kObjFloatPerNormal];
                for (int32_t i = 0; i < triangle_num; i++) {
                    for (int32_t j = 0; j < kVertexPerTri; j++) {
                        (*normalBuf)[i * kVertexPerTri * kObjFloatPerNormal + j * kObjFloatPerNormal + 0] = normalArray[faceArray[i].normal_index[j] - 1].x;
                        (*normalBuf)[i * kVertexPerTri * kObjFloatPerNormal + j * kObjFloatPerNormal + 1] = normalArray[faceArray[i].normal_index[j] - 1].y;
                        (*normalBuf)[i * kVertexPerTri * kObjFloatPerNormal + j * kObjFloatPerNormal + 2] = normalArray[faceArray[i].normal_index[j] - 1].z;
                    }
                }
            }

            if (tangentArray.size() > 0 && tangentBuf != nullptr) {
                *tangentBuf = new float[kVertexPerTri * triangle_num * kObjFloatPerTangent];
                for (int32_t i = 0; i < triangle_num; i++) {
                    for (int32_t j = 0; j < kVertexPerTri; j++) {
                        (*tangentBuf)[i * kVertexPerTri * kObjFloatPerTangent + j * kObjFloatPerTangent + 0] = tangentArray[faceArray[i].tangent_index[j] - 1].x;
                        (*tangentBuf)[i * kVertexPerTri * kObjFloatPerTangent + j * kObjFloatPerTangent + 1] = tangentArray[faceArray[i].tangent_index[j] - 1].y;
                        (*tangentBuf)[i * kVertexPerTri * kObjFloatPerTangent + j * kObjFloatPerTangent + 2] = tangentArray[faceArray[i].tangent_index[j] - 1].z;
                    }
                }
            }

            if (binormalArray.size() > 0 && binormalBuf != nullptr) {
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

void ObjMeshReader::ExtractFaceData(const char* buffer, int32_t& vertexIndex, int32_t& texcoordIndex, int32_t& lightMapTexIndex, int32_t& normalIndex, int32_t& tangetIndex, int32_t& binormalIndex) {
    if (buffer != nullptr) {
        // Extract vertex index
        vertexIndex = atoi(buffer);

        // Has texture coordinate index ?
        int32_t index = 0;
        int32_t bufLen = strlen(buffer);

        if (texcoordIndex != -1) {
            for (index = 0; index < bufLen; index++) {
                if (buffer[index] == '/') {
                    break;
                }
            }

            if (index < bufLen - 1) {
                if (buffer[index + 1] != '/') {
                    const char* texIndexBuf = reinterpret_cast<const char*>(buffer + index + 1);
                    texcoordIndex = atoi(texIndexBuf);
                }
            }
        }

        // Has light map texture coordinate index ?
        if (lightMapTexIndex != -1) {
            for (index = index+ 1; index < bufLen; index++) {
                if (buffer[index] == '/') {
                    break;
                }
            }

            if (index < bufLen - 1) {
                const char* lightMapTexIndexBuf = reinterpret_cast<const char*>(buffer + index + 1);
                lightMapTexIndex = atoi(lightMapTexIndexBuf);
            }
        }

        // Has normal index ?
        if (normalIndex != -1) {
            for (index = index + 1; index < bufLen; index++) {
                if (buffer[index] == '/') {
                    break;
                }
            }

            if (index < bufLen - 1) {
                const char* normalIndexBuf = reinterpret_cast<const char*>(buffer + index + 1);
                normalIndex = atoi(normalIndexBuf);
            }
        }

        // Has tanget index ?
        if (tangetIndex != -1) {
            for (index = index + 1; index < bufLen; index++) {
                if (buffer[index] == '/') {
                    break;
                }
            }

            if (index < bufLen - 1) {
                const char* tangetIndexBuf = reinterpret_cast<const char*>(buffer + index + 1);
                tangetIndex = atoi(tangetIndexBuf);
            }
        }

        // Has binormal index ?
        if (binormalIndex != -1) {
            for (index = index + 1; index < bufLen; index++) {
                if (buffer[index] == '/') {
                    break;
                }
            }

            if (index < bufLen - 1) {
                const char* binormalIndexBuf = reinterpret_cast<const char*>(buffer + index + 1);
                binormalIndex = atoi(binormalIndexBuf);
            }
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }
}

//----------------------------------------------------------------------------------------
// ModelFile DEFINITION
//----------------------------------------------------------------------------------------
int32_t MeshReader::ExtractModelData(
                    const char* meshFile,
                    math::Vector& boundMin,
                    math::Vector& boundMax,
                    float** vertexBuf,
                    float** texcoordBuf,
                    float** lightMapTexCoordBuf,
                    float** normalBuf,
                    float** tangentBuf,
                    float** binormalBuf
                    ) {
    int32_t result = 0;

    if (meshFile != nullptr && vertexBuf != nullptr) {
        char postfix[kMaxFilePostFixLength];
        int32_t len = strlen(meshFile);
        int32_t dot_index = 0;
        for (; dot_index < len; dot_index++) {
            if (meshFile[dot_index] == '.') {
                break;
            }
        }

        // Has dot?
        if (dot_index < len - 1) {
            memcpy(postfix, meshFile + dot_index, len - dot_index);
            postfix[len - dot_index] = 0;

            if (!strcmp(postfix, ".obj")) {
                ObjMeshReader objMesh;
                result = objMesh.ExtractModelData(
                    meshFile,
                    boundMin,
                    boundMax,
                    vertexBuf,
                    texcoordBuf,
                    lightMapTexCoordBuf,
                    normalBuf,
                    tangentBuf,
                    binormalBuf);
            } else {
                GLB_SAFE_ASSERT(false);
            }
        }
    } else {
        GLB_SAFE_ASSERT(false);
    }

    return result;
}

void MeshReader::RelaseBuf(float** vertexBuf, float** texcoordBuf, float** lightMapTexCoordBuf, float** normalBuf, float** tangetBuf, float** binormalBuf) {
    if (vertexBuf != nullptr) {
        float* buf = *vertexBuf;
        if (buf != nullptr) {
            delete[] buf;
            buf = nullptr;
        }
        *vertexBuf = nullptr;
    }

    if (texcoordBuf != nullptr) {
        float* buf = *texcoordBuf;
        if (buf != nullptr) {
            delete[] buf;
            buf = nullptr;
        }
        *texcoordBuf = nullptr;
    }

    if (lightMapTexCoordBuf != nullptr) {
        float* buf = *lightMapTexCoordBuf;
        if (buf != nullptr) {
            delete[] buf;
            buf = nullptr;
        }
        *lightMapTexCoordBuf = nullptr;
    }

    if (normalBuf != nullptr) {
        float* buf = *normalBuf;
        if (buf != nullptr) {
            delete[] buf;
            buf = nullptr;
        }
        *normalBuf = nullptr;
    }

    if (tangetBuf != nullptr) {
        float* buf = *tangetBuf;
        if (buf != nullptr) {
            delete[] buf;
            buf = nullptr;
        }
        *tangetBuf = nullptr;
    }

    if (binormalBuf != nullptr) {
        float* buf = *binormalBuf;
        if (buf != nullptr) {
            delete[] buf;
            buf = nullptr;
        }
        *binormalBuf = nullptr;
    }
}

};  // namespace scene

};  // namespace glb