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

//------------------------------------------------------
// @Constant
//------------------------------------------------------
static const int32_t kMaxVertexAttributeName = 32;
static const int32_t kMaxVertexLayoutAttributes = 8;
static const int32_t kMaxUniformName = 64;

//------------------------------------------------------
// @Vertex Declartion
//------------------------------------------------------
enum VertexAttribute {
    VA_UNKNOWN = 0,
    VA_POS,
    VA_TEXCOORD,
    VA_NORMAL,
    VA_TANGENT,
    VA_BINORMAL,
    VA_COLOR,
};

struct VertexLayoutAttribute {
    VertexAttribute attriType;
    int32_t         offset;
    int32_t         size;
};

struct VertexLayout {
    int32_t               count;
    VertexLayoutAttribute layouts[kMaxVertexLayoutAttributes];
};

struct ShaderLayoutAttribute {
    VertexAttribute attriType;
    int32_t         location;
    char            name[kMaxVertexAttributeName];
};

struct ShaderLayout {
    int32_t               count;
    ShaderLayoutAttribute layouts[kMaxVertexLayoutAttributes];
};

};  // namespace glb

#endif  // GLB_GLBSTRUCTS_H_