//--------------------------------------------------------
// Declaration: Copyright (c), by i_dovelemon, 2016. All right reserved.
// Author: i_dovelemon[1322600812@qq.com]
// Date: 2016 / 09 / 19
// Brief: Define some structs and enums
//--------------------------------------------------------
#ifndef GLB_GLBSTRUCTS_H_
#define GLB_GLBSTRUCTS_H_

#include <stdint.h>

namespace glb {

namespace render {

//------------------------------------------------------
// @Constant
//------------------------------------------------------
static const int32_t kMaxVertexAttributeName = 32;
static const int32_t kMaxUniformName = 64;

//------------------------------------------------------
// @Vertex Declartion
//------------------------------------------------------
enum VertexAttribute {
    VA_UNKNOWN = 0,
    VA_POS,
    VA_TEXCOORD,
    VA_LIGHT_MAP_TEXCOORD,
    VA_NORMAL,
    VA_TANGENT,
    VA_BINORMAL,
    VA_WORLDMATRIX,
    VA_TRANSINVWORLDMATRIX,
    VA_COLOR,
    VA_MAX = VA_COLOR,
};

struct VertexLayoutAttribute {
    VertexAttribute attriType;
    int32_t         offset;
    int32_t         size;
};

struct VertexLayout {
    int32_t               count;
    VertexLayoutAttribute layouts[VA_MAX];
};

struct ShaderLayoutAttribute {
    VertexAttribute attriType;
    int32_t         location;
    char            name[kMaxVertexAttributeName];
};

struct ShaderLayout {
    int32_t               count;
    ShaderLayoutAttribute layouts[VA_MAX];
};

// Parameter's data type
enum ParameterFormat {
    PARAMETER_FORMAT_INT = 0,
    PARAMETER_FORMAT_FLOAT,
    PARAMETER_FORMAT_FLOAT3,
    PARAMETER_FORMAT_FLOAT4,
    PARAMETER_FORMAT_TEXTURE_2D,
    PARAMETER_FORMAT_TEXTURE_3D,
    PARAMETER_FORMAT_TEXTURE_CUBE,
    PARAMETER_FORMAT_MATRIX,
};

struct ShaderParameter {
    ParameterFormat format;
    char            name[kMaxUniformName];
};

// Shader Pass name
static const char* kLightLoopPassName = "lightloop";
static const char* kShadowPassName = "shadow";
static const char* kDecalPassName = "decal";

};  // namespace render

};  // namespace glb

#endif  // GLB_GLBSTRUCTS_H_